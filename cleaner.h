#ifndef CLEANER_H
#define CLEANER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QTimer>

extern QStringList spot;
extern QStringList priyom;

class Cleaner : public QObject
{
    Q_OBJECT
public:
    explicit Cleaner(QObject *parent = nullptr);

signals:

private slots:
    void clean();
};

#endif // CLEANER_H
