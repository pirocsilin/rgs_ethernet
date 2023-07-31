#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>
#include <QDateTime>
#include <QThread>
#include <QDir>
#include <fstream>
#include <iostream>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Server; class CorrectData; }
QT_END_NAMESPACE

class CorrectData;

class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    static void PreparetionData();
    static void printFrames(int);
    static int Count;
    ~Server();

private:
    Ui::Server *ui;
    QUdpSocket *server;
    QTimer *timer;
    CorrectData *mythread;
    int ptr{};
    static QVector<QByteArray> Frames;

private slots:
    void slotSendData();
    void on_STATE_clicked();
    void on_frequency_valueChanged(int arg1);
    void slotDataIsReady();
    void on_oneFrame_clicked();
};

class CorrectData : public QThread{
    Q_OBJECT
public:
    CorrectData(QObject *parent = nullptr) : QThread(parent) {}
protected:
    void run() override;
};

#endif // SERVER_H
