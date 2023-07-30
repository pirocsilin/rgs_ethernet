#include "client.h"
#include "./ui_client.h"

Client::Client(QWidget *parent):QMainWindow(parent), ui(new Ui::Client){
    ui->setupUi(this);
    //quint16 port = 2424;
    //QHostAddress host("127.0.0.1");

    client = new QUdpSocket(this);
    client->bind(2424);
    ui->statusbar->setStyleSheet("background-color: rgb(249, 147, 80);");
    ui->statusbar->showMessage("Listening port: 2424");
    //                           + QString::number(port) + " on LocalHost");
    connect(client, &QUdpSocket::readyRead, this, &Client::slotReadDatagrams);
}

void Client::slotReadDatagrams(){
    QByteArray data;
    do{
        data.resize(client->pendingDatagramSize());
        client->readDatagram(data.data(), data.size());
    }while(client->hasPendingDatagrams());

    QDateTime dt;
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);
    in >> dt;
    ui->textBrowser->append(dt.toString());
    qDebug() << "new frame";
}

Client::~Client(){
    delete ui;
}

