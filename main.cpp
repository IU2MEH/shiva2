#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <QSettings>
#include <QString>
#include <QDir>
#include <QUrl>
#include <readresources.h>
#include <server.h>
#include <dxclient.h>
#include <priyom.h>
#include <cleaner.h>

QStringList marker;
QStringList spot;
QStringList priyom;

void noDebugOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString current_time=QDateTime::currentDateTime().toString("ddMMyyyy-hh:mm:ss");
    switch (type) {
    case QtDebugMsg:
//        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
//        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(stderr,"%s Info: %s \n",current_time.toStdString().c_str(),localMsg.constData());
        break;
    case QtWarningMsg:
//        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "%s Critical: %s (%s:%u, %s)\n",current_time.toStdString().c_str(), localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "%s Fatal: %s (%s:%u, %s)\n",current_time.toStdString().c_str(), localMsg.constData(), context.file, context.line, context.function);
        abort();
    }

}

void DebugOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString current_time=QDateTime::currentDateTime().toString("ddMMyyyy-hh:mm:ss");
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "%s Debug: %s (%s:%u, %s)\n",current_time.toStdString().c_str(),localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "%s Info: %s (%s:%u, %s)\n",current_time.toStdString().c_str(),localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "%s Warning: %s (%s:%u, %s)\n",current_time.toStdString().c_str(),localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "%s Critical: %s (%s:%u, %s)\n",current_time.toStdString().c_str(), localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "%s Fatal: %s (%s:%u, %s)\n",current_time.toStdString().c_str(), localMsg.constData(), context.file, context.line, context.function);
        abort();
    }

}

void ask_port()
{
    QSettings settings;
    QTextStream cin(stdin);
    qInfo().nospace()<<"A free TCP port to listen on: ";
    int tcpport=cin.readLine().toInt();
    if (tcpport<1024||tcpport>65535)
    {
        qInfo()<<"TCP port must be between 1024 and 65535";
        ask_port();
    }
    else
        settings.setValue("tcpport",tcpport);
}

void ask_callsign()
{
    QSettings settings;
    QTextStream cin(stdin);
    qInfo().nospace()<<"Your callsign: ";
    QString callsign=cin.readLine();
    if (callsign.isEmpty())
    {
        qInfo()<<"Callsign must not be empty!";
        ask_callsign();
    }
    else
        settings.setValue("callsign",callsign);
}

void ask_dxcluster()
{
    QSettings settings;
    QTextStream cin(stdin);
    qInfo().nospace()<<"Public DX cluster to connect to: ";
    QString dxcluster=cin.readLine();
    if(dxcluster.isEmpty())
    {
        qInfo()<<"Public DX cluster must not be empty!";
        ask_dxcluster();
    }
    else
    {
        QUrl url(dxcluster);
        if (!url.isValid())
        {
            qInfo()<<"Public DX cluster invalid!";
            ask_dxcluster();
        }
        else
            settings.setValue("dxcluster",dxcluster);
    }
}

void ask_wrkdir()
{
    if (QSysInfo::kernelType()=="winnt")
    {
        QSettings settings;
        QTextStream cin(stdin);

        qInfo()<<"Directory with resources: ";
        qInfo()<<"(press enter for default C:\\Shiva2\\)";

        QString wrkdir=cin.readLine();
        if (wrkdir.isEmpty())
            wrkdir="C:\\Shiva2\\\\";
        else
            wrkdir.append("\\");
        QDir dir(wrkdir);
        if (!dir.exists())
        {
            qInfo()<<"Directory not found! Create it? [y/N]";
            QString reply=cin.readLine();
            if (reply=="y")
            {
                dir.mkpath(wrkdir);
                settings.setValue("wrkdir",wrkdir);
            }
            else
                ask_wrkdir();
        }
        else
            settings.setValue("wrkdir",wrkdir);
    }

    else if (QSysInfo::kernelType()=="linux")
    {
        QSettings settings;
        QTextStream cin(stdin);

        qInfo()<<"Directory with resources: ";
        qInfo()<<"(press enter for default /etc/shiva2/)";

        QString wrkdir=cin.readLine();
        if (wrkdir.isEmpty())
            wrkdir="//etc//shiva2//";
        else
        {
            wrkdir.append("/");
            wrkdir.replace("/","//");
        }
        qInfo()<<"Directory"<<wrkdir;
        QDir dir(wrkdir);
        if (!dir.exists())
        {
            qInfo()<<"Directory"<<wrkdir<<"not found!";
            ask_wrkdir();
        }
        else
            settings.setValue("wrkdir",wrkdir);
    }
}

void ask_interval()
{
    QSettings settings;
    QTextStream cin(stdin);
    qInfo().nospace()<<"Sending interval in seconds (press enter for default = 30 secs) : ";
    int interval=cin.readLine().toInt();
    if (interval==0)
        interval=30;
    if (interval<5||interval>1800)
    {
        qInfo()<<"Interval must be between 5 (5 seconds) and 1800 (30 mins)";
        ask_interval();
    }
    else
        settings.setValue("interval",interval*1000);
}

void ask_timeout()
{
    QSettings settings;
    QTextStream cin(stdin);
    qInfo().nospace()<<"Timeout in seconds (press enter for default = 2 seconds) : ";
    int timeout=cin.readLine().toInt();
    if (timeout==0)
        timeout=2;
    if (timeout<2||timeout>60)
    {
        qInfo()<<"Timeout must be between 2 (2 seconds) and 60 (1 min)";
        ask_timeout();
    }
    else
        settings.setValue("timeout",timeout*1000);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if ( ! a.arguments().contains(QLatin1String("-d"))) {
        qInstallMessageHandler(noDebugOutput);
    }
    else
        qInstallMessageHandler(DebugOutput);

    QCoreApplication::setOrganizationName("IU2MEH");
    QCoreApplication::setOrganizationDomain("-");
    QCoreApplication::setApplicationName("Shiva2");
    QSettings settings;
    QTextStream cin(stdin);
    if (! a.arguments().contains(QLatin1String("-b")))
    {
        if (!settings.value("tcpport").isNull()&&!settings.value("callsign").isNull()&&!settings.value("dxcluster").isNull())
        {
            qInfo()<<"Settings found:";
            qInfo()<<"TCP port: "+settings.value("tcpport").toString();
            qInfo()<<"Callsign: "+settings.value("callsign").toString();
            qInfo()<<"Public DX cluster: "+settings.value("dxcluster").toString();
            qInfo()<<"Directory with resources: "+settings.value("wrkdir").toString();
            int interval=settings.value("interval").toInt()/1000;
            qInfo()<<"Interval: "+QString::number(interval)+" seconds";
            int timeout=settings.value("timeout").toInt()/1000;
            qInfo()<<"Timeout: "+QString::number(timeout)+" seconds";
            qInfo().noquote()<<"Do you want to change them? [y/N] ";
            QString reply=cin.readLine();
            if (reply=="y")
                settings.clear();
        }
        if (settings.value("tcpport").isNull()||settings.value("callsign").isNull()||settings.value("dxcluster").isNull())
        {
            settings.clear();
            qInfo()<<"Please fill settings:";
            ask_port();
            ask_callsign();
            ask_dxcluster();
            ask_wrkdir();
            ask_interval();
            ask_timeout();
        }
    }

    qInfo()<<"Starting...";

    marker.clear();
    spot.clear();
    priyom.clear();

    Server myserver;
    ReadResources readresources;
    DXClient dxclient;
    Priyom priyom;
    Cleaner cleaner;

    return a.exec();
}
