#include "nostosummawindow.h"
#include "ui_nostosummawindow.h"
#include "url.h"
#include "bankwindow.h"

nostoSummaWindow::nostoSummaWindow(QByteArray token, QString myCard, bool cardType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::nostoSummaWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-image: url(:/icons/talvi.jpg)");
    this->setWindowState(Qt::WindowMaximized);
    qDebug()<<"nostosumma konstruktori";
    webToken=token;
    cardnum=myCard;
    ui->timer->setPalette(Qt::red);
    ui->timer->setAutoFillBackground(true);
    QPalette Pal = ui->timer->palette();
    Pal.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
    Pal.setColor(QPalette::Normal, QPalette::Window, Qt::black);
    ui->timer->setPalette(Pal);
    if(cardType==true)//debit käytössä = false , credit käytössä = true
    {
        credit=true;
    }
    else
    {
        credit=false;
    }
    qDebug()<<"kortti nostossa on"<<credit;
    QString site_url=url::getBaseUrl()+"cards/"+myCard;
    QNetworkRequest request((site_url));
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    nostoManager = new QNetworkAccessManager(this);
    connect(nostoManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(nostoSlot(QNetworkReply*)));
    reply = nostoManager->get(request);

    ui->nimi_label->setText(myCard+"\n");
    ui->kuittiButton->hide();
    ui->lineEdit->hide();
    ui->muu_info->hide();
    ui->confirmButton->hide();
    ui->jakolabel->hide();

    ui->kumiButton->hide();
    ui->nollaButton->hide();
    ui->yksiButton->hide();
    ui->kaksiButton->hide();
    ui->kolmeButton->hide();
    ui->neljaButton->hide();
    ui->viisiButton->hide();
    ui->kuusiButton->hide();
    ui->seitsemanButton->hide();
    ui->kahdeksanButton->hide();
    ui->yhdeksanButton->hide();

}
nostoSummaWindow::~nostoSummaWindow()
{
    delete ui;
}
void nostoSummaWindow::nostoSlot(QNetworkReply *reply)
{
    if(credit==false){
    QByteArray response_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    QString omistaja=json_obj["card_owner"].toString();
    QString Saldo=QString::number(json_obj["debit_balance"].toDouble());

    id_account=QString::number(json_obj["id_account"].toInt());
    ui->nimi_label->setText("Kortin numero: " + cardnum + "\nKortin omistaja: " +omistaja);

    QString site_url=url::getBaseUrl()+"accounts/"+id_account;
    QNetworkRequest request((site_url));
    qDebug()<<site_url;
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    balanceManager = new QNetworkAccessManager(this);
    connect(balanceManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(balanceSlot(QNetworkReply*)));
    reply = balanceManager->get(request);
    for (aika = 10; aika >= 0; aika--) {
        delay();
        ui->timer->display(aika);
        if (aika == 0&& this->isHidden()==false) {
            bankwindow *bank = new bankwindow(webToken,cardnum,credit);
            bank->show();
            close();
        }
    }
    }else if(credit==true){
        QByteArray response_data=reply->readAll();
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonObject json_obj = json_doc.object();
        QString omistaja=json_obj["card_owner"].toString();
        QString Saldo=QString::number(json_obj["credit_limit"].toDouble());
        id_account=QString::number(json_obj["id_account"].toInt());

        ui->nimi_label->setText(omistaja);

        QString site_url=url::getBaseUrl()+"accounts/"+id_account;
        QNetworkRequest request((site_url));
        request.setRawHeader(QByteArray("Authorization"),(webToken));
        balanceManager = new QNetworkAccessManager(this);
        connect(balanceManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(balanceSlot(QNetworkReply*)));
        reply = balanceManager->get(request);
        for (aika = 10; aika >= 0; aika--) {
            delay();
            ui->timer->display(aika);
            if (aika == 0&& this->isHidden()==false) {
                bankwindow *bank = new bankwindow(webToken,cardnum,credit);
                bank->show();
                close();
            }
        }

    }
  }
void nostoSummaWindow::balanceSlot(QNetworkReply *reply)
{


    if(credit==false){
    QByteArray response_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    balance=QString::number(json_obj["debit_balance"].toDouble());
    creditLimit = QString::number(json_obj["credit_limit"].toDouble());
    usedCredit = QString::number(json_obj["used_credit"].toDouble());
    ui->kyhny_info->setText("Tilillä katetta: "+balance);
    }
    if(credit==true)
        {
        QByteArray response_data=reply->readAll();
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonObject json_obj = json_doc.object();
        balance=QString::number(json_obj["used_credit"].toDouble());
        debitBalance = QString::number(json_obj["debit_balance"].toDouble());
        creditLimit = QString::number(json_obj["credit_limit"].toDouble());
        ui->kyhny_info->setText("Luottoraja on: "+creditLimit + "\nKäytetty: " + balance);
    }

    QString site_url=url::getBaseUrl()+"transactions/";
    qDebug()<<site_url;
    QNetworkRequest request((site_url));
    //WEBTOKEN ALKU
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    //WEBTOKEN LOPPU
    transactionManager = new QNetworkAccessManager(this);
    connect(transactionManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(transactionSlot(QNetworkReply*)));
    reply = transactionManager->get(request);
}



void nostoSummaWindow::on_pushButton20e_clicked()
{
    aika = 10;
    nostoMaara=20;
    nosto=balance.toDouble();
    countMoney(nosto,20);
    ui->pushButton20e->hide();
    ui->pushButton40e->hide();
    ui->pushButton60e->hide();
    ui->pushButton100e->hide();
    ui->pushButton200e->hide();
    ui->pushButton500e->hide();
    ui->muuButton->hide();
    ui->label->hide();
}
void nostoSummaWindow::on_pushButton40e_clicked()
{
    aika = 10;
    nostoMaara=40;
    nosto=balance.toDouble();
    countMoney(nosto,40);
    ui->pushButton20e->hide();
    ui->pushButton40e->hide();
    ui->pushButton60e->hide();
    ui->pushButton100e->hide();
    ui->pushButton200e->hide();
    ui->pushButton500e->hide();
    ui->muuButton->hide();
    ui->label->hide();
}
void nostoSummaWindow::on_pushButton60e_clicked()
{
    aika = 10;
    nostoMaara=60;
    nosto=balance.toDouble();
    countMoney(nosto,60);
    ui->pushButton20e->hide();
    ui->pushButton40e->hide();
    ui->pushButton60e->hide();
    ui->pushButton100e->hide();
    ui->pushButton200e->hide();
    ui->pushButton500e->hide();
    ui->muuButton->hide();
    ui->label->hide();
}
void nostoSummaWindow::on_pushButton100e_clicked()
{
    aika = 10;
    nostoMaara=100;
    nosto=balance.toDouble();
    countMoney(nosto,100);
    ui->pushButton20e->hide();
    ui->pushButton40e->hide();
    ui->pushButton60e->hide();
    ui->pushButton100e->hide();
    ui->pushButton200e->hide();
    ui->pushButton500e->hide();
    ui->muuButton->hide();
    ui->label->hide();
}
void nostoSummaWindow::on_pushButton200e_clicked()
{
    aika = 10;
    nostoMaara=200;
    nosto=balance.toDouble();
    countMoney(nosto,200);
    ui->pushButton20e->hide();
    ui->pushButton40e->hide();
    ui->pushButton60e->hide();
    ui->pushButton100e->hide();
    ui->pushButton200e->hide();
    ui->pushButton500e->hide();
    ui->muuButton->hide();
    ui->label->hide();
}
void nostoSummaWindow::on_pushButton500e_clicked()
{
    aika = 10;
    nostoMaara=500;
    nosto=balance.toDouble();
    countMoney(nosto,500);
    ui->pushButton20e->hide();
    ui->pushButton40e->hide();
    ui->pushButton60e->hide();
    ui->pushButton100e->hide();
    ui->pushButton200e->hide();
    ui->pushButton500e->hide();
    ui->muuButton->hide();
    ui->label->hide();
}

void nostoSummaWindow::on_suljeButton_clicked()
{
    bankwindow *bank = new bankwindow(webToken, cardnum,credit);
    bank->show();
    close();
}

void nostoSummaWindow::delay()
{
    int afkTimer=1;
    QTime dieTime= QTime::currentTime().addSecs(afkTimer);
     while (QTime::currentTime() < dieTime)
         QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void nostoSummaWindow::transactionSlot(QNetworkReply *reply){

    QByteArray response_data=reply->readAll();
    qDebug()<<response_data;
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();
    QJsonArray array = {};
    QJsonArray rightIDList = {};
    QJsonObject obj;
    int listIndex = 0;
    for (int i = 0; i < json_array.size();i++) {
        array.insert(i,json_array.at(i));
        QJsonValue items = json_array.at(i);
        obj = items.toObject();
        QString id_transactions = QString::number(obj["id_transactions"].toInt());

        rightIDList.insert(listIndex,id_transactions);
        listIndex++;
    }
    QJsonValue lastItem = rightIDList.last();
    lastID = lastItem.toString().toInt()+1;
    qDebug() << "last transactions ID: " << lastID;

}
void nostoSummaWindow::updateSlot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    reply->deleteLater();
    updateManager->deleteLater();
}
void nostoSummaWindow::countMoney(double omaSaldo, double nostoSumma)
{
    QDateTime time= QDateTime::currentDateTime();
    QString date = time.toString("dd.MM.yyyy hh:mm:ss");
     if(credit==false)
    {
         if (nostoSumma <= 0) {
            for (int i = 3; i >= 0; i--) {
                delay();
                ui->timer->display(i);
                ui->nosto_info->setText("Virheellinen summa. Suljetaan...");
                if (i == 0&& this->isHidden()==false) {
                    bankwindow *bank = new bankwindow(webToken,cardnum,credit);
                    bank->show();
                    close();
                }
            }
         }else if(omaSaldo<nostoSumma){
              ui->nosto_info->setText("Tilillä ei riittävästi katetta.");
         }else if(omaSaldo>=nostoSumma){

            QJsonObject jsonObjTrans;
            jsonObjTrans.insert("id_transactions",lastID);
            jsonObjTrans.insert("card_number",cardnum);
            jsonObjTrans.insert("sum",nostoSumma);
            jsonObjTrans.insert("date",date);
            jsonObjTrans.insert("description","nosto debit");
            QString site_url2=url::getBaseUrl()+"/transactions/";
            QNetworkRequest request2((site_url2));
            omaSaldo=omaSaldo-nostoSumma;
            ui->nosto_info->setText("Nosto onnistui");
            QJsonObject jsonObj;
            jsonObj.insert("debit_balance",omaSaldo);
            jsonObj.insert("credit_limit",creditLimit);
            jsonObj.insert("used_credit",usedCredit);

            QString site_url=url::getBaseUrl()+"/accounts/"+id_account;

            QNetworkRequest request((site_url));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            //WEBTOKEN ALKU
            request.setRawHeader(QByteArray("Authorization"),(webToken));
            request2.setRawHeader(QByteArray("Authorization"),(webToken));
            //WEBTOKEN LOPPU
            updateManager = new QNetworkAccessManager(this);
            connect(updateManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateSlot(QNetworkReply*)));
            reply = updateManager->put(request, QJsonDocument(jsonObj).toJson());
            reply = updateManager->post(request2, QJsonDocument(jsonObjTrans).toJson());
            balance = QString::number(omaSaldo);
            ui->kyhny_info->setText("Katetta jäljellä: " +balance);
            ui->kuittiButton->show();
        }
    }
    if (credit==true)
    {

        omaSaldo=omaSaldo+nostoSumma;

        creditMax=creditLimit.toDouble();
        if (nostoSumma <= 0) {
           ui->nosto_info->setText("Virheellinen summa syötetty.");
        }else if(creditMax>=omaSaldo)
        {
        ui->nosto_info->setText("Nosto onnistui");
        QJsonObject jsonObjTrans;
        jsonObjTrans.insert("id_transactions",lastID);
        jsonObjTrans.insert("card_number",cardnum);
        jsonObjTrans.insert("sum",nostoSumma);
        jsonObjTrans.insert("date",date);
        jsonObjTrans.insert("description","nosto credit");
        QString site_url2=url::getBaseUrl()+"/transactions/";
        QNetworkRequest request2((site_url2));

        QJsonObject jsonObj;
        jsonObj.insert("credit_limit",creditLimit);
        jsonObj.insert("debit_balance",debitBalance);
        jsonObj.insert("used_credit",omaSaldo);

        QString site_url=url::getBaseUrl()+"/accounts/"+id_account;
        QNetworkRequest request((site_url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        //WEBTOKEN ALKU
        request.setRawHeader(QByteArray("Authorization"),(webToken));
        request2.setRawHeader(QByteArray("Authorization"),(webToken));
        //WEBTOKEN LOPPU
        updateManager = new QNetworkAccessManager(this);
        connect(updateManager, SIGNAL(finished (QNetworkReply*)), this, SLOT(updateSlot(QNetworkReply*)));
        reply = updateManager->put(request, QJsonDocument(jsonObj).toJson());
        reply = updateManager->post(request2, QJsonDocument(jsonObjTrans).toJson());
        balance = QString::number(omaSaldo);
        QString nostettu = QString::number(nostoSumma);
        ui->kyhny_info->setText("Nostettu: "+nostettu);
        ui->kuittiButton->show();
        }
        else
        {
             ui->nosto_info->setText("Tilillä ei riittävästi luottoa.");
        }
    }
}

void nostoSummaWindow::on_kuittiButton_clicked()
{
        objectkuittiwindow = new kuittiwindow(webToken, cardnum,credit,nostoMaara);
        objectkuittiwindow->show();
        this->close();
}
void nostoSummaWindow::on_muuButton_clicked()
{
    aika = 10;
    ui->lineEdit->show();
    ui->muu_info->show();
    ui->confirmButton->show();
    ui->kumiButton->show();
    ui->nollaButton->show();
    ui->yksiButton->show();
    ui->kaksiButton->show();
    ui->kolmeButton->show();
    ui->neljaButton->show();
    ui->viisiButton->show();
    ui->kuusiButton->show();
    ui->seitsemanButton->show();
    ui->kahdeksanButton->show();
    ui->yhdeksanButton->show();
}
void nostoSummaWindow::on_confirmButton_clicked()
{
    QDateTime time= QDateTime::currentDateTime();
    QString date = time.toString("dd.MM.yyyy hh:mm:ss");
    aika = 10;
    ui->nosto_info->setText("");
    ui->jakolabel->hide();
    QString nostoluku;
    nostoluku=ui->lineEdit->text();
    ui->lineEdit->clear();
    ui->muu_info->clear();
    int y = nostoluku.toDouble();
    nostoMaara=y;
    nosto=balance.toDouble();
    if (y%10==0)
    {
        countMoney(nosto,y);
    } else
    {
        ui->jakolabel->show();
    }
    ui->pushButton20e->hide();
    ui->pushButton40e->hide();
    ui->pushButton60e->hide();
    ui->pushButton100e->hide();
    ui->pushButton200e->hide();
    ui->pushButton500e->hide();
    ui->muuButton->hide();
    ui->label->hide();
    ui->muu_info->hide();
    ui->kumiButton->hide();
    ui->nollaButton->hide();
    ui->yksiButton->hide();
    ui->kaksiButton->hide();
    ui->kolmeButton->hide();
    ui->neljaButton->hide();
    ui->viisiButton->hide();
    ui->kuusiButton->hide();
    ui->seitsemanButton->hide();
    ui->kahdeksanButton->hide();
    ui->yhdeksanButton->hide();
    ui->confirmButton->hide();
    ui->lineEdit->hide();
}
void nostoSummaWindow::close_window() {
    close();
}
void nostoSummaWindow::on_nollaButton_clicked()
{
    aika = 10;
    ui->lineEdit->setText(ui->lineEdit->text()+ "0");
}
void nostoSummaWindow::on_yksiButton_clicked()
{
    aika = 10;
    ui->lineEdit->setText(ui->lineEdit->text()+ "1");
}
void nostoSummaWindow::on_kaksiButton_clicked()
{
    aika = 10;
    ui->lineEdit->setText(ui->lineEdit->text()+ "2");
}
void nostoSummaWindow::on_kolmeButton_clicked()
{
    aika = 10;
    ui->lineEdit->setText(ui->lineEdit->text()+ "3");
}
void nostoSummaWindow::on_neljaButton_clicked()
{
    aika = 10;
    ui->lineEdit->setText(ui->lineEdit->text()+ "4");
}
void nostoSummaWindow::on_viisiButton_clicked()
{
    aika = 10;
    ui->lineEdit->setText(ui->lineEdit->text()+ "5");
}
void nostoSummaWindow::on_kuusiButton_clicked()
{
    aika = 10;
    ui->lineEdit->setText(ui->lineEdit->text()+ "6");
}
void nostoSummaWindow::on_seitsemanButton_clicked()
{
    aika = 10;
    ui->lineEdit->setText(ui->lineEdit->text()+ "7");
}
void nostoSummaWindow::on_kahdeksanButton_clicked()
{
    aika = 10;
    ui->lineEdit->setText(ui->lineEdit->text()+ "8");
}
void nostoSummaWindow::on_yhdeksanButton_clicked()
{
    aika = 10;
    ui->lineEdit->setText(ui->lineEdit->text()+ "9");
}
void nostoSummaWindow::on_kumiButton_clicked()
{
    ui->lineEdit->backspace();
}
