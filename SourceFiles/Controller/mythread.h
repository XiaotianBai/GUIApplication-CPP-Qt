#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <stdio.h>
#include <QtGlobal>
#include <QProcess>

class MyThread : public QThread
{
    Q_OBJECT
public:
    QByteArray requestData;
    QByteArray TxData;
    MyThread();
 void runsim();
 void kill();
    void setMessage(const QString &message);
    void setPortnum(const QString &num);
    void setPortrate(const QString &rate);
    void stop();
    void startCom();
    void changeTxState(bool stat);
    void changeRxState(bool stat);
    void changeComState(bool stat);
    bool comOpened(void);
    bool comStoped(void);

signals:
    void request(const QString &s);
    void comRecive();



protected:
    void run();


private:
    QProcess *simprocess;
    QString messageStr;
    QString portnum;
    QString portrate;
    volatile bool com_opened;
    volatile bool stopped;
    volatile bool tx_event;
    volatile bool rx_event;
};

#endif // MYTHREAD_H
