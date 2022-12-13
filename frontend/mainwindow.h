#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include "bankwindow.h"
#include <QDebug>
#include "url.h"
#include "kortinvalintawindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void delay();
public slots:
    void showWindow();
private slots:
    void loginSlot (QNetworkReply *reply);
    void on_loginButton_clicked();
    void on_pushButton_1_clicked();
    void on_peruutaButton_clicked();
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
private:
    Ui::MainWindow *ui;
    bankwindow *objectBankWindow;
    kortinValintaWindow *objectKortinValinta;
    QNetworkAccessManager *loginManager;
    QNetworkReply *reply;
    QByteArray response_data;
    QString cardNum;
    QString cardPin;
    QByteArray token;
    int loginTries = 3;
    int kirjautuminen=1;
    QString idAccount;
};
#endif // MAINWINDOW_H
