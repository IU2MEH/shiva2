#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QSettings>
#include <QCoreApplication>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QDir>


extern QStringList marker;
extern QStringList spot;
extern QStringList priyom;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
private:
    QTcpServer *tcpServer = nullptr;
    int check_user();

private slots:
    void NewConn();
    void senddata();
};

#endif // SERVER_H
