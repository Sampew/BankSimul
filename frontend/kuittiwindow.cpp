#include "kuittiwindow.h"
#include "ui_kuittiwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QCoreApplication>
#include <QHeaderView>
#include <QMessageBox>
#include "bankwindow.h"

kuittiwindow::kuittiwindow(QByteArray token,QString cardnum,bool cardType,double nostoMaara,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kuittiwindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-image: url(:/icons/talvi.jpg)");
    this->setWindowState(Qt::WindowMaximized);
    webToken = token;
    card_number = cardnum;
    nosto=QString::number(nostoMaara);;
    if(cardType==true)//debit käytössä = false , credit käytössä = true
    {
        credit=true;
    }
    else
    {
        credit=false;
    }
    ui->timer->setPalette(Qt::red);
    ui->timer->setAutoFillBackground(true);
    QPalette Pal = ui->timer->palette();
    Pal.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
    Pal.setColor(QPalette::Normal, QPalette::Window, Qt::black);
    ui->timer->setPalette(Pal);

    QString site_url=url::getBaseUrl()+"cards/" + card_number;
    QNetworkRequest request((site_url));
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    kuittiManager = new QNetworkAccessManager(this);

    connect(kuittiManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(kuittiSlot(QNetworkReply*)));
    reply = kuittiManager->get(request);
}

kuittiwindow::~kuittiwindow()
{
    qDebug() << "kuitti destruktori";
    delete ui;
}

void kuittiwindow::delay()
{
    int afkTimer=1;
    QTime dieTime= QTime::currentTime().addSecs(afkTimer);
     while (QTime::currentTime() < dieTime)
         QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void kuittiwindow::kuittiSlot(QNetworkReply *reply)
{
    QByteArray response_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    account = QString::number(json_obj["id_account"].toInt());
    cardNum = QString::number(json_obj["card_number"].toInt());
    card_owner = json_obj["card_owner"].toString();

    QString site_url=url::getBaseUrl()+"accounts/" + account;
    QNetworkRequest request((site_url));
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    asiakasManager = new QNetworkAccessManager(this);

    connect(asiakasManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(asiakasSlot(QNetworkReply*)));
    reply = asiakasManager->get(request);


}

void kuittiwindow::asiakasSlot(QNetworkReply *reply)
{
    QByteArray response_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    debitBalance = QString::number(json_obj["debit_balance"].toDouble());
    usedCredit = QString::number(json_obj["used_credit"].toDouble());
    creditLimit = QString::number(json_obj["credit_limit"].toDouble());
    QDateTime time= QDateTime::currentDateTime();
    QString date = time.toString("dd.MM.yyyy hh:mm:ss");
    if(credit==false){
    ui->labelKuitti->setText("Kuitti tapahtumasta\n\nNostettu summa: " + nosto + "\nUusi saldo: " + debitBalance +
                             "\nKortin omistaja: " + card_owner+ "\nKortin numero: " + cardNum + "\nPäivämäärä: " + date);
}
    else
    {
    ui->labelKuitti->setText("Kuitti tapahtumasta\n\nNostettu summa: " + nosto + "\nKäytetty luotto: " + usedCredit +
                             "\nLuottoraja:" + creditLimit +
                             "\nKortin omistaja: " + card_owner+ "\nKortin numero: " + cardNum + "\nPäivämäärä: " + date);
    }

    for (aika = 10; aika >= 0; aika--) {
        delay();
        ui->timer->display(aika);

        if (aika == 0&& this->isHidden()==false) {
            bankwindow *bank = new bankwindow(webToken,card_number,credit);
            bank->show();
            close();
        }
    }

}

void kuittiwindow::on_pushButton_clicked()
{
    bankwindow *main = new bankwindow(webToken,card_number,credit);
    main->show();
    close();
}
void kuittiwindow::close_window() {
    close();
}
