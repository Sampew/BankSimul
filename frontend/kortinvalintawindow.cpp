#include "kortinvalintawindow.h"
#include "ui_kortinvalintawindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QCoreApplication>
#include <QHeaderView>
#include <QMessageBox>
kortinValintaWindow::kortinValintaWindow(QByteArray token,QString cardNum,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kortinValintaWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-image: url(:/icons/talvi.jpg)");
    this->setWindowState(Qt::WindowMaximized);
    qDebug()<<"kortinvalinta konstruktori";
    cardNumber=cardNum;
    webToken=token;
    QString site_url=url::getBaseUrl()+"cards/"+cardNumber;
    QNetworkRequest request((site_url));
    request.setRawHeader(QByteArray("Authorization"),(webToken));
    dataManager = new QNetworkAccessManager(this);
    connect(dataManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(dataSlot(QNetworkReply*)));
    reply = dataManager->get(request);

}

kortinValintaWindow::~kortinValintaWindow()
{
    delete ui;
    delete objectBankWindow;
    objectBankWindow=nullptr;
}

void kortinValintaWindow::setWebToken(const QByteArray &newWebToken)
{
    webToken = newWebToken;
}

void kortinValintaWindow::dataSlot(QNetworkReply *reply)
{
    QByteArray response_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();
    QString kortti = json_obj["debit_credit"].toString();

    if (kortti == "debit") {
        credit=false;
        objectBankWindow=new bankwindow(webToken,cardNumber,credit);
        objectBankWindow->setWebToken("Bearer "+response_data);
        objectBankWindow->show();
        this->close();
        }
    reply->deleteLater();
    dataManager->deleteLater();
}

void kortinValintaWindow::on_creditButton_clicked()
{
    credit=true;
    objectBankWindow=new bankwindow(webToken,cardNumber,credit);
    objectBankWindow->setWebToken("Bearer "+response_data);
    objectBankWindow->show();
    this->close();
}

void kortinValintaWindow::on_debitButton_clicked()
{
    credit=false;
    objectBankWindow=new bankwindow(webToken,cardNumber,credit);
    objectBankWindow->setWebToken("Bearer "+response_data);
    objectBankWindow->show();
    this->close();
}
