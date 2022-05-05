#include "priyom.h"



void Priyom::getdata()
{
    QNetworkAccessManager *networkManager;
    networkManager = new QNetworkAccessManager(this);
    // download number stations scheduled 1 hour before and 1 day later
    QString urlStr="https://calendar2.priyom.org/events?timeMin="+QDateTime::currentDateTimeUtc().addSecs(-1*60*60).toString("yyyy-MM-dd'%20'hh:mm:ss'.000'")+"&timeMax="+QDateTime::currentDateTimeUtc().addDays(1).toString("yyyy-MM-dd'%20'hh:mm:ss'.000'");
    qInfo()<<"Priyom url:"<<urlStr;
    QUrl url(urlStr);
    QNetworkRequest request;

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config);

    request.setUrl(url);
    connect (networkManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(done(QNetworkReply*)));
    networkManager->get(request);
}

void Priyom::done(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
       qDebug()<<"Priyom calendar download ok";
       QString data = QString(reply->readAll ());
       QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
       QJsonObject object = jsonDocument.object();
       QJsonArray items = object.value("items").toArray();
       for(int j=0;j<items.size();j++)
       {
            QJsonValue sm = items.at(j).toObject().value("summary");
            QJsonObject dt = items.at(j).toObject().value("start").toObject();
            QJsonValue sd = dt.value("dateTime");
            QDateTime startdate= QDateTime::fromString(sd.toString(),"yyyy-MM-dd'T'hh:mm:ss'.000Z'");
            startdate.setTimeSpec(Qt::UTC);
            QString startdatestring=startdate.toString("ddd MMM dd hh:mm:ss yyyy");
            QString smstr=sm.toString();
            QString stationname=smstr.section(" ", 0, 0);
            smstr.remove(0,stationname.length()+1);
            QString comment=smstr.right(smstr.length()-smstr.lastIndexOf("kHz")-4);
            smstr.chop(comment.length());
            QString mod=comment.section(" ", 0, 0);
            comment.remove(0,mod.length()+1);
            comment.replace(" ","_");
            stationname="Priyom.org:_"+stationname+"_"+comment;
            if (smstr.contains("kHz,"))
            {
                QStringList innerlist=smstr.split(",");
                for (int k=0;k<innerlist.count();k++)
                {
                    priyom.append(startdatestring+"|DX de PR1Y0M:     "+innerlist[k].replace("kHz",".0 ")+stationname+" ");
                }
            }
            else
            {
                priyom.append(startdatestring+"|DX de PR1Y0M:     "+smstr.replace("kHz",".0")+stationname+" "+mod+" ");
            }
       }
    }
    else
    {
       qDebug()<<QString(reply->errorString ());
       qInfo()<<"Priyom calendar download ERROR";
    }
}

void Priyom::reload()
{
    priyom.clear();
    getdata();
}

Priyom::Priyom(QObject *parent) : QObject(parent)
{
    getdata();
    // update priyom every 12 hours
    QTimer *timer_reload = new QTimer(this);
    connect(timer_reload, SIGNAL(timeout()),this, SLOT(reload()));
    timer_reload->start(12*60*60*1000);
}
