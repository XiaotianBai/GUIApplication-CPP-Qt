#include "mythread.h"
#include <QtDebug>
#include <QCoreApplication>
//FOR RS232
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include<Qprocess>
MyThread::MyThread()
{
   stopped=false;

}

void MyThread::run()
{
    QSerialPort *my_serialport= new QSerialPort;

    while(!stopped)
    {
        if(stopped&&com_opened)
        {
            my_serialport->close();
            com_opened=false;

        }
        if(!com_opened)
        {
            //open com
            qDebug() << "Brush:" <<"---open com port------";
            com_opened=true;
            my_serialport->setPortName(portnum);
            my_serialport->open(QIODevice::ReadWrite);
            my_serialport->setBaudRate(portrate.toInt());
            my_serialport->setDataBits(QSerialPort::Data8);
            my_serialport->setParity(QSerialPort::NoParity);
            my_serialport->setStopBits(QSerialPort::OneStop);
            my_serialport->setFlowControl(QSerialPort::NoFlowControl);
            com_opened=true;
        }
        if(this->com_opened&&this->tx_event)
        {
                this->tx_event=false;
                my_serialport->clear(QSerialPort::AllDirections);
                qDebug() << "Brush:" <<"send data to com"<<this->TxData.length();
                qDebug() << "arr size:" <<this->TxData.length();
               my_serialport->write(this->TxData);
              if (my_serialport->waitForBytesWritten(5))
              {
                qDebug() << "Brush:" <<"send data success";
                if (my_serialport->waitForReadyRead(50))  //50ms
                {
                    requestData = my_serialport->readAll();
                    while (my_serialport->waitForReadyRead(15))
                    requestData += my_serialport->readAll();
                    emit(this->comRecive());
                }else
                {
                        qDebug() << "Brush:" <<"wait return time out";
                }
              }else
              {
                  qDebug() << "Brush:" <<"send time out";
              }
        }
        if (my_serialport->waitForReadyRead(5))  //50ms
        {
            while (my_serialport->waitForReadyRead(5))
            this->msleep(20);
            requestData = my_serialport->readAll();
            emit(this->comRecive());
        }
        if(stopped&&com_opened)
        {
            my_serialport->close();
            com_opened=false;
        }
    }
}
void MyThread::runsim()
{
    QProcess *myprocess=new QProcess();
//myprocess->start("C:\sim.exe");
    QString strfile;
    strfile = QCoreApplication::applicationDirPath();
    qDebug()<<strfile;
//ShellExecuteA(NULL,"open","\images\1.exe",NULL,NULL,SW_SHOW);
   QString strPath = strfile+"/sim.exe";
   // QDesktopServices bs;
    //bs.openUrl(QUrl(strPath));
myprocess->startDetached(strPath);
}

void MyThread::kill()
{QProcess *myprocess=new QProcess();
    if(myprocess)
    myprocess->close();
    delete myprocess;
    myprocess = 0;
}

void MyThread::stop()
{
   stopped=true;

}
void MyThread::startCom()
{
   stopped=false;

}
void MyThread::changeComState(bool stat)
 {
        com_opened=stat;
}
void MyThread::setMessage(const QString &message)
{
   messageStr = message;

}
void MyThread::setPortnum(const QString &num)
{
    portnum=num;

}

void MyThread::setPortrate(const QString &rate)
{
    portrate=rate;

}
void MyThread:: changeTxState(bool stat)
{
    tx_event=stat;
}
void MyThread:: changeRxState(bool stat)
{
    rx_event=stat;
}

bool MyThread:: comOpened(void) {
    return com_opened;
}

bool MyThread:: comStoped(void) {
    return stopped;
}
