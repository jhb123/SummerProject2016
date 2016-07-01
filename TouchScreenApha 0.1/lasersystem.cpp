#include "lasersystem.h"
#include<QDebug>
#include<QString>
#include<QDir>

LaserSystem::LaserSystem(QObject *parent) : QObject(parent)
{
    SeedLaser = new Laser(this);
    EDFA = new edfa(this);


    setUpComports();
    loadSettings();
}

LaserSystem::~LaserSystem(){
}

//Function to automatically connect EDFA and laser. Returns true if success.
bool LaserSystem::setUpComports(){
    QStringList availableComports = SeedLaser->interbus->getAvailableComports();

    bool laserFound = false;
    bool EDFAFound = false;
    bool connectSuccess = true;

    int comportCounter = 0;
    int noOfComports = availableComports.size();

    while((comportCounter < noOfComports) && (laserFound == false)){
        SeedLaser -> setUpPort(availableComports[comportCounter]);

        SeedLaser ->setAcknowlage(true);

        if(SeedLaser->checkComs()){
            laserFound = true;
            laserComOpen = true;
            laserCom = availableComports[comportCounter];
            qDebug() << "Laser found at port " << laserCom;
        }
        else{
            SeedLaser->closePort();

        }

        comportCounter++;

    }

    for(int i=0; i< noOfComports; i++){
        if(availableComports[i] != laserCom){
            EDFA->setUpPort(availableComports[i]);

            if(EDFA->replyCheck()){
                EDFAFound = true;
                EDFAComOpen = true;
                EDFACom = availableComports[i];
                qDebug() << "EDFA found at port " << EDFACom;
            }
            else{
                EDFA->closeComport();
            }
        }
    }

    if(laserFound == false){
        qDebug() << "Laser NOT found";
        laserComOpen = false;
        connectSuccess = false;
    }

    if(EDFAFound == false){
        qDebug() << "EDFA NOT found";
        EDFAComOpen = false;
      connectSuccess = false;

    }

    return connectSuccess;

}


bool LaserSystem::setEmission(bool state){
    bool success = false;
    qDebug() << "Attempting to set emission to " << state;
    if((setLaserEmission(state) == true) && (setEDFAEmission(state)== true)){
        success = true;
    }
    getEmission(false);
    return success;
}

bool LaserSystem::getEmission(bool update){
    bool emission = true;

    if(!getLaserEmission(update)){
        emission = false;
    }

    if(!getEDFAEmission(update)){
        emission = false;
    }
    qDebug() << "System on " << emission;


    emit systemEmission(emission);
    return emission;
}

bool LaserSystem::setLaserEmission(bool state){
    qDebug() << "Laser change emission to " << state;
    SeedLaser->setEmission(state);
    getEmission(true);
    return (getLaserEmission(false) == state) ?  true : false;
}

bool LaserSystem::getLaserEmission(bool update){
    if(update){
         laserOn = SeedLaser -> getEmission();
         qDebug() << "Laser emission " << laserOn;
    }
    emit laserEmission(laserOn);
    return laserOn;
}

bool LaserSystem::setEDFAEmission(bool state){
   qDebug() << "Setting EDFA emission to " << state;
   EDFA->setEmission(state);
   getEmission(true);
   return (getEDFAEmission(false) == state) ?  true : false;
}


bool LaserSystem::getEDFAEmission(bool update){
 if(update){
     EDFAOn = EDFA->getEmission();
     qDebug() << "EDFA emission " << EDFAOn;
 }
 emit EDFAEmission(EDFAOn);
 return EDFAOn;
}

bool LaserSystem::setPower(int power){

}

bool LaserSystem::setWavelength(int wavelength){

}



float LaserSystem::getPower(bool update){
    EDFAPower = 30.0; //testing;

}

float LaserSystem::getWavelength(bool update){
    laserWavelength = 1560.0; //Testing

}

void LaserSystem::updateSystemState(){
getEmission(true);

}

void LaserSystem::loadSettings(){
    settings = new QSettings(QDir::currentPath() + "/system_settings.ini", QSettings::IniFormat);
    if(QFile::exists(QDir::currentPath() + "/system_settings.ini")){
        settings -> beginGroup("Laser");
        setLaserEmission(settings->value("initialEmission").toBool());
        setWavelength(settings->value("wavelength").toFloat());
        settings -> endGroup();
        settings -> beginGroup("EDFA");
        setEDFAEmission(settings->value("initialEmission").toBool());
        setPower(settings->value("power").toFloat());
        settings->endGroup();
        qDebug() << "Loaded old ini" ;
    }
    else{
        settings->beginGroup("Laser");
        settings->setValue("initialEmission", false);
        settings->setValue("wavelength", 1560.000);
        settings->endGroup();
        settings->beginGroup("EDFA");
        settings->setValue("initialEmission", false);
        settings->setValue("power", 25);
        settings->endGroup();
        delete settings;
        loadSettings();
        qDebug() << "Loaded new ini" ;
    }
}

void LaserSystem::saveSettings(){

}
