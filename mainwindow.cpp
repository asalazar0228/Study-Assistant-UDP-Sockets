#include "mainwindow.h"
#include "ui_mainwindow.h"

// Made the address the local host for testing purposes
#define localdir QHostAddress::LocalHost

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //give window a title
    this -> setWindowTitle("Studying Assistant/UDP Sockets");

    //bind UDP socket to a specific address and port
    udpsocket = new QUdpSocket(this);
    udpsocket-> bind(localdir, 2016);

    //connect readyRead and readSocket using signals and slots
    connect(udpsocket, SIGNAL(readyRead()),this,SLOT(readSocket()));

    ui->myPort->setText("2016");
    ui->goalEdit->setAlignment(Qt::AlignHCenter);

    //myIP is set to my local host please note that this address cannot be used by others
    //this is jst for testing purposes
    QString myaddr = QHostAddress(localdir).toString();
    ui->myIP->setText(myaddr);

    getIP();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getIP()
{
    //go to another site in order to be able to public IP to give to friend
    //this site has your IP in body
    QNetworkReply *reply;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    reply =  manager->get(QNetworkRequest(QUrl("http://whatismyip.org")));

    //connect signal and slot
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(addIP(QNetworkReply *)));
}


void MainWindow::addIP(QNetworkReply *reply)
{
    //the point of this slot function is to parse out the public IP out of the HTML body
    QString contents;
    if(reply->error() == QNetworkReply::NoError){
//        qDebug() << reply->readAll();
        contents = reply->readAll();
        QRegExp rx(".{4}:.{4}:.{4}:.4}:.{4}:.{4}:.{4}:.{4}");
        int pos = rx.indexIn(contents);
        QStringList list = rx.capturedTexts();
        QString result = list.at(0);
        qDebug() << list;
    }
    else{
        QMessageBox::information(0, "Network error", "Could not get external IP sorry :(");
    }
    reply->deleteLater();
}

//similar to other send buttons in Window... use as reference for other buttons
void MainWindow::on_wakeUp_clicked()
{
    friend_address= ui->friend_IP->text();
    portnumber= ui->friend_Port->text().toInt();

    //string being sent to friend
    QString wakeup= "Friend Pls Wake Up and Study!!!";

    udpsocket->writeDatagram(wakeup.toLatin1(),friend_address,portnumber);
}

void MainWindow::on_checkIn_clicked()
{
    friend_address= ui->friend_IP->text();
    portnumber= ui->friend_Port->text().toInt();

    udpsocket->writeDatagram("I acknowledge the message sent",friend_address,portnumber);
}

void MainWindow::readSocket()
{
    while(udpsocket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(udpsocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpsocket ->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
        ui->textBrowser->append(QString(datagram));
        ui->friend_IP->setText(sender.toString());
    }
}



void MainWindow::on_start_clicked()
{
    //this function allows you to start a timer for yourself
    //set in minutes
    timer = new QTimer(this);
    int mins_desired = ui->goalEdit->text().toInt();
    if(!(mins_desired>0))
    {
        QMessageBox warning_message;
        warning_message.critical(0,"Error","Goal time must be above zero. Thank you ^_^");
    }
    else
    {
    //convert from minutes for Qt timer
    int time_desired = (mins_desired)*1000*60;
    timer->start(time_desired);
    ui->textBrowser->append("Your timer of " + QString::number(mins_desired)+ " mins has started");
    ui->goalEdit->setDisabled(true);
    }
}

void MainWindow::on_checkTime_clicked()
{
    //this function allows you to check how much time is remaining on your timer
    int remaining_time = (timer->remainingTime())/1000;
    int remaining_mins = remaining_time/60;

    ui->textBrowser->append("You have " + QString::number(remaining_mins)+ " mins remaining");
}

void MainWindow::on_encourage_clicked()
{
    friend_address= ui->friend_IP->text();
    portnumber= ui->friend_Port->text().toInt();

    QString message= "You can do it!! You got this :)";

    udpsocket->writeDatagram(message.toLatin1(),friend_address,portnumber);

}

void MainWindow::on_ack_button_clicked()
{
    friend_address= ui->friend_IP->text();
    portnumber= ui->friend_Port->text().toInt();

    QString message= "I acknowledge your message :)";

    udpsocket->writeDatagram(message.toLatin1(),friend_address,portnumber);

}

void MainWindow::on_done_button_clicked()
{
    friend_address= ui->friend_IP->text();
    portnumber= ui->friend_Port->text().toInt();

    QString message= "I am done studying :)";

    udpsocket->writeDatagram(message.toLatin1(),friend_address,portnumber);

}
