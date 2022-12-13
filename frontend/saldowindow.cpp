#include "saldowindow.h"
#include "ui_saldowindow.h"
#include "url.h"
#include "bankwindow.h"

saldoWindow::saldoWindow(QByteArray token,QString cardnum,bool cardType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saldoWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-image: url(:/icons/talvi.jpg)");
    this->setWindowState(Qt::WindowMaximized);
    qDebug()<<"saldo konstruktori";
    card_number = cardnum;
    webToken=token;
    ui->timer->setPalette(Qt::red);
    ui->timer->setAutoFillBackground(true);
    QPalette Pal = ui->timer->palette();
    Pal.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
    Pal.setColor(QPalette::Normal, QPalette::Window, Qt::black);
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setStyleSheet("QTableView {selection-background-color: green;}");
    ui->timer->setPalette(Pal);
    if(cardType==true)//debit käytössä = false , credit käytössä = true
    {
        credit=true;
    }
    else
    {
        credit=false;
    }
    QString site_url=url::getBaseUrl()+"cards/"+card_number;
    QNetworkRequest request((site_url));
    qDebug()<<site_url;
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    saldoManager = new QNetworkAccessManager(this);
    connect(saldoManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(saldoSlot(QNetworkReply*)));
    reply = saldoManager->get(request);

}

saldoWindow::~saldoWindow()
{
    delete ui;
}

void saldoWindow::delay()
{
    int afkTimer=1;
    QTime dieTime= QTime::currentTime().addSecs(afkTimer);
     while (QTime::currentTime() < dieTime) {
         QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
     }
}
void saldoWindow::saldoSlot(QNetworkReply *reply)
{
    QByteArray response_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    QString account = QString::number(json_obj["id_account"].toInt());
    QString card_number = QString::number(json_obj["card_number"].toInt());
    QString debit_credit = json_obj["debit_credit"].toString();
    QString card_owner = json_obj["card_owner"].toString();
    QString lista;

    lista += "Account: " + account + "\nCard number: "+card_number + "\nDebit/Credit: "+debit_credit + "\nCard owner: "+card_owner;
    ui->labelAsiakas->setText(lista);
    reply->deleteLater();
    saldoManager->deleteLater();

    QString site_url=url::getBaseUrl()+"accounts/"+account;
    QNetworkRequest request((site_url));
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    asiakasManager = new QNetworkAccessManager(this);
    connect(asiakasManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(asiakasSlot(QNetworkReply*)));
    reply = asiakasManager->get(request);

    for (int i = 10; i >= 0; i--) {
        delay();
        ui->timer->display(i);

        if (i == 0&& this->isHidden()==false) {
            bankwindow *bank = new bankwindow(webToken,card_number,credit);
            bank->show();
            close();
        }
    }
}

void saldoWindow::asiakasSlot(QNetworkReply *reply)
{
    if(credit==false){
    QByteArray response_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    QString Saldo=QString::number(json_obj["debit_balance"].toDouble());
    QString account;
    account = QString::number(json_obj["id_account"].toInt());
    ui->labelSaldo->setText("Saldo: "+Saldo);
    QString site_url=url::getBaseUrl()+"transactions/";
    QNetworkRequest request((site_url));
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    tapahtumaManager = new QNetworkAccessManager(this);
    connect(tapahtumaManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(tapahtumaSlot(QNetworkReply*)));
    reply = tapahtumaManager->get(request);}
    else{
        QByteArray response_data=reply->readAll();
        qDebug()<<response_data;
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonObject json_obj = json_doc.object();
        QString creditLimit=QString::number(json_obj["credit_limit"].toDouble());
        QString usedCredit=QString::number(json_obj["used_credit"].toDouble());
        QString account;
        account = QString::number(json_obj["id_account"].toInt());
        ui->labelSaldo->setText("Luottoraja: "+creditLimit+"\n"+"Käytetty luotto: "+usedCredit);
        QString site_url=url::getBaseUrl()+"transactions/";

        QNetworkRequest request((site_url));
        request.setRawHeader(QByteArray("Authorization"),(webToken));
        tapahtumaManager = new QNetworkAccessManager(this);
        connect(tapahtumaManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(tapahtumaSlot(QNetworkReply*)));
        reply = tapahtumaManager->get(request);
    }
}

void saldoWindow::tapahtumaSlot(QNetworkReply *reply) {
    QByteArray response_data=reply->readAll();
    qDebug()<<response_data;
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();
    QJsonArray array = {};
    QJsonArray omatTapahtumat = {};
    QJsonArray omatTapahtumatVikat = {};
    QString id_transactions;
    QString lista;
    QJsonValue values;
    int listaIndex = 0;

    for (int i = 0; i < json_array.size();i++) {
        array.insert(i,json_array.at(i));
        QJsonValue items = json_array.at(i);
        QJsonObject obj = items.toObject();
        id_transactions = QString::number(obj["id_transactions"].toInt());
        QString card_numbers = QString::number(obj["card_number"].toInt());
        QString sums = QString::number(obj["sum"].toInt());
        QString dates = obj["date"].toString();
        QString descriptions = obj["description"].toString();
        if (card_numbers == saldoWindow::card_number) {
            omatTapahtumat.insert(listaIndex,items);
            listaIndex++;

        }


    }
    int size = omatTapahtumat.size() -1;
    for (int i = 0;i <5;i++) {
        omatTapahtumatVikat.insert(i,omatTapahtumat.at(size));
        qDebug() << "LISÄTTY: " << omatTapahtumat.at(size);
        QJsonValue items = omatTapahtumatVikat.at(i);
        QJsonObject obj = items.toObject();
        id_transactions = QString::number(obj["id_transactions"].toInt());
        QString numerot= QString::number(obj["card_number"].toInt());
        QString summat = QString::number(obj["sum"].toInt());
        QString pvm = obj["date"].toString();
        QString kuvaus = obj["description"].toString();
        lista += "Transaction: " + id_transactions + "Card number: "+numerot + "Sum: "+summat + "Date: "+pvm + "Description: "+kuvaus+"\n";
        m_TableHeader<<"ID"<<"Kortin numero"<<"Summa"<<"Päivämäärä"<<"Tapahtuma";
        ui->tableWidget->setHorizontalHeaderLabels(m_TableHeader);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(id_transactions));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(numerot));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(summat));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(pvm));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(kuvaus));
        size--;
    }
}

void saldoWindow::on_suljeButton_clicked()
{
    bankwindow *bank = new bankwindow(webToken, card_number,credit);
    bank->show();
    close();
}
