#include "touchscreen.h"
#include "ui_touchscreen.h"

#include <QTimer>
#include <QDebug>

TouchScreen::TouchScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TouchScreen)
{
    EDFAOnOff = false;
    emissionOn = false;
    wavelength = 1560; //For debugging.
    prevWavelength = wavelength;

    ui->setupUi(this);

    ui->LaserQuestOff->show();
    ui->LaserQuestOn->hide();
    ui->EDFAOff->show();
    ui->EDFAOn->hide();
    ui->StableOff->show();
    ui->StableOn->hide();
    ui->TurnOnBtn->show();
    ui->TurnOffBtn->hide();

    // Create the Interbus handler
    interbus = new IBHandler(this);
    connect(interbus, SIGNAL(comportOpened(QString,bool)), this, SLOT(comportOpened(QString,bool)));

    // Fill the port combobox with available ports
    ui->cbComports->clear();
    ui->cbComports->addItems( interbus->getAvailableComports() );

    setupComports();

    QTimer *wavelengthTimer = new QTimer(this);
    connect(wavelengthTimer, SIGNAL(timeout()),this, SLOT(wavelength = ui->hsWavelength->value()));

    connect(ui->pbOpenClose, SIGNAL(pressed()), this, SLOT(butOpenClose()));
    connect(ui->TurnOnBtn, SIGNAL(pressed()), this, SLOT(setSystemState()));
    connect(ui->TurnOffBtn, SIGNAL(pressed()), this, SLOT(setSystemState()));

    //Change line edit to be read only and print make disabled untill com port is connected.

    //Serial commands used to communicate with EDFA


    //button pressed to turn edfa on/off

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()),this,SLOT(checkLaser()));
  timer->start(250);
}

TouchScreen::~TouchScreen()
{
    delete ui;
}

bool TouchScreen::regStatusCheck(RegSetStatusTypes regStatus){
    bool statusSuccess = false;

    //Switch for possible errors.
    switch (regStatus)
        {
        case GetSuccess:
            statusSuccess = true;
            break;
        case GetBusy:           ui->pteStatus->appendPlainText("Read busy"); break;  //Prints error to pte. Pte name may need changing
        case GetNacked:         ui->pteStatus->appendPlainText("Read nacked"); break;
        case GetCRCErr:         ui->pteStatus->appendPlainText("Read CRC error"); break;
        case GetTimeout:        ui->pteStatus->appendPlainText("Read timeout error"); break;
        case GetComError:       ui->pteStatus->appendPlainText("Read communication error"); break;
        case GetPortClosed:     ui->pteStatus->appendPlainText("Read port closed"); break;
        case GetPortNotFound:   ui->pteStatus->appendPlainText("Read port not found"); break;
        }

    return statusSuccess;
}

bool TouchScreen::regStatusCheck(RegGetStatusTypes regStatus){
    bool statusSuccess = false;

    //Switch for possible errors.
    switch (regStatus)
        {
        case GetSuccess:
            statusSuccess = true;
            break;
        case GetBusy:           ui->pteStatus->appendPlainText("Read busy"); break;  //Prints error to pte. Pte name may need changing
        case GetNacked:         ui->pteStatus->appendPlainText("Read nacked"); break;
        case GetCRCErr:         ui->pteStatus->appendPlainText("Read CRC error"); break;
        case GetTimeout:        ui->pteStatus->appendPlainText("Read timeout error"); break;
        case GetComError:       ui->pteStatus->appendPlainText("Read communication error"); break;
        case GetPortClosed:     ui->pteStatus->appendPlainText("Read port closed"); break;
        case GetPortNotFound:   ui->pteStatus->appendPlainText("Read port not found"); break;
        }

    return statusSuccess;
}

//Method to check the emission state of the laser.
void TouchScreen::checkEmission(){
    QByteArray readData;

    if(regStatusCheck(interbus->readRegister(devAddress, 0x30, readData))){
        quint8 emissionData = interbus -> GetInt8(readData, 0);

        if(emissionData == 1){
            ui->LaserQuestOn->show();
            ui->LaserQuestOff->hide();
            //Where change to on off button will go.
            emissionOn = true;
        }
        else if(emissionData == 0){
            ui->LaserQuestOff->show();
            ui->LaserQuestOn->hide();
            emissionOn = false;
        }
        else{
            ui->pteStatus->appendPlainText("Unknown emission state");
        }
    }
    else{
        ui->pteStatus->appendPlainText("Error in Emission read");
    }

   }

void TouchScreen::setupComports(){
    QStringList availableComports = interbus->getAvailableComports();

    bool laserFound = false;

    int comportCounter = 0;
    int noOfComports = availableComports.size();
    ui->pteStatus->appendPlainText(QString::number(noOfComports));



     while((comportCounter < noOfComports) && (laserFound == false)){

        interbus -> openComport(availableComports[comportCounter]);

        QByteArray availableCheck;
        //Sets acknowlage mode to on. 36 is the acknowlage register.
        interbus->SetInt8(availableCheck, 0x01, 0); //Sets buffer.
        interbus->writeRegister(devAddress, 0x36, availableCheck);

        if(regStatusCheck(interbus->readRegister(devAddress, 0x30, availableCheck))){
            ui->pteStatus->appendPlainText("Laser found @ " + availableComports[comportCounter]);
            laserFound = true;
            laserComport = availableComports[comportCounter];
        }
        else{
            ui->pteStatus->appendPlainText("Laser NOT found @ " + availableComports[comportCounter]);
            interbus->closeComport();

        }

        comportCounter++;

    }

    if(laserFound == false){
      ui->pteStatus->appendPlainText("No laser found");
    }

}

void TouchScreen::butOpenClose()
{
    if (interbus->isComportOpen()){
        interbus->closeComport();
        wavelengthTimer -> stop();

    }
    else{
        interbus->openComport(ui->cbComports->currentText());
        checkEmission();
        checkWavelength();
        //wavelengthTimer -> start(500);
    }
}



void TouchScreen::comportOpened(QString portname, bool opened)
{
    Q_UNUSED(portname)

    ui->cbComports->setEnabled(!opened);
    if (opened)
        ui->pbOpenClose->setText(tr("Close"));
    else
        ui->pbOpenClose->setText(tr("Open"));
}


void TouchScreen::updateLaserState(){
    //Function which can run on a timer to update all of the measured laser values.
}

void TouchScreen::checkWavelength(){
    wavelength = 1560;

}

//Method for sending on/off commands.
void TouchScreen::setSystemState(){
    qint8 onOffSend;
    if(emissionOn){ //Checks state of emission at this time.
        onOffSend = 0x00;
        ui->TurnOnBtn->show();
        ui->TurnOffBtn->hide();
    }
    else{
        onOffSend = 0x01;
        ui->TurnOffBtn->show();
        ui->TurnOnBtn->hide();
    }

    ui->pteStatus->appendPlainText(QString::number(onOffSend));
    QByteArray writeData;

    interbus->SetInt8(writeData, onOffSend, 0); //Sets buffer.

    if(regStatusCheck(interbus->writeRegister(devAddress, 0x30, writeData))){


    checkEmission(); //Checks the new emission state may not need as if the previous if passed the laser recieved the message.

    //turns on the amplifier if the emission was turned on
    if(emissionOn){
        EDFAOnOff = false;
        ui->TurnOffBtn->show();
        ui->TurnOnBtn->hide();
        QTimer::singleShot(2000, this, SLOT(EDFAOnOffSwitch()));
        ui->EDFAOn->show();
        ui->EDFAOff->hide();
    }
    else{
        EDFAOnOff = true;
        QTimer::singleShot(10, this, SLOT(EDFAOnOffSwitch()));
        ui->TurnOnBtn->show();
        ui->TurnOffBtn->hide();
        ui->EDFAOn->hide();
        ui->EDFAOff->show();
    }
   }
   else{
        ui->pteStatus->appendPlainText("Emission state could not be updated");
    }
}
