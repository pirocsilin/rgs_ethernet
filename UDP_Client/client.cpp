#include "client.h"
#include "./ui_client.h"

Client::Client(QWidget *parent):QMainWindow(parent), ui(new Ui::Client){
    ui->setupUi(this);

    client = new QUdpSocket(this);
    client->bind(2424);
    ui->statusbar->setStyleSheet("background-color: rgb(249, 147, 80);");
    ui->statusbar->showMessage("Listening port: 2424");

    connect(client, &QUdpSocket::readyRead, this, &Client::slotReadDatagrams);
}

void Client::slotReadDatagrams(){
    QByteArray data;
    do{
        data.resize(client->pendingDatagramSize());
        client->readDatagram(data.data(), data.size());
    }while(client->hasPendingDatagrams());

    ui->textBrowser->clear();
    QString str("");
    for(int i=0; i<data.size(); i++){
        if(i % 8 == 0){
            ui->textBrowser->append(str);
            str = "";
        }
        str += QString::number(static_cast<u_char>(data[i])) + "\t";
        //str += QString::number(data[i]) + "\t";
    }
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::Start);
    ui->textBrowser->setTextCursor(cursor);
    qDebug() << "new frame";
}

Client::~Client(){
    delete ui;
}
