#include "server.h"
#include "./ui_server.h"

Server::Server(QWidget *parent):QMainWindow(parent), ui(new Ui::Server){

    ui->setupUi(this);
    server = new QUdpSocket(this);
    timer = new QTimer(this);
    timer->setInterval(1000);

    ui->IP_addrass->setText("127.0.0.1");
    ui->PORT->setText("2424");
    ui->statusbar->showMessage("Press START for send frames");
    ui->statusbar->setStyleSheet("background-color: rgb(251, 232, 123);");

    connect(timer, &QTimer::timeout, this, &Server::slotSendData);
}
int Server::Count{};

void Server::slotSendData(){
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << QDateTime::currentDateTime();

    QHostAddress addr;
    if(server->writeDatagram(data, QHostAddress::LocalHost, 2424) != -1){

        ui->statusbar->setStyleSheet("background-color: rgb(0, 255, 0);");
        ui->statusbar->showMessage("Отправлено пакетов: " +
                                   QString::number(++Count));
    }
    else{
        timer->stop();
        ui->STATE->setText("START");
        ui->statusbar->showMessage("ERROR CONNECT!");
        ui->statusbar->setStyleSheet("background-color: rgb(255, 0, 0);");
    }
}

void Server::on_STATE_clicked(){
    if(timer->isActive()){
        timer->stop();
        ui->STATE->setText("START");
        ui->statusbar->showMessage("Press START for send frames");
        ui->statusbar->setStyleSheet("background-color: rgb(251, 232, 123);");
    }
    else{
        timer->start();
        ui->STATE->setText("STOP");
    }
}

void Server::on_frequency_valueChanged(int arg1){
    timer->setInterval(arg1 * 1000);
}

Server::~Server(){
    delete ui;
}
