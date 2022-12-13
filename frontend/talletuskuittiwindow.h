#ifndef TALLETUSKUITTIWINDOW_H
#define TALLETUSKUITTIWINDOW_H
#include <QtNetwork>
#include <QWidget>
#include <QTableWidget>
#include <QDialog>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonDocument>
#include "url.h"
#include <QWidget>

namespace Ui {
class talletusKuittiWindow;
}

class talletusKuittiWindow : public QDialog
{
    Q_OBJECT

public:
    explicit talletusKuittiWindow(QByteArray token,QString cardnum,bool cardType,double talletusMaara,QWidget *parent = nullptr);
    ~talletusKuittiWindow();
    void setWebToken(const QByteArray &newWebToken);
    void delay();
public slots:
    void close_window();
private slots:
    void kuittiSlot(QNetworkReply *reply);
    void asiakasSlot(QNetworkReply *reply);
    void on_pushButton_clicked();
private:
    Ui::talletusKuittiWindow *ui;
    QString card_number;
    QString talletus;
    QByteArray webToken;
    QNetworkAccessManager *kuittiManager;
    QNetworkAccessManager *asiakasManager;
    QByteArray response_data;
    QNetworkReply *reply;
    QString lista;
    bool credit;
    int aika;
    QString account;
    QString cardNum;
    QString card_owner;
    QString debitBalance;
    QString usedCredit;
    QString creditLimit;
};

#endif // TALLETUSKUITTIWINDOW_H
