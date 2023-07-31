#include "server.h"
#include "./ui_server.h"

Server::Server(QWidget *parent):QMainWindow(parent), ui(new Ui::Server){

    ui->setupUi(this);
    server = new QUdpSocket(this);
    timer = new QTimer(this);
    mythread = new CorrectData(this);
    timer->setInterval(1000);

    ui->IP_addrass->setText("127.0.0.1");
    ui->PORT->setText("2424");

    connect(timer, &QTimer::timeout, this, &Server::slotSendData);
    connect(mythread, &CorrectData::finished, this, &Server::slotDataIsReady);

    mythread->start();
}

void Server::slotDataIsReady(){
    if(Frames.size() > 0){
        qDebug() << "Finished";
        ui->prep_block->deleteLater();
        ui->statusbar->showMessage("Press START for send frames");
        ui->statusbar->setStyleSheet("background-color: rgb(251, 232, 123);");
    }
    else{
        ui->prep_block->setText("File read error");
    }
}

void CorrectData::run(){
    qDebug() << "run is OK";
    Server::PreparetionData();
}

int Server::Count{};
QVector<QByteArray> Server::Frames{};

void Server::slotSendData(){
    QHostAddress addr = QHostAddress(ui->IP_addrass->text());
    qint16 port = ui->PORT->text().toShort();
    int countByte = server->writeDatagram(Frames[ptr], Frames[ptr].size(), addr, port);
    if(countByte != -1){

        ui->statusbar->setStyleSheet("background-color: rgb(0, 255, 0);");
        ui->statusbar->showMessage("Frame: " + QString::number(ptr+1) + ", " + QString::number(countByte) + " byte"
                                   " | " + addr.toString() + ", port: " + QString::number(port));
        ptr = (ptr + 1) % 1200;
    }
    else{
        timer->stop();
        ui->STATE->setText("START");
        ui->statusbar->showMessage("ERROR CONNECT!");
        ui->statusbar->setStyleSheet("background-color: rgb(255, 0, 0);");
    }
}

void Server::on_oneFrame_clicked(){ slotSendData(); }

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

void Server::printFrames(int numFrame){
    int count{};
    for(auto val : Frames[numFrame]){
        if(count++ % 8 == 0) cout << endl;
        printf("\t0x%x", static_cast<u_char>(val));
    }
    cout << "\n Size frame: " << Frames[numFrame].size() << endl;
}

void Server::PreparetionData(){

    #define COUNT_FRAME 1200      // Количество кадров
    #define COUNT_ELEM  1338-42   // Количество значимых символов в кадре

    QString fPath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("rgs_ethernet_log.c");

    int  _pass = 42;    // Количество пропускаемых символов [Total = COUNT_ELEM + _pass]
    char _read [2],
         _mask [2] = {'0', 'x'};

    ifstream in(fPath.toStdString(), ios::binary);
    if(in.is_open()){
        qDebug() << "File is open";

        while(Frames.size() < COUNT_FRAME){

            QByteArray q_frame;

            while(q_frame.size() < COUNT_ELEM){
                in.read((char*)&_read, 2);

                if(*((short*)_read) == *((short*)_mask)){

                    in.read((char*)&_read, 2);

                    if(_pass) { _pass--; continue; }

                    u_char num = static_cast<u_char>(stoi(_read, nullptr, 16));

                    q_frame.append(num);
                }
                in.seekg(-1, ios::cur);
                //cin.get();
            }
            Frames.push_back(q_frame);
            _pass = 42;
        }
        printFrames(1199);  // Выводим один фрейм для проверки
    }
    else{
        qDebug() << "ERROR open file";
    }
}

Server::~Server(){
    delete ui;
}
