#include "Laser.h"


Laser::Laser(QObject *parent) : QObject(parent)
{
    interbus = new IBHandler(this);

}

Laser::~Laser(){
}

//Private functions to check the status of read and write
bool Laser::regStatusCheck(RegSetStatusTypes regStatus){
    bool statusSuccess = false;

    //Switch for possible errors.
    switch (regStatus)
        {
        case GetSuccess:
            statusSuccess = true;
            break;
     //Need some way to throw errors
    /*
        case GetBusy:           ui->pteStatus->appendPlainText("Read busy"); break;  //Prints error to pte. Pte name may need changing
        case GetNacked:         ui->pteStatus->appendPlainText("Read nacked"); break;
        case GetCRCErr:         ui->pteStatus->appendPlainText("Read CRC error"); break;
        case GetTimeout:        ui->pteStatus->appendPlainText("Read timeout error"); break;
        case GetComError:       ui->pteStatus->appendPlainText("Read communication error"); break;
        case GetPortClosed:     ui->pteStatus->appendPlainText("Read port closed"); break;
        case GetPortNotFound:   ui->pteStatus->appendPlainText("Read port not found"); break;
        */
        }

    return statusSuccess;
}

bool Laser::regStatusCheck(RegGetStatusTypes regStatus){
    bool statusSuccess = false;

    //Switch for possible errors.
    switch (regStatus)
        {
        case GetSuccess:
            statusSuccess = true;
            break;
        /*
        case GetBusy:           ui->pteStatus->appendPlainText("Read busy"); break;  //Prints error to pte. Pte name may need changing
        case GetNacked:         ui->pteStatus->appendPlainText("Read nacked"); break;
        case GetCRCErr:         ui->pteStatus->appendPlainText("Read CRC error"); break;
        case GetTimeout:        ui->pteStatus->appendPlainText("Read timeout error"); break;
        case GetComError:       ui->pteStatus->appendPlainText("Read communication error"); break;
        case GetPortClosed:     ui->pteStatus->appendPlainText("Read port closed"); break;
        case GetPortNotFound:   ui->pteStatus->appendPlainText("Read port not found"); break;
        */
        }

    return statusSuccess;
}

bool Laser::setUpPort(QString name){
    return interbus -> openComport(name);
}

void Laser::closePort(){
    interbus -> closeComport();
}

bool Laser::checkComs(){
    QByteArray readData;
    return regStatusCheck(interbus->readRegister(devAddress, 0x30, readData));
}

//Method to check the emission state of the laser.
bool Laser::getEmission(){
    QByteArray readData;
    if(regStatusCheck(interbus->readRegister(devAddress, 0x30, readData))){
        quint8 emissionData = interbus -> GetInt8(readData, 0);
        if(emissionData == 1){
            return true;
        }
        else if(emissionData == 0){
            return false;
        }

    }
}

//May need to give some error here if neither of these two cases occur. At this time, the previous emission state would be returned.

bool Laser::setEmission(bool emission){
    QByteArray writeData;
    if(emission){
        interbus->SetInt8(writeData, 0x01, 0); //Sets buffer.
    }
    else{
        interbus->SetInt8(writeData, 0x00, 0); //Sets buffer.
    }

    return regStatusCheck(interbus->writeRegister(devAddress, 0x30, writeData));
}

bool Laser::setAcknowlage(bool acknowlageState){
    QByteArray availableCheck;
    if(acknowlageState){
        interbus->SetInt8(availableCheck, 0x01, 0); //Sets buffer.
    }
    else{
        interbus->SetInt8(availableCheck, 0x00, 0); //Sets buffer.
    }

    return regStatusCheck(interbus->writeRegister(devAddress, 0x36, availableCheck));
}
