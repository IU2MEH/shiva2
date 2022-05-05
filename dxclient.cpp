#include "dxclient.h"

QTcpSocket *dxclient;
QTimer *dxclustertimeout;

void DXClient::connected()
{
    QSettings settings;
    QString callsign=settings.value("callsign").toString();
    dxclient->write((callsign+"\r\n").toStdString().c_str());
    qInfo()<<"Logged in the DXCluster"<<settings.value("dxcluster").toString()<<" as: "<<callsign;
}

void DXClient::disconnected()
{
    qDebug()<<"Disconnected from the DXCluster";
}

void DXClient::readyRead()
{
    QString line;
    line.clear();
    line=dxclient->readAll();
    if(line.startsWith("DX de "))
    {
        if (line.contains("DX de "))
        {
            QStringList lines;
            lines.clear();
            lines=line.split("DX de ");
            for (int i=1 ;i<lines.size();i++)
            {
                spot.append(QDateTime::currentDateTimeUtc().toString("ddd MMM dd hh:mm:ss yyyy")+"|DX de "+lines.at(i));
            }
        }
        else
        {
            qInfo()<<"DXcluster client line parsing error, please check!";
        }
        dxclustertimeout->start(30000);
    }
}

void DXClient::connector()
{
    QSettings settings;
    QStringList url;
    dxclient=new QTcpSocket(this);
    connect(dxclient,SIGNAL(connected()),this,SLOT(connected()));
    connect(dxclient,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(dxclient,SIGNAL(readyRead()),this,SLOT(readyRead()));
    url=settings.value("dxcluster").toString().split(":");
    dxclient->connectToHost(url.at(0),url.at(1).toInt());
    if (!dxclient->waitForConnected(settings.value("timeout").toInt()))
        qInfo()<<"Error connecting to DXCluster: "<<url.at(0)<<":"<<url.at(1);
    else
        qDebug()<<"Connected to DXCluster: "<<url.at(0)<<":"<<url.at(1);
}

void DXClient::reconnect()
{
    qInfo()<<"I did not receive any data from DXCluster for a while";
    qInfo()<<"Reconnecting...";
    connector();
}


DXClient::DXClient(QObject *parent) : QObject(parent)
{
    connector();
    dxclustertimeout=new QTimer(this);
    dxclustertimeout->start(30000);
    connect(dxclustertimeout,SIGNAL(timeout()),this,SLOT(reconnect()));
}
