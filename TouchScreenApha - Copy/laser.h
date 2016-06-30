/* Laser Class
 * This class acts a connecting class between the interbus and the UI to reduce the amount of code in the UI source code
 *
 * First Created 29/06/2016 -SPG
 *
 * Chnage Log:
 *
 *
 * */

#ifndef LASER_H
#define LASER_H

#include "Interbus/ibhandler.h"

class Laser: public QObject
{
    Q_OBJECT
public:
    explicit Laser(QObject *parent = 0);
    ~Laser();

    IBHandler *interbus;

    bool getEmission(); //Function to check emission status. Returns true if emission is on.


    bool setAcknowlage(bool acknowlageState);
    bool setEmission(bool emission);

    //Method to check if the laser is comunicating with serial.
    bool checkComs();

    void checkWavelength();
    void updateLaserState();

    bool setUpPort(QString name);
    void closePort();



public slots:

signals:
    void emmissionChanged(bool emission);

private slots:

private:

    bool regStatusCheck(RegGetStatusTypes regStatus); //Overloaded status check method
    bool regStatusCheck(RegSetStatusTypes regStatus);

    bool emissionOn;
    quint16 wavelength;

    const quint8 devAddress = 10;


};


#endif // LASER_H
