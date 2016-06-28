/*
 * MainWindow
 * This is the implimentation file for the mainwondow class which handles all of the UI function.
 *
 * The class uses an "Interbus" provided by NKT photonics which handles all of the low level
 * serialEDFA communication.
 *
 * First version created 27/06/16
 *
 * Change log:
 * 27/06- SPG Updated reg check to a function so that it can be checked without repeating code.
 *
 *
 * */


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    EDFAOnOff = false;
    wavelength = 1560; //For debugging.
    prevWavelength = wavelength;

    ui->setupUi(this);

    // Create the Interbus handler
    interbus = new IBHandler(this);
    connect(interbus, SIGNAL(comportOpened(QString,bool)), this, SLOT(comportOpened(QString,bool)));

    // Fill the port combobox with available ports
    ui->cbComports->clear();
    ui->cbComports->addItems(interbus->getAvailableComports());

    setupComports();

    //Creates a new timer for reading wavelength data at interval.
    QTimer *wavelengthTimer = new QTimer(this);
    connect(wavelengthTimer, SIGNAL(timeout()),this, SLOT(wavelength = ui->hsWavelength->value()));

    //Change line edit to be read only and print make disabled untill com port is connected.
    ui->emissionState->setReadOnly(true);
    ui->emissionState ->setText("Unavailable");

    ui->hsWavelength->setEnabled(false);
    ui->hsWavelength->setTracking(true);

    //Connects slots for open/close and switch emission buttons.
    connect(ui->pbOpenClose, SIGNAL(pressed()), this, SLOT(butOpenClose()));
    connect(ui->pbSwitchEmission, SIGNAL(pressed()), this, SLOT(switchEmissionState()));

    //Connects for wavelength slider.
    connect(ui->hsWavelength, SIGNAL(valueChanged(int)),
            ui->sbWavelength, SLOT(setValue(int)));
    connect(ui->sbWavelength, SIGNAL(valueChanged(int)),
            ui->hsWavelength, SLOT(setValue(int)));
    //connect(ui->sbWavelength, SIGNAL(sliderReleased(int)),this, SLOT(wavelength = this->value()));

    ui->pbSwitchEmission->setEnabled(false);
/*
    //serialEDFA commands used to communicate with EDFA

    serialEDFA = new QserialEDFAPort(this);
    serialEDFA->setPortName("COM3");
    serialEDFA->setBaudRate(QserialEDFAPort::Baud9600);
    serialEDFA->setDataBits(QserialEDFAPort::Data8);
    serialEDFA->setParity(QserialEDFAPort::NoParity);
    serialEDFA->setStopBits(QserialEDFAPort::OneStop);
    serialEDFA->setFlowControl(QserialEDFAPort::NoFlowControl);
    serialEDFA->open(QIODevice::ReadWrite);

    serialEDFA->write("OM 0\r\n");
*/
    //button pressed to turn edfa on/off
    connect(ui->EDFAOnOff,SIGNAL(pressed()),this,SLOT(EDFAOnOffSwitch()));

  //  timer = new QTimer(this);
  //  connect(timer, SIGNAL(timeout()),this,SLOT(myfunction()));
   // timer->start(2000);
}

//Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

//Method to check if the read/write has been successful. Returns true bool if it has been successful.
bool MainWindow::regStatusCheck(RegGetStatusTypes regStatus){
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

bool MainWindow::regStatusCheck(RegSetStatusTypes regStatus){
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

void MainWindow::setupComports(){
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

//Method to check which function open/close button should have and also to
//disable certain buttons when no device is connected.
void MainWindow::butOpenClose()
{
    if (interbus->isComportOpen()){
        interbus->closeComport();
        ui->emissionState ->setText("Unavailable");
        ui->pbSwitchEmission->setEnabled(false);  //Disables buttons for changing emission and wavelength
        ui->hsWavelength->setEnabled(false);
        //wavelengthTimer -> stop();

    }
    else{
        interbus->openComport(ui->cbComports->currentText()); //Opens selected com port
        checkEmission();  //Checks if the laser is emitting.
        emissionStateDispUpdate();  //Updates the emission diplay.
        ui->pbSwitchEmission->setEnabled(true); //Enables buttons
        ui->hsWavelength->setEnabled(true);
        checkWavelength();
        //wavelengthTimer -> start(500);
    }
}

//Chnages button text based on state of com ports.
void MainWindow::comportOpened(QString portname, bool opened)
{
    Q_UNUSED(portname)

    ui->cbComports->setEnabled(!opened);
    if (opened)
        ui->pbOpenClose->setText(tr("Close"));
    else
        ui->pbOpenClose->setText(tr("Open"));
}

//Debug method.
void MainWindow::myfunction()
{
    qDebug() << "Update..";
}


//Method to check the emission state of the laser.
void MainWindow::checkEmission(){
    QByteArray readData;

    if(regStatusCheck(interbus->readRegister(devAddress, 0x30, readData))){
        quint8 emissionData = interbus -> GetInt8(readData, 0);

        if(emissionData == 1){
            ui->pteStatus->appendPlainText("Emission on");
            //Where change to on off button will go.
            emissionOn = true;
        }
        else if(emissionData == 0){
            ui->pteStatus->appendPlainText("Emission off");
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

//Method for sending on/off commands.
void MainWindow::switchEmissionState(){
    qint8 onOffSend;
    if(emissionOn){ //Checks state of emission at this time.
        onOffSend = 0x00;
    }
    else{
        onOffSend = 0x01;
    }

    ui->pteStatus->appendPlainText(QString::number(onOffSend));
    QByteArray writeData;

    interbus->SetInt8(writeData, onOffSend, 0); //Sets buffer.

    if(regStatusCheck(interbus->writeRegister(devAddress, 0x30, writeData))){


    checkEmission(); //Checks the new emission state may not need as if the previous if passed the laser recieved the message.
    emissionStateDispUpdate();

    //turns on the amplifier if the emission was turned on
    if(emissionOn){
        EDFAOnOff = false;
        QTimer::singleShot(2000, this, SLOT(EDFAOnOffSwitch()));
    }
    else{
        EDFAOnOff = true;
        QTimer::singleShot(10, this, SLOT(EDFAOnOffSwitch()));
    }
   }
   else{
        ui->pteStatus->appendPlainText("Emission state could not be updated");
    }
}


    //Function to change emission state display window.
void MainWindow::emissionStateDispUpdate(){
    if(emissionOn){
        ui->emissionState->setText("Emission On");
    }
    else{
        ui->emissionState->setText("Emission Off");
    }
}

void MainWindow::updateLaserState(){
    //Function which can run on a timer to update all of the measured laser values.
}

void MainWindow::checkWavelength(){
    wavelength = 1560;

}


void MainWindow::setWavelength(){

if(prevWavelength != wavelength){
 wavelength=ui->hsWavelength->value();
 ui->pteStatus->appendPlainText("Wavelength set to: " + QString::number(wavelength) + " nm");
 prevWavelength=wavelength;
}

}
//turns on the edfa if EDFAOnOff is false and other way around if true
void MainWindow::EDFAOnOffSwitch(){
    if (EDFAOnOff == true){
        serialEDFA->write("OM 0\r\n"); //off
        serialEDFA->waitForBytesWritten(50);
        ui->EDFAOnOff->setText("Turn On");
        EDFAOnOff = false;

    }
    else{
        EDFAOnOff = true;
        serialEDFA->write("OM 2\r\n");
        serialEDFA->waitForBytesWritten(50);
        ui->EDFAOnOff->setText("Turn Off");
    }
}

