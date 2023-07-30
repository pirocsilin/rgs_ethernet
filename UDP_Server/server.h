#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    static int Count;
    ~Server();

private:
    Ui::Server *ui;
    QUdpSocket *server;
    QTimer *timer;

private slots:
    void slotSendData();
    void on_STATE_clicked();
    void on_frequency_valueChanged(int arg1);
};

#endif // SERVER_H
