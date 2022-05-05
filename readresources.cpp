#include "readresources.h"

QDir dir;
QStringList station_list;

QStringList ReadResources::station_name_list()
{
    QFile file(dir.path()+"broadcas.txt");
    qDebug()<<"reading: "+dir.path()+"broadcas.txt";
    QStringList station_list;
    station_list.clear();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
       {
        qInfo()<<"Cant read broadcas.txt file";
        return {"error","999"};
       }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        station_list.append(line);
    }
    file.close();
    return station_list;
}

void ReadResources::rread()
{
    station_list=station_name_list();
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks );
    QFileInfoList list = dir.entryInfoList();
    int j=0;
    for (int i = 0; i < list.size(); ++i)
     {
         if (list.at(i).fileName()=="LIST.list")
         {
             j++;
             QFile file(dir.path()+qPrintable(QString("%2").arg(list.at(i).fileName())));
             if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             {   qInfo()<<"Cant read file"<<dir.path()+qPrintable(QString("%2").arg(list.at(i).fileName()));
                 return;
             }
             qDebug()<<"Reading file"<<dir.path()+qPrintable(QString("%2").arg(list.at(i).fileName()));
             QTextStream in(&file);
             int i=0;
             while (!in.atEnd())
             {
                 QString line = in.readLine();
                 QStringRef Freq(&line,0,5);
                 QStringRef Start_date(&line,80,6);
                 QStringRef End_date(&line,87,6);
                 QStringRef Start_time(&line,6,4);
                 QStringRef End_time(&line,11,4);
                 QStringRef DayoftheWeek(&line,72,7);
                 QStringRef Language(&line,102,10);
                 QStringRef Power(&line,51,4);
                 QStringRef StationName(&line,117,3);



                 if (!Freq.toString().contains(";"))
                 {
                     if (End_time.toString()=="2400")
                     {
                         QString endt= "2359";
                         End_time.clear();
                         End_time=QStringRef(&endt,0,4);
                         if (DayoftheWeek.contains(QString::number(QDate::currentDate().dayOfWeek()))
                              && QDate::fromString(Start_date.toString(),"ddMMyy").addYears(100)<=QDateTime::currentDateTimeUtc().date()
                               && QDateTime::currentDateTimeUtc().date()<=QDate::fromString(End_date.toString(),"ddMMyy").addYears(100)
                                && QDateTime::fromString(Start_time.toString(),"hhmm").time()<=QDateTime::currentDateTimeUtc().time()
                                 && QDateTime::currentDateTimeUtc().time()<=QDateTime::fromString(End_time.toString(),"hhmm").time()
                                  )
                                 for (int i = 0; i < station_list.size(); ++i)
                                 {
                                     QStringRef StationShortName(&station_list.at(i),0,3);
                                     QStringRef StationLongName(&station_list.at(i),4,station_list.at(i).length()-4);
                                     if (StationName.toString()==StationShortName.toString())
                                       {
                                        marker.append("DX de SH1VA2:     "+Freq.toString()+".0 "+StationLongName.toString().replace(" ","_")+" AM "+QDateTime::currentDateTimeUtc().time().toString("hhmm")+"Z\r\n");
                                       }
                                 }
                      }
                     else if (DayoftheWeek.contains(QString::number(QDate::currentDate().dayOfWeek()))
                          && QDate::fromString(Start_date.toString(),"ddMMyy").addYears(100)<=QDateTime::currentDateTimeUtc().date()
                           && QDateTime::currentDateTimeUtc().date()<=QDate::fromString(End_date.toString(),"ddMMyy").addYears(100)
                            && QDateTime::fromString(Start_time.toString(),"hhmm").time()<=QDateTime::currentDateTimeUtc().time()
                             && QDateTime::currentDateTimeUtc().time()<=QDateTime::fromString(End_time.toString(),"hhmm").time()
                              )
                         for (int i = 0; i < station_list.size(); ++i)
                         {
                             QStringRef StationShortName(&station_list.at(i),0,3);
                             QStringRef StationLongName(&station_list.at(i),4,station_list.at(i).length()-4);
                             if (StationName.toString()==StationShortName.toString())
                                     {
                                      marker.append("DX de SH1VA2:     "+Freq.toString()+".0 "+StationLongName.toString().replace(" ","_")+" AM "+QDateTime::currentDateTimeUtc().time().toString("hhmm")+"Z\r\n");
                                     }
                         }
                  /*   else
                         for (int i = 0; i < station_list.size(); ++i)
                         {
                             QStringRef StationShortName(&station_list.at(i),0,3);
                             QStringRef StationLongName(&station_list.at(i),4,station_list.at(i).length()-4);
                             if (StationName.toString()==StationShortName.toString())
                                 qDebug()<<"Skipped:"<<Freq<<Start_date<<Start_time<<End_date<<End_time<<DayoftheWeek<<StationLongName<<Language.trimmed()<<Power.trimmed();
                         }*/
                 }
                 i++;
             }
             qDebug()<<i<<"lines read";
             file.close();
         }
         else if (list.at(i).suffix()=="list" && list.at(i).fileName()!="LIST.list")
         {
             j++;
             QFile file(dir.path()+qPrintable(QString("%2").arg(list.at(i).fileName())));
             if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             {   qDebug()<<"Cant read file"<<dir.path()+qPrintable(QString("%2").arg(list.at(i).fileName()));
                 return;
             }
             qDebug()<<"Reading file"<<dir.path()+qPrintable(QString("%2").arg(list.at(i).fileName()));
             QTextStream in(&file);
             int i=0;
             while (!in.atEnd())
             {
                 QString line = in.readLine();
                 QStringList details = line.split(',');
                 if ( details.count() == 8 )
                 {
                     QString Start_date = details.at(0);
                     QString End_date = details.at(1);
                     QString DayoftheWeek = details.at(2);
                     QString Start_time = details.at(3);
                     QString End_time = details.at(4);
                     float Freq = details.at(5).toInt();
                     Freq=Freq/1000;
                     QString Mod = details.at(6);
                     QString StationName = details.at(7);
                     if ( End_date.endsWith("****") ) //yearly events
                     {
                         End_date.replace(4,4,QDateTime::currentDateTimeUtc().toString("yyyy"));
                     }
                     if (DayoftheWeek.contains(QString::number(QDate::currentDate().dayOfWeek()))
                                          && QDate::fromString(Start_date,"ddMMyyyy")<=QDateTime::currentDateTimeUtc().date()
                                           && QDateTime::currentDateTimeUtc().date()<=QDate::fromString(End_date,"ddMMyyyy")
                                            && QDateTime::fromString(Start_time,"hhmm").time()<=QDateTime::currentDateTimeUtc().time()
                                             && QDateTime::currentDateTimeUtc().time()<=QDateTime::fromString(End_time,"hhmm").time()
                                              )
                     {
                         marker.append("DX de SH1VA2:     "+QString::number(Freq,'f',1)+" "+StationName.replace(" ","_")+" "+Mod+" "+QDateTime::currentDateTimeUtc().time().toString("hhmm")+"Z\r\n");
                     }
                 }
                 i++;
             }
             qDebug()<<i<<"lines read";
             file.close();
         }
     }
    if (j==0)
        qInfo()<<"No List files Found";
 }

void ReadResources::reload()
{
    marker.clear();
    rread();
}

ReadResources::ReadResources(QObject *parent) : QObject(parent)
{
    QSettings settings;
    QString path=settings.value("wrkdir").toString();
    dir.setPath(path);
    rread();
    // update markers every 1 min
    QTimer *timer_reload = new QTimer(this);
    connect(timer_reload, SIGNAL(timeout()),this, SLOT(reload()));
    timer_reload->start(60000);
}
