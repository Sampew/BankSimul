#include "bankwindow.h"
#include "mainwindow.h"
#include "ui_bankwindow.h"
#include "url.h"
#include "nostosummawindow.h"

MainWindow *objectmainWindow;


bankwindow::bankwindow(QByteArray webToken,QString cardNumber,bool credit,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bankwindow)

{
    ui->setupUi(this);

    this->setStyleSheet("background-image: url(:/icons/talvi.jpg)");
    this->setWindowState(Qt::WindowMaximized);
    qDebug()<<"bankwindow konstruktori";

    objectmainWindow = new MainWindow;

    connect(ui->kirjauduUlosButton,&QPushButton::clicked,objectmainWindow,&MainWindow::showWindow);
    connect(ui->kirjauduUlosButton,&QPushButton::clicked,this,&bankwindow::closeWindow);
    ui->labelLocked->hide();
    ui->timer->setPalette(Qt::red);
    ui->timer->setAutoFillBackground(true);
    QPalette Pal = ui->timer->palette();
    Pal.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
    Pal.setColor(QPalette::Normal, QPalette::Window, Qt::black);
    ui->timer->setPalette(Pal);

    qDebug()<<"kortti on"<<credit;
    if(credit==true)
    {
        cardType=true;
        ui->talletusButton->hide();
    }
    else
    {
        cardType=false;
    }
    qDebug()<<"cardtype"<<cardType;
    myCard=cardNumber;
    token = webToken;

    QString site_url=url::getBaseUrl()+"cards/"+myCard;
    QNetworkRequest request((site_url));
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    dataManager = new QNetworkAccessManager(this);
    connect(dataManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(dataSlot(QNetworkReply*)));
    reply = dataManager->get(request);
}
bankwindow::~bankwindow()
{
    qDebug() << "bankwindow destruktori";
    delete ui;
    delete objectnostoSummaWindow;
    objectnostoSummaWindow=nullptr;
    delete objectsaldoWindow;
    objectsaldoWindow=nullptr;
    delete objecttapahtumaWindow;
    objecttapahtumaWindow=nullptr;
    delete objecttalletusWindow;
    objecttalletusWindow =nullptr;

}

void bankwindow::setWebToken(const QByteArray &newWebToken)
{
    token = newWebToken;

}

void bankwindow::on_saldoButton_clicked()
{
    objectsaldoWindow = new saldoWindow(token,myCard,cardType);
    objectsaldoWindow->show();
    closeWindow();
}

void bankwindow::on_tapahtumaButton_clicked()
{
    objecttapahtumaWindow = new tapahtumaWindow(token,myCard,cardType);
    objecttapahtumaWindow->show();
    closeWindow();
}

void bankwindow::dataSlot(QNetworkReply *reply)
{

    QByteArray response_data=reply->readAll();
    qDebug()<<"response data in dataslot, bankwindow.cpp: " << response_data;
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    QString omistaja = json_obj["card_owner"].toString();
    QString cardType = json_obj["debit_credit"].toString();

    ui->labelOmistaja->setText(omistaja);
    qDebug()<<"omistaja: " <<omistaja;
    qDebug()<<"kortti: " <<cardType;
    if (omistaja == "LOCKED") {
        ui->labelOmistaja->hide();
        //ui->labelAccount->hide();
        //ui->labelCredit->hide();
        ui->timer->hide();
        ui->talletusButton->hide();
        ui->kirjauduUlosButton->hide();
        ui->nostoButton->hide();
        ui->saldoButton->hide();
        ui->tapahtumaButton->hide();
        ui->labelLocked->show();
        for (int i = 3; i >= 0;i--) {
            QString info = "KORTTI LUKITTU\nOhjelma suljetaan "+ QString::number(i) +" kuluttua...";
            ui->labelLocked->document()->setPlainText(info);
            delay();
        }
        objectmainWindow->showWindow();
        closeWindow();

    }

    for (aika = 30; aika >= 0; aika--) {
         delay();
         ui->timer->display(aika);
         if (aika == 0 && this->isHidden()==false) {
            objectmainWindow->show();
            closeWindow();
         }
    }
    reply->deleteLater();
    dataManager->deleteLater();
}

void bankwindow::on_nostoButton_clicked()
{
    objectnostoSummaWindow =new nostoSummaWindow(token, myCard,cardType);
    objectnostoSummaWindow->show();
    closeWindow();
}


void bankwindow::closeWindow() {
    close();
}

void bankwindow::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


}

void bankwindow::on_talletusButton_clicked()
{
    objecttalletusWindow =new talletusWindow(token, myCard, cardType);
    objecttalletusWindow->show();
    closeWindow();
}
