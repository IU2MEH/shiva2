#include "server.h"


QList<QTcpSocket *> myclients;

void Server::senddata()
{
    QSettings settings;
    for( int j=0;j<myclients.size();j++)
    {
        for (int i=0;i<marker.size();i++)
        {
            myclients.at(j)->write(marker.at(i).toLocal8Bit());
            myclients.at(j)->flush();
        }
        for (int k=0;k<spot.size();k++)
        {
            myclients.at(j)->write(spot.at(k).toLocal8Bit().right(spot.at(k).size()-25));
            myclients.at(j)->flush();
        }
        for (int h=0;h<priyom.size();h++)
        {
            QDateTime dt=QDateTime::fromString(priyom.at(h).toLocal8Bit().left(24),"ddd MMM dd hh:mm:ss yyyy");
            dt.setTimeSpec(Qt::UTC);
            //qDebug()<<"datetime:"<<priyom.at(h).toLocal8Bit().left(24);
            // send only number stations that will start to tx 10 mins later from now
            if (QDateTime::currentDateTimeUtc().secsTo(dt)<10*60)
            {
                QString data;
                data.clear();
                data=priyom.at(h).toLocal8Bit().right(priyom.at(h).size()-25)+QDateTime::currentDateTimeUtc().time().toString("hhmm")+"Z\r\n";
                myclients.at(j)->write(data.toLocal8Bit());
                myclients.at(j)->flush();
            }
        }
    }
}


void Server::NewConn()
{
    QSettings settings;
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    myclients << clientConnection;
    if (myclients.size()>500)
    {
        qInfo()<<"Max client limit of 500 reached!";
        clientConnection->write("\r\n\r\n\r\nMax client limit of 500 reached!\r\n\r\n\r\n");
        clientConnection->close();
        return;
    }
    connect(clientConnection, &QAbstractSocket::disconnected,clientConnection, &QObject::deleteLater);
    connect(clientConnection, &QTcpSocket::disconnected, this, [this, clientConnection]() {myclients.removeAll(clientConnection);}, Qt::QueuedConnection);
    qDebug()<<"New client: "<<clientConnection;
    qDebug()<<"New connection from:"<<clientConnection->peerAddress().toString()<<":"<<clientConnection->peerPort();
    // checking user ////////////////////////////////////
    QString user;
    QString password;
    int found=0;
    QString data;
    clientConnection->write("Please enter your callsign: ");
    if (clientConnection->waitForReadyRead(10000))
    {
            data = QString(clientConnection->readLine());
            if (!data.front().isLetter()) //removing telnet negoziation
                if (clientConnection->waitForReadyRead(10000)) //removing telnet negoziation
                    data = QString(clientConnection->readLine()); //removing telnet negoziation
    }
    else
    {
        clientConnection->close();
        return;
    }
    user=data.trimmed().toUpper();
    qInfo()<<"User:"<<user<<"is logging in";
    clientConnection->write("Please enter password: ");
    if (clientConnection->waitForReadyRead(10000))
        {
            data = QString(clientConnection->readLine());
        }
    password=data.trimmed();
    qDebug()<<"with password"<<password;

    QDir dir;
    QString path=settings.value("wrkdir").toString();
    dir.setPath(path);
    QFile file(dir.path()+"user.conf");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
       {
        qInfo()<<"Cant read file "<<dir.path()<<"user.conf";
        clientConnection->close();
        return;
       }
    else
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (!line.startsWith("#"))
            {
                QStringList line_splitted=line.split(',');
                if ( line_splitted.count() == 2 )
                {
                    QString users=line_splitted.at(0).toUpper();
                    QString passwords=line_splitted.at(1);
                    if ( user == users && password == passwords )
                        {
                            found=1;
                            qInfo()<<user<<" logged in from:"<<clientConnection->peerAddress().toString()<<":"<<clientConnection->peerPort();
                            clientConnection->write("\r\n\r\nWelcome to sh1va2 dx cluster\r\n");
                            clientConnection->write("This software was written by IU2MEH\r\n");
                            clientConnection->write("73 \r\n\r\n");
                            file.close();
                            // keep this synced with senddata()
                            for (int i=0;i<marker.size();i++)
                            {
                               clientConnection->write(marker.at(i).toLocal8Bit());
                               clientConnection->flush();
                            }
                            // ////////////////////////////////////////////////////////////////////
                        }
                }
            }
        }
    }
    if (found==0)
    {
        qInfo()<<"Invalid login:"<<user<<"with password"<<password;
        file.close();
        clientConnection->close();
        return;
    }
}

Server::Server()
{
    QSettings settings;
    tcpServer = new QTcpServer(this);
    QTimer *timer_senddata = new QTimer(this);
    if (!tcpServer->listen(QHostAddress::Any,settings.value("tcpport").toInt()))
    {
        qInfo()<<"Can not start server";
        return;
    }
    connect(timer_senddata, SIGNAL(timeout()),this, SLOT(senddata()));
    timer_senddata->start(settings.value("interval").toInt());
    qInfo()<<"Listening on port"<<settings.value("tcpport").toInt();
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::NewConn);
}
