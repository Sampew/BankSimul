#ifndef TAPAHTUMAWINDOW_H
#define TAPAHTUMAWINDOW_H
#include <QtNetwork>
#include <QWidget>
#include <QTableWidget>
#include <QDialog>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonDocument>
#include "url.h"

namespace Ui {
class tapahtumaWindow;
}

class tapahtumaWindow : public QDialog
{
    Q_OBJECT
public:
    explicit tapahtumaWindow(QByteArray token,QString myCard, bool cardType, QWidget *parent = nullptr);
    ~tapahtumaWindow();
    void setWebToken(const QByteArray &newWebToken);
    void delay();
private slots:
    void on_closeButton_clicked();
    void tapahtumaSlot(QNetworkReply *reply);
    void asiakasSlot(QNetworkReply *reply);
    void on_backwardButton_clicked();
    void on_forwardButton_clicked();
private:
    QTableWidget* m_pTableWidget;
    QStringList m_TableHeader;
    Ui::tapahtumaWindow *ui;
    QString card_number;
    QByteArray webToken;
    QNetworkAccessManager *tapahtumaManager;
    QNetworkAccessManager *asiakasManager;
    QByteArray response_data;
    QNetworkReply *reply;
    QString lista;
    int totalRows=0;
    int rowsOverTen=0;
    int lastVisibleRowNumber=0;
    int numbersAboveTen=0;
    int resetCounter=0;
    int minimumRows=0;
    int lastIncrement;
    int aika;
    bool credit;
};
#endif // TAPAHTUMAWINDOW_H
