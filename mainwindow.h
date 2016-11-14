#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QUdpSocket>
#include <QAbstractSocket>
#include <QTimer>
#include <QtGlobal>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QRegExp>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void getIP();

public slots:
    void readSocket();

private slots:
    void on_wakeUp_clicked();
    void on_checkIn_clicked();
    void on_start_clicked();
    void on_checkTime_clicked();
    void addIP(QNetworkReply *reply);
    void on_encourage_clicked();
    void on_ack_button_clicked();
    void on_done_button_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpsocket;
    QHostAddress friend_address;
    quint16 portnumber;
    QTimer *timer;
};

#endif // MAINWINDOW_H
