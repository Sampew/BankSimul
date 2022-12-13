#ifndef KORTINVALINTAWINDOW_H
#define KORTINVALINTAWINDOW_H
#include <QWidget>
#include "bankwindow.h"

namespace Ui {
class kortinValintaWindow;
}

class kortinValintaWindow : public QDialog
{
    Q_OBJECT
public:
    explicit kortinValintaWindow(QByteArray token,QString cardNum,QWidget *parent= nullptr);
    ~kortinValintaWindow();
    const QString &getWebToken () const;
    void setWebToken(const QByteArray &newWebToken);
private slots:
    void on_creditButton_clicked();
    void on_debitButton_clicked();
    void dataSlot (QNetworkReply *reply);

private:
    Ui::kortinValintaWindow *ui;
    bankwindow *objectBankWindow;
    QByteArray webToken;
    QString cardNumber;
    QNetworkReply *reply;
    QByteArray response_data;
    QNetworkAccessManager *dataManager;
    bool credit;
};
#endif // KORTINVALINTAWINDOW_H
