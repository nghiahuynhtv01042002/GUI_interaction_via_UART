#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>

int count = 0;
QSerialPort* mySerialPort = new QSerialPort;
QString dataOut ;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->setWindowTitle("Serial Control");

    // set comport in laptop appear on combox_Port_Name
    foreach(auto &portInfo, QSerialPortInfo::availablePorts()){
        ui->combox_Port_Name->addItem(portInfo.portName());
    }
    // add Baudrate_value into combox_Baudrate

    QStringList Baudrate_list = {"115200","9600","4800","2400","1200"};
    ui->combox_Baudrate->addItems(Baudrate_list);
    // add databits into comBox_Data_Bits
    QStringList databits_list = {"8","7","6"};
    ui->comBox_Data_Bits->addItems(databits_list);
    // add stopbits into combox_Stop_Bits
    QStringList stopbits_list = {"One","Two"};
    ui->combox_Stop_Bits->addItems(stopbits_list);
    // add Paritybits into combox_Stopbit
    QStringList Parity_list ={"None","Odd","Even"};
    ui->combox_Parity_bits->addItems(Parity_list);
    //enable some button
    ui->btnconnect->setEnabled(true);
    ui->btn_Close->setEnabled(false);
    ui->lbl_Status_Comport->setText("OFF");
    ui->lbl_Status_Comport->setStyleSheet("color: black;background-color: grey;");
    //readata


//    qDebug()<<titleWindow;
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox closemsgBox(QMessageBox::Question,"Question","Do You Want To Exit???",QMessageBox::Yes| QMessageBox::No);
    closemsgBox.setDefaultButton(QMessageBox::No);
    int ret = closemsgBox.exec();
    if(ret ==QMessageBox::No){
        event->ignore();
    }
}


void MainWindow::on_btnconnect_clicked() {
//    ui->list_data->addItem("iam TXN");

//    if(mySerialPort == nullptr){
//        mySerialPort->close();
//        delete mySerialPort;
//    }
    // config serial port with data took from comboxs
    mySerialPort = new QSerialPort(this);
    mySerialPort->setPortName(ui->combox_Port_Name->currentText());
    mySerialPort->setBaudRate(ui->combox_Baudrate->currentText().toInt());
    mySerialPort->setDataBits(static_cast<QSerialPort::DataBits>(ui->comBox_Data_Bits->currentText().toInt()));
    mySerialPort->setStopBits(static_cast<QSerialPort::StopBits>(ui->combox_Stop_Bits->currentText().toInt()));
    mySerialPort->setParity(static_cast<QSerialPort::Parity>(ui->combox_Parity_bits->currentText().toInt()));

    mySerialPort->open(QIODevice::ReadWrite);
    connect(mySerialPort, SIGNAL(readyRead()), this, SLOT(Serial_DataReceived()));
    if(mySerialPort->isOpen()){
        ui->btnconnect->setEnabled(false);
        ui->btn_Close->setEnabled(true);
        ui->lbl_Status_Comport->setText("ON");
        ui->lbl_Status_Comport->setStyleSheet("color: black;background-color: green;");
    }
    else{
       ui->btnconnect->setEnabled(false);
       ui->btn_Close->setEnabled(true);
       ui->lbl_Status_Comport->setText("OFF");
       ui->lbl_Status_Comport->setStyleSheet("color: black;background-color: grey;");
       QMessageBox::warning(this,"Warning","Port is not conected!!");
    }

}

void MainWindow::Mytimer_timeout(){

}

void MainWindow::on_btn_Close_clicked()
{
    if(mySerialPort->isOpen()){
        mySerialPort->close();
        ui->btnconnect->setEnabled(true);
        ui->btn_Close->setEnabled(false);
        ui->lbl_Status_Comport->setText("OFF");
        ui->lbl_Status_Comport->setStyleSheet("color: black;background-color: grey;");
    }
}


void MainWindow::on_btnSend_Data_clicked()
{
    if(mySerialPort->isOpen()){
        dataOut = ui->txtEdit_Send_Data->toPlainText() + "\n";
        mySerialPort->write(dataOut.toUtf8());
        ui->listWidget_data_display->addItem("DATA SEND:");
        ui->listWidget_data_display->addItem(dataOut.trimmed()); //cut off '\n'

    }else{
        QMessageBox::warning(this,"Warning","Port is not conected!!");
    }
}

void MainWindow::on_btnClear_Send_Data_clicked()
{
    ui->txtEdit_Send_Data->clear();
}
void MainWindow::Serial_DataReceived()
{
// If you don't want it to be colorful, just use this code below. :VV;
//    ui->listWidget_data_display->addItem("DATA RECIEVE:");
//    ui->listWidget_data_display->addItem(mySerialPort->readAll());

    QListWidgetItem *item_recieve1 = new QListWidgetItem("DATA RECIEVE:");
    item_recieve1->setForeground(QBrush(Qt::darkGreen)); // Change color here
    ui->listWidget_data_display->addItem(item_recieve1);

    QListWidgetItem *item_recieve2 = new QListWidgetItem(mySerialPort->readAll());
    item_recieve2->setForeground(QBrush(Qt::darkGreen)); // Change color here
    ui->listWidget_data_display->addItem(item_recieve2);

}


void MainWindow::on_btnClearData_Display_clicked()
{
   ui->listWidget_data_display->clear();
}

