#ifndef PRIYOM_H
#define PRIYOM_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QDateTime>
#include <QTimer>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

extern QStringList priyom;

class Priyom : public QObject
{
    Q_OBJECT
public:
    explicit Priyom(QObject *parent = nullptr);

private:
    void getdata();
private slots:
    void done(QNetworkReply*);
signals:

private slots:
    void reload();

};

#endif // PRIYOM_H
