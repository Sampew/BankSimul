#include "talletuswindow.h"
#include "ui_talletuswindow.h"
#include "bankwindow.h"

talletusWindow::talletusWindow(QByteArray token,QString cardNumber,bool cardType,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::talletusWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-image: url(:/icons/talvi.jpg)");
    this->setWindowState(Qt::WindowMaximized);
    ui->kuittiButton->hide();
    ui->summaLabel->hide();
    card_number = cardNumber;
    webToken=token;

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
    QString site_url=url::getBaseUrl()+"cards/"+card_number;
    QNetworkRequest request((site_url));
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    asiakasManager = new QNetworkAccessManager(this);
    connect(asiakasManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(asiakasSlot(QNetworkReply*)));
    reply = asiakasManager->get(request);
}

talletusWindow::~talletusWindow()
{
    delete ui;
}

void talletusWindow::delay()
{

    int afkTimer=1;
    QTime dieTime= QTime::currentTime().addSecs(afkTimer);
     while (QTime::currentTime() < dieTime)
         QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void talletusWindow::talletusDelay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void talletusWindow::asiakasSlot(QNetworkReply *reply)
{
    aika = 10;
    QByteArray response_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    account = QString::number(json_obj["id_account"].toInt());
    QString omistaja=json_obj["card_owner"].toString();
    QString number = QString::number(json_obj["card_number"].toInt());

    ui->labelOmistaja->setText("Kortin omistaja: " +omistaja + "\nKortin Numero: " + number);
    QString site_url=url::getBaseUrl()+"accounts/"+account;
    QNetworkRequest request((site_url));
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    saldoManager = new QNetworkAccessManager(this);
    connect(saldoManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(saldoSlot(QNetworkReply*)));
    reply = saldoManager->get(request);

    QString site_url2=url::getBaseUrl()+"transactions/";
    qDebug()<<site_url2;
    QNetworkRequest request2((site_url2));
    //WEBTOKEN ALKU
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    //WEBTOKEN LOPPU
    transactionManager = new QNetworkAccessManager(this);
    connect(transactionManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(transactionSlot(QNetworkReply*)));
    reply = transactionManager->get(request2);


}
void talletusWindow::transactionSlot(QNetworkReply *reply){

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

void talletusWindow::saldoSlot(QNetworkReply *reply)
{
    QByteArray response_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();

    saldo=QString::number(json_obj["debit_balance"].toDouble());
    credit_limit=QString::number(json_obj["credit_limit"].toDouble());
    used_credit=QString::number(json_obj["used_credit"].toDouble());
    x = saldo.toDouble();
    ui->talletusLabel->setText("Syötä talletettava määrä");


    for (aika = 10; aika >= 0; aika--) {
        delay();
        ui->timer->display(aika);

        if (aika == 0&& this->isHidden()==false) {
            bankwindow *main = new bankwindow(webToken,card_number,credit);
            main->show();
            close();
        }
    }


}

void talletusWindow::on_talletaButton_clicked()
{
    aika = 10;
    ui->talletaButton->hide();
    ui->pushButton_0->hide();
    ui->pushButton_1->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->pushButton_6->hide();
    ui->pushButton_7->hide();
    ui->pushButton_8->hide();
    ui->pushButton_9->hide();
    ui->pyyhiButton->hide();
    ui->lineEditMaara->hide();
    ui->talletusLabel->hide();
    ui->kuittiButton->show();
    ui->summaLabel->show();

    sum=ui->lineEditMaara->text();
    ui->lineEditMaara->clear();
    talletusMaara = sum.toDouble();
    double y = sum.toDouble();
    talletus=x + y;
    if(y<=0){
            qDebug()<<"summa on 0";
            ui->summaLabel->setText("Ei mahdollinen");
            ui->saldoLabel->setText("Määrän oltava yli 0");
            ui->kuittiButton->hide();
    }
    else if(y>=0){
        QDateTime time= QDateTime::currentDateTime();
        QString date = time.toString("dd.MM.yyyy hh:mm:ss");

        QJsonObject jsonObjTrans;
        jsonObjTrans.insert("id_transactions",lastID);
        jsonObjTrans.insert("card_number",card_number);

        jsonObjTrans.insert("date",date);
        jsonObjTrans.insert("description","talletus");
        QString site_url2=url::getBaseUrl()+"/transactions/";
        QNetworkRequest request2((site_url2));

    aika = 10;

    QJsonObject jsonObj;
    jsonObj.insert("debit_balance",talletus);
    jsonObj.insert("credit_limit",credit_limit);
    jsonObj.insert("used_credit",used_credit);
    QString uusiSaldo = QString::number(talletus);
    ui->saldoLabel->setText("Uusi Saldo: " + uusiSaldo);
    ui->summaLabel->setText("Talletettu summa: " + sum);
    jsonObjTrans.insert("sum",y);
    QString site_url=url::getBaseUrl()+"/accounts/"+account;

    QNetworkRequest request((site_url));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    request2.setRawHeader(QByteArray("Authorization"),(webToken));

    talletusManager = new QNetworkAccessManager(this);
    connect(talletusManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(talletusSlot(QNetworkReply*)));
    reply = talletusManager->put(request, QJsonDocument(jsonObj).toJson());
    reply = talletusManager->post(request2, QJsonDocument(jsonObjTrans).toJson());

    }
    else{
        close();
    }

}

void talletusWindow::talletusSlot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    reply->deleteLater();
    talletusManager->deleteLater();

}

void talletusWindow::on_peruutaButton_clicked()
{
    bankwindow *bank = new bankwindow(webToken, card_number,credit);
    bank->show();
    close();
}

void talletusWindow::on_pushButton_1_clicked()
{
    aika = 10;
    ui->lineEditMaara->setText(ui->lineEditMaara->text()+ "1");
}

void talletusWindow::on_pushButton_2_clicked()
{
    aika = 10;
    ui->lineEditMaara->setText(ui->lineEditMaara->text()+ "2");
}


void talletusWindow::on_pushButton_3_clicked()
{
    aika = 10;
    ui->lineEditMaara->setText(ui->lineEditMaara->text()+ "3");
}


void talletusWindow::on_pushButton_4_clicked()
{
    aika = 10;
    ui->lineEditMaara->setText(ui->lineEditMaara->text()+ "4");
}


void talletusWindow::on_pushButton_5_clicked()
{
    aika = 10;
    ui->lineEditMaara->setText(ui->lineEditMaara->text()+ "5");
}


void talletusWindow::on_pushButton_6_clicked()
{
    aika = 10;
    ui->lineEditMaara->setText(ui->lineEditMaara->text()+ "6");
}


void talletusWindow::on_pushButton_7_clicked()
{
    aika = 10;
    ui->lineEditMaara->setText(ui->lineEditMaara->text()+ "7");
}


void talletusWindow::on_pushButton_8_clicked()
{
    aika = 10;
    ui->lineEditMaara->setText(ui->lineEditMaara->text()+ "8");
}


void talletusWindow::on_pushButton_9_clicked()
{
    aika = 10;
    ui->lineEditMaara->setText(ui->lineEditMaara->text()+ "9");
}


void talletusWindow::on_pushButton_0_clicked()
{
    aika = 10;
    ui->lineEditMaara->setText(ui->lineEditMaara->text()+ "0");
}


void talletusWindow::on_pyyhiButton_clicked()
{
    ui->lineEditMaara->backspace();
}

void talletusWindow::on_kuittiButton_clicked()
{
    objecttalletusKuittiWindow = new talletusKuittiWindow(webToken, card_number,credit,talletusMaara);
    objecttalletusKuittiWindow->show();
    this->close();
}
