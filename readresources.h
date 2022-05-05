#ifndef READRESOURCES_H
#define READRESOURCES_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QTimer>

extern QStringList marker;

class ReadResources : public QObject
{
    Q_OBJECT
private:
    QStringList station_name_list();
    void rread();
public:
    explicit ReadResources(QObject *parent = nullptr);

signals:

private slots:
    void reload();

};

#endif // READRESOURCES_H
