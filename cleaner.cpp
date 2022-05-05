#include "cleaner.h"


void Cleaner::clean()
{
    // remove spots older than 1 min and number stations older than 1 hour
    for (int i=0;i<spot.size();i++)
    {
        QDateTime dt=QDateTime::fromString(spot.at(i).toLocal8Bit().left(24),"ddd MMM dd hh:mm:ss yyyy");
        dt.setTimeSpec(Qt::UTC);
        if (QDateTime::currentDateTimeUtc().secsTo(dt)<-60)
        {
            spot.removeAt(i);
        }
    }
    for (int i=0;i<priyom.size();i++)
    {
        QDateTime dt=QDateTime::fromString(priyom.at(i).toLocal8Bit().left(24),"ddd MMM dd hh:mm:ss yyyy");
        dt.setTimeSpec(Qt::UTC);
        if (QDateTime::currentDateTimeUtc().secsTo(dt)<-60*60)
        {
            priyom.removeAt(i);
        }
    }
}




Cleaner::Cleaner(QObject *parent) : QObject(parent)
{
    QTimer *timer_clean = new QTimer(this);
    connect(timer_clean, SIGNAL(timeout()),this, SLOT(clean()));
    timer_clean->start(60000);
}
