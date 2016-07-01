#ifndef LASERSYSTEM_H
#define LASERSYSTEM_H

#include "laser.h"
#include "edfa.h"
#include <QSettings>

class LaserSystem: public QObject
{
    Q_OBJECT
public:
    explicit LaserSystem(QObject *parent = 0);
    ~LaserSystem();

    edfa *EDFA;
    Laser *SeedLaser;

    bool setUpComports();

    bool setEmission(bool state);
    bool setLaserEmission(bool state);
    bool setEDFAEmission(bool state);

    bool setPower(int power);

    bool setWavelength(int wavelength);


    bool getEmission(bool update);
    bool getLaserEmission(bool update);
    bool getEDFAEmission(bool update);

    float getPower(bool update);

    float getWavelength(bool update);



public slots:
    void updateSystemState();
    void saveSettings();

signals:
    void laserComportOpen(bool open);

    void laserEmission(bool emission);
    void EDFAEmission(bool emission);
    void systemEmission(bool emission);

    void laserWavelengthUpdate(float wavelength);
    void EDFAPowerUpdate(float power);


private:

    bool EDFAOn;
    bool laserOn;
    bool systemOn;

    bool laserComOpen;
    bool EDFAComOpen;


    float EDFAPower;
    float laserWavelength;

    QString laserCom;
    QString EDFACom;

    void loadSettings();

    QSettings *settings;







};

#endif // LASERSYSTEM_H
