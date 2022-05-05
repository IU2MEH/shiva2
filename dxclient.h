#ifndef DXCLIENT_H
#define DXCLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QSettings>
#include <QDateTime>
#include <QTimer>


extern QStringList spot;


class DXClient : public QObject
{
    Q_OBJECT
public:
    explicit DXClient(QObject *parent = nullptr);
private:
    void connector();

private slots:
    void connected();
    void disconnected();
    void readyRead();
    void reconnect();

};

#endif // DXCLIENT_H
