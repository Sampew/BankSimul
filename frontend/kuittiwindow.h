#ifndef KUITTIWINDOW_H
#define KUITTIWINDOW_H
#include <QtNetwork>
#include <QWidget>
#include <QTableWidget>
#include <QDialog>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonDocument>
#include "url.h"

namespace Ui{
class kuittiwindow;
}

class kuittiwindow : public QDialog
{
    Q_OBJECT
public:
    explicit kuittiwindow(QByteArray token,QString cardnum,bool cardType,double nostoMaara,QWidget *parent = nullptr);
    ~kuittiwindow();
    void setWebToken(const QByteArray &newWebToken);
    void delay();
public slots:
    void close_window();
private slots:
    void kuittiSlot(QNetworkReply *reply);
    void asiakasSlot(QNetworkReply *reply);
    void on_pushButton_clicked();
private:
    QTableWidget* m_pTableWidget;
    QStringList m_TableHeader;
    Ui::kuittiwindow *ui;
    QString card_number;
    QString nosto;
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
#endif // KUITTIWINDOW_H
