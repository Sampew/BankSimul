#ifndef NOSTOSUMMAWINDOW_H
#define NOSTOSUMMAWINDOW_H
#include "url.h"
#include "kuittiwindow.h"
#include <QWidget>
#include <QObject>
#include <QDialog>
#include <QNetworkAccessManager>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QMovie>
#include <QTimer>


namespace Ui {
class nostoSummaWindow;
}
class nostoSummaWindow : public QDialog
{
    Q_OBJECT
public:
    explicit nostoSummaWindow(QByteArray token, QString myCard,bool cardType, QWidget *parent = nullptr);
    ~nostoSummaWindow();
    void setWebToken(const QByteArray &newWebToken);
    int lastID;
public slots:
    void close_window();
private slots:
    void on_pushButton20e_clicked();
    void on_pushButton40e_clicked();
    void on_pushButton60e_clicked();
    void on_pushButton100e_clicked();
    void on_pushButton200e_clicked();
    void on_pushButton500e_clicked();
    void nostoSlot (QNetworkReply *reply);
    void balanceSlot (QNetworkReply *reply);
    void updateSlot (QNetworkReply *reply);
    void transactionSlot(QNetworkReply *reply);
    void on_suljeButton_clicked();
    void delay();
    void countMoney(double balance, double amount);
    void on_kuittiButton_clicked();
    void on_muuButton_clicked();
    void on_confirmButton_clicked();
    void on_nollaButton_clicked();
    void on_yksiButton_clicked();
    void on_kaksiButton_clicked();
    void on_kolmeButton_clicked();
    void on_neljaButton_clicked();
    void on_viisiButton_clicked();
    void on_kuusiButton_clicked();
    void on_seitsemanButton_clicked();
    void on_kahdeksanButton_clicked();
    void on_yhdeksanButton_clicked();
    void on_kumiButton_clicked();
private:
    Ui::nostoSummaWindow *ui;
    QByteArray webToken;
    double nosto;
    double creditMax;
    double nostoMaara;
    QString balance;
    QString usedCredit;
    QString creditLimit;
    QString debitBalance;
    QString cardnum;
    QNetworkAccessManager *nostoManager;
    QNetworkAccessManager *balanceManager;
    QNetworkAccessManager *updateManager;
    QNetworkAccessManager *transactionManager;
    QString id_account;
    QNetworkReply *reply;
    QByteArray response_data;
    kuittiwindow *objectkuittiwindow;
    int aika;
    bool credit;
};
#endif // NOSTOSUMMAWINDOW_H
