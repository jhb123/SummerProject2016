#ifndef EDFA_H
#define EDFA_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QSerialPort>


class edfa: public QObject
{
    Q_OBJECT
public:
    explicit edfa(QObject *parent = 0);
    ~edfa();
    bool setUpPort(QString name);
    void closeComport();

    void EDFAsetPower(float value);

    //Added to make similar to laser - SPG 29/06
    void setEmission(bool emission);

    QString EDFAReadPh1();
    QString EDFAReadPh2();

signals:
        void EDFAComportOpen(bool open);

private:
    QSerialPort *serial;

    void EDFAOn();
    void EDFAOff();



};

#endif // EDFA_H
