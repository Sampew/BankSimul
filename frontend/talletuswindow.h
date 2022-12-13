#ifndef TALLETUSWINDOW_H
#define TALLETUSWINDOW_H
#include "ui_bankwindow.h"
#include "url.h"
#include <QWidget>
#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>
#include "talletuskuittiwindow.h"

namespace Ui {
class talletusWindow;
}

class talletusWindow : public QDialog
{
    Q_OBJECT

public:
    explicit talletusWindow(QByteArray token,QString cardNumber,bool cardType,QWidget *parent = nullptr);
    ~talletusWindow();
    void delay();
    void talletusDelay();
    int lastID;
private slots:
    void on_talletaButton_clicked();
    void on_peruutaButton_clicked();
    void talletusSlot (QNetworkReply *reply);
    void asiakasSlot (QNetworkReply *reply);
    void saldoSlot (QNetworkReply *reply);
    void transactionSlot(QNetworkReply *reply);
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_0_clicked();
    void on_pyyhiButton_clicked();

    void on_kuittiButton_clicked();

private:
    Ui::talletusWindow *ui;
    QByteArray webToken;
    QString card_number;
    QString account;
    talletusKuittiWindow *objecttalletusKuittiWindow;
    QNetworkAccessManager *asiakasManager;
    QNetworkAccessManager *saldoManager;
    QNetworkAccessManager *talletusManager;
    QNetworkAccessManager *transactionManager;
    QNetworkReply *reply;
    QByteArray response_data;
    QString cardNum;
    QString sum;
    QString saldo;
    QString credit_limit;
    QString used_credit;
    QString description;
    QString transacSum;
    QString id_transactions;
    QString date;
    double talletusMaara;
    double talletus;
    double x;
    int aika;
    bool credit;
};

#endif // TALLETUSWINDOW_H
