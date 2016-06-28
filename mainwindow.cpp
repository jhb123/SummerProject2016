#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QDebug>
QSerialPort *serial;

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
    ui->cbComports->addItems( interbus->getAvailableComports() );

    QTimer *wavelengthTimer = new QTimer(this);
    connect(wavelengthTimer, SIGNAL(timeout()),this, SLOT(wavelength = ui->hsWavelength->value()));

    //Change line edit to be read only and print make disabled untill com port is connected.
    ui->emissionState->setReadOnly(true);
    ui->emissionState ->setText("Unavailable");

    ui->hsWavelength->setEnabled(false);
    ui->hsWavelength->setTracking(true);

    connect(ui->pbOpenClose, SIGNAL(pressed()), this, SLOT(butOpenClose()));
    connect(ui->pbSwitchEmission, SIGNAL(pressed()), this, SLOT(switchEmissionState()));

    //Connects for wavelength slider.
    connect(ui->hsWavelength, SIGNAL(valueChanged(int)),
            ui->sbWavelength, SLOT(setValue(int)));
    connect(ui->sbWavelength, SIGNAL(valueChanged(int)),
            ui->hsWavelength, SLOT(setValue(int)));
    //connect(ui->sbWavelength, SIGNAL(sliderReleased(int)),this, SLOT(wavelength = this->value()));

    ui->pbSwitchEmission->setEnabled(false);

    //Serial commands used to communicate with EDFA

    serial = new QSerialPort(this);
    serial->setPortName("COM3");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);

    serial->write("OM 0\r\n");

    //button pressed to turn edfa on/off
    connect(ui->EDFAOnOff,SIGNAL(pressed()),this,SLOT(EDFAOnOffSwitch()));

  //  timer = new QTimer(this);
  //  connect(timer, SIGNAL(timeout()),this,SLOT(myfunction()));
   // timer->start(2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::butOpenClose()
{
    if (interbus->isComportOpen()){
        interbus->closeComport();
        ui->emissionState ->setText("Unavailable");
        ui->pbSwitchEmission->setEnabled(false);
        ui->hsWavelength->setEnabled(false);
        wavelengthTimer -> stop();

    }
    else{
        interbus->openComport(ui->cbComports->currentText());
        checkEmission();
        emissionStateDispUpdate();
        ui->pbSwitchEmission->setEnabled(true);
        ui->hsWavelength->setEnabled(true);
        checkWavelength();
        //wavelengthTimer -> start(500);
    }
}


void MainWindow::comportOpened(QString portname, bool opened)
{
    Q_UNUSED(portname)

    ui->cbComports->setEnabled(!opened);
    if (opened)
        ui->pbOpenClose->setText(tr("Close"));
    else
        ui->pbOpenClose->setText(tr("Open"));
}


void MainWindow::checkEmission(){
    QByteArray readData;
    bool statusSuccess = false;

    RegGetStatusTypes statusReg = interbus->readRegister(devAddress, 0x30, readData);

    switch (statusReg)
        {
        case GetSuccess:
            statusSuccess = true;
            break;
        case GetBusy:           ui->pteStatus->appendPlainText("Read busy"); break;
        case GetNacked:         ui->pteStatus->appendPlainText("Read nacked"); break;
        case GetCRCErr:         ui->pteStatus->appendPlainText("Read CRC error"); break;
        case GetTimeout:        ui->pteStatus->appendPlainText("Read timeout error"); break;
        case GetComError:       ui->pteStatus->appendPlainText("Read communication error"); break;
        case GetPortClosed:     ui->pteStatus->appendPlainText("Read port closed"); break;
        case GetPortNotFound:   ui->pteStatus->appendPlainText("Read port not found"); break;
        }

    if(statusSuccess){
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

void MainWindow::switchEmissionState(){
    qint8 onOffSend;
    if(emissionOn){
        onOffSend = 0x00;
    }
    else{
        onOffSend = 0x01;
    }
    ui->pteStatus->appendPlainText(QString::number(onOffSend));
    QByteArray writeData;

    interbus->SetInt8(writeData, onOffSend, 0);

    RegSetStatusTypes status = interbus->writeRegister(devAddress, 0x30, writeData);
    switch (status)
    {
    case SetSuccess:
        ui->pteStatus->appendPlainText("Write U8 success: " + interbus->GetHexString(writeData, 0));
        break;
    case SetBusy:           ui->pteStatus->appendPlainText("Write U8 busy"); break;
    case SetNacked:         ui->pteStatus->appendPlainText("Write U8 nacked"); break;
    case SetCRCErr:         ui->pteStatus->appendPlainText("Write U8 CRC error"); break;
    case SetTimeout:        ui->pteStatus->appendPlainText("Write U8 timeout error"); break;
    case SetComError:       ui->pteStatus->appendPlainText("Write U8 communication error"); break;
    case SetPortClosed:     ui->pteStatus->appendPlainText("Write U8 port closed"); break;
    case SetPortNotFound:   ui->pteStatus->appendPlainText("Write U8 port not found"); break;
    }

    //emissionOn = !emissionOn;
    checkEmission();
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
        serial->write("OM 0\r\n"); //off
        serial->waitForBytesWritten(50);
        ui->EDFAOnOff->setText("Turn On");
        EDFAOnOff = false;

    }
    else{
        EDFAOnOff = true;
        serial->write("OM 2\r\n");
        serial->waitForBytesWritten(50);
        ui->EDFAOnOff->setText("Turn Off");
    }
}

