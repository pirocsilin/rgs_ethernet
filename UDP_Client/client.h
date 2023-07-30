#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private:
    Ui::Client *ui;
    QUdpSocket *client;

private slots:
    void slotReadDatagrams();
};
#endif // CLIENT_H
