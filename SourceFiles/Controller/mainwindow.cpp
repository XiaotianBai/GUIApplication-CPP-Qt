/*TODO
 *1 读下位机代码动作组导入到标签2
 *2 执行上位机动作组 修改成等待下位机执行OK再执行下一条
 *3 执行下位机动作组 显示  这个要修改下位机代码 动作做完才能做下一条
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qicon>
#include <QMessageBox>
#include <QProcess>
#include <QDateTime>
#include <QString>
#include <QtCore/QDebug>
//FOR RS232
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <windows.h>
#include <QDesktopServices>
#include <QCloseEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //初始化状态栏
    statusBar_init();
    //初始化界面
    this->ui_init();
    //初始化串口线程
    this->threadA.changeTxState(false);
    this->threadA.TxData.clear();
    connect(&this->threadA, SIGNAL(comRecive()), this, SLOT(handleRxData()));
    //初始化舵机
    this->dj_data_init();
    //注册一个定时器控制舵机数据发送和执行
    dj_timer_ctl_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化数据设置
void MainWindow::ui_init(void) {
    // Example use QSerialPortInfo

    int serialIndex = 0, i=0;
    QString serialName;

    //设置app图标
   setWindowIcon(QIcon("./images/app.ico"));

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite) && info.description().contains("USB-to-Serial",Qt::CaseSensitive))
        {
           ui->cmbSeriaNum->addItem(info.portName());
           serial.close();
           i++;
        }

        if(info.description().contains("USB-to-Serial",Qt::CaseSensitive)) {
            serialIndex =  i;
            serialName = info.portName();
            qDebug() << "contains: USB-to-Serial" << QString::number(serialIndex);
        }

    }
    //设置显示的串口号
    if(serialIndex) {
        ui->cmbSeriaNum->setCurrentIndex(serialIndex);
        ui->txtCmd->setText(QStringLiteral("Detected:") + serialName + QStringLiteral("Is the USB-to-serial Connection"));
    } else {
        ui->txtCmd->setText(QStringLiteral("USB-to-serial Connection Undetected"));
    }
    flag_serial_open = false;

    //波特率显示设置
    QStringList rateList;
    rateList << "600" <<"1200" << "2400" << "4800" << "9600"
                << "14400" << "19200" << "28800" << "38400"
                   << "57600" << "115200" << "230400" << "460800";
    ui->cmbRate->addItems(rateList);
    ui->cmbRate->setCurrentIndex(4);

    //动作组测试加入item
    ui->listWidget->addItem("{G0}");

    //多标签集合测试
    ui->tabWidget->setTabText(0, QStringLiteral("Action List"));

    //默认选取上位机
  //  ui->radioButtonUp->setChecked(true);

    //检测串口初始化
//serial_monitor_init();

}
//串口监控初始化
void MainWindow::serial_monitor_init(void) {
//    int m_fd = serial.open(QIODevice::ReadWrite);
//    if (m_fd < 0){
//        QMessageBox::warning(this, tr("Error"), tr("Fail to open serial port!"));
//        return ;
//    }
//    m_notifier = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
//    connect (m_notifier, SIGNAL(activated(int)), this, SLOT(serial_monitor()));
}

void MainWindow::serial_monitor(void) {
//    ui->txtCmd->setText("Detected Port!");
}

//状态栏初始化
//状态栏初始化
void MainWindow::statusBar_init(void) {
    statusBarLabel = new QLabel();
    statusBar()->addWidget(statusBarLabel);
    statusBarLabel->setText(QStringLiteral("Ready"));
}

//处理接收数据显示在命令接收栏上面
void MainWindow::handleRxData(void) {
    qDebug() << "Reveicing Data!";
    QString str_get = get_uart_str();
    qDebug() << "Data Received!";
    if(dj_action_save || dj_action_do) {
        ui->txtCmd->append(str_get);

        if(dj_action_do) { //针对执行下位机 处理执行下位机返回值 特别是次数执行时
            if(str_get.contains("do_group_success_times",Qt::CaseSensitive)
            || str_get.contains("do_all_success_times",Qt::CaseSensitive)) {
             //   if(ui->radioButtonDn->isChecked()) {
                    ui->pbtnDoAction->setText(QStringLiteral("Execute"));
                    dj_action_do = false;
             //   }
            }
        }

    } else {
        ui->txtCmd->append(str_get);
    }

    if(flag_dj_rd_action) {
        handleRdActionMessageBox("Get Actions", str_get);
        flag_dj_rd_action = false;
    } else if (flag_dj_rd_sector) {
        handleRdSectorMessageBox("Save Information", str_get);
        flag_dj_rd_sector = false;

    }
}


//串口打开关闭按钮
void MainWindow::on_pbtnOpen_clicked()
{
    if(!flag_serial_open) {
        if(ui->cmbSeriaNum->currentIndex()!=-1) {
            //start recive thread
            threadA.startCom();
            threadA.setPortnum(ui->cmbSeriaNum->currentText());
            threadA.setPortrate(ui->cmbRate->currentText());
            threadA.changeComState(false);
            threadA.start();
            ui->txtCmd->append(QStringLiteral("Port Successfully Opened"));
            ui->pbtnOpen->setText(QStringLiteral("Close Port"));\
            flag_serial_open = true;
        } else {
            QMessageBox::information(this,QStringLiteral("Warning"),QStringLiteral("Port is not selected or detected"),QMessageBox::Yes);
        }


    } else {
        threadA.stop();
        ui->pbtnOpen->setText(QStringLiteral("Open Port"));
        ui->txtCmd->append(QStringLiteral("Port Successfully Closed"));
        flag_serial_open = false;
    }

}

//命令发送按钮
void MainWindow::on_pbtnSend_clicked() {
    if(ui->cmbSeriaNum->currentText() != "") {
        this->send_uart_str(ui->edtCmdSend->text());
    }
    else {
        QMessageBox::information(this,QStringLiteral("Warning"),QStringLiteral("Failed: No Connection!"),QMessageBox::Yes);
    }

}
void MainWindow::closeEvent(QCloseEvent *event)
{
    switch( QMessageBox::information( this, QStringLiteral("Warning"),
      QStringLiteral("Do you want to quit?"),
      QStringLiteral("Yes"), QStringLiteral("No"),
      0, 1 ) )
      {
       case 0:
       event->accept();
          break;
       case 1:
       default:
          event->ignore();
          break;
      }
 }
void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void MainWindow::on_SimBtn_clicked() {
//threadA.runsim();

     QString strfile;
     strfile = QCoreApplication::applicationDirPath();
     qDebug()<<strfile;
 //ShellExecuteA(NULL,"open","\images\1.exe",NULL,NULL,SW_SHOW);
    QString strPath = strfile+"/Arm_Simulator.exe";
    // QDesktopServices bs;
     //bs.openUrl(QUrl(strPath));
    process->startDetached(strPath);

}

//list动作组按钮点击事件
void MainWindow::on_listWidget_clicked(const QModelIndex &index) {
    QString temp = ui->listWidget->currentItem()->text();
    dj_group_index_cur = ui->listWidget->currentRow();              //获取当前点击的item的位置

    dj_group_to_dj_single(temp);   //把item中的动作组值更新到控件中
    send_uart_str(temp);     //发送到串口
}

//串口发送数据
void MainWindow::send_uart_str(QString str) {
    if(this->threadA.comOpened()) {
        //qDebug() << "send success!";
        this->threadA.TxData.clear();
        this->threadA.TxData.append(str);
        this->threadA.changeTxState(true);
        ui->edtCmdSend->setText(str);     //显示到命令行
    } else {
        qDebug() << "send failure!";
    }
}

//串口接收数据
QString MainWindow::get_uart_str(void) {
    char *buf;
    if(threadA.requestData.size()>0) {
        seria_str.clear();
        buf=threadA.requestData.data();
        qDebug() << "receive num:" <<threadA.requestData.size();
        seria_str = QString(QLatin1String(buf));
        threadA.requestData.clear();
        return seria_str;
    }
    return NULL;
}


//把从list中读取的动作组更新到各个控件和数组中并发送出去
void MainWindow::dj_group_to_dj_single(QString str) {
    //解析str到dj_single数组并且更新到控件 涉及到某些动作窗口是否有效 TODO
    int len = str.length(), i=0;
    //unsigned char *djg_char = (unsigned char*)str.toLatin1().data();
    QByteArray djg_char=str.toLocal8Bit();
    int group = 0, index = 0, pwm = 0,  time = 0;

    //更新控件和数组
    dj_single_clear();          //清除舵机控制组中的相关变量
    qDebug() <<QStringLiteral("Clear Actions:") + dj_single[0].dj_single_s
            + dj_single[1].dj_single_s
            + dj_single[2].dj_single_s
            + dj_single[3].dj_single_s
            + dj_single[4].dj_single_s
            + dj_single[5].dj_single_s;
    qDebug() << "The Action:" + str + "Length:" + QString::number(len);
    while(djg_char[i] != '}' && i<len) {
        if(djg_char[i] == 'G') {
            i++;
            while(djg_char[i] != '#' && i<len){
                group = 10*group + (djg_char[i++] - '0');
            }
        } else if(djg_char[i] == '#') {
            i++;
            while(djg_char[i] != 'P' && i<len){
                index = 10*index + (djg_char[i++] - '0');
            }
        } else if(djg_char[i] == 'P') {
            i++;
            while(djg_char[i] != 'T' && i<len){
                pwm = 10*pwm + (djg_char[i++] - '0');
            }
        } else if(djg_char[i] == 'T') {
            i++;
            while(djg_char[i] != '!' && i<len){
                time = 10*time + (djg_char[i++] - '0');
            }

            dj_single[index].changed = true;
            dj_single[index].pwm = pwm;
            dj_single[index].time = time;
            dj_single[index].dj_single_s = combine_dj_single(index, pwm, time);
            qDebug() << "Analyzing Number:" + QString::number(index);
            djSingleupToKongJian(index, pwm, time);//更新到控件
            index = pwm = time = 0;
        }  else i++;
    }
    //重新赋值原来的值 防止突变
    ui->listWidget->item(dj_group_index_cur)->setText(str);

    qDebug() << "Outgoing Grouped Action" + dj_single[0].dj_single_s
            + dj_single[1].dj_single_s
            + dj_single[2].dj_single_s
            + dj_single[3].dj_single_s
            + dj_single[4].dj_single_s
            + dj_single[5].dj_single_s;


}

//TODO 更新到控件
void MainWindow::djSingleupToKongJian(int index, int pwm, int time) {
    switch(index) {
        case               0:
            ui ->pwmSpinBox0->setValue(pwm);
            ui     ->pwmBar0->setValue(pwm);
            ui->timeSpinBox0->setValue(time);
            ui    ->timeBar0->setValue(time);
        break;
        case               1:
            ui ->pwmSpinBox1->setValue(pwm);
            ui     ->pwmBar1->setValue(pwm);
            ui->timeSpinBox1->setValue(time);
            ui    ->timeBar1->setValue(time);
        break;
        case               2:
            ui ->pwmSpinBox2->setValue(pwm);
            ui     ->pwmBar2->setValue(pwm);
            ui->timeSpinBox2->setValue(time);
            ui    ->timeBar2->setValue(time);
        break;
        case               3:
            ui ->pwmSpinBox3->setValue(pwm);
            ui     ->pwmBar3->setValue(pwm);
            ui->timeSpinBox3->setValue(time);
            ui    ->timeBar3->setValue(time);
        break;
        case               4:
            ui ->pwmSpinBox4->setValue(pwm);
            ui     ->pwmBar4->setValue(pwm);
            ui->timeSpinBox4->setValue(time);
            ui    ->timeBar4->setValue(time);
        break;
        case               5:
            ui ->pwmSpinBox5->setValue(pwm);
            ui     ->pwmBar5->setValue(pwm);
            ui->timeSpinBox5->setValue(time);
            ui    ->timeBar5->setValue(time);
        break;
    }
}

//处理从舵机调试区域得到的数据
void MainWindow::pwmBarHandle(int index, int pwm, int time) {

    dj_single[index].changed = true;    //标记改变

    //发送单个命令
    send_uart_str(dj_single[index].dj_single_s);

    combine_dj_single_all();//先检查最小舵机命令模块有效性并进行组合
    QString dj_group_str  = combine_dj_group();//组合动作组

    //组装到item中
    ui->listWidget->item(dj_group_index_cur)->setText(dj_group_str);
}

//组合所有有效舵机控制单元最小命令
void MainWindow::combine_dj_single_all(void) {
    int i, pwm, time;
    for(i=0; i<6; i++) {
        if(dj_single[i].valid) {
            switch(i) {
                case 0:pwm = ui->pwmBar0->value();time = ui->timeBar0->value();break;
                case 1:pwm = ui->pwmBar1->value();time = ui->timeBar1->value();break;
                case 2:pwm = ui->pwmBar2->value();time = ui->timeBar2->value();break;
                case 3:pwm = ui->pwmBar3->value();time = ui->timeBar3->value();break;
                case 4:pwm = ui->pwmBar4->value();time = ui->timeBar4->value();break;
                case 5:pwm = ui->pwmBar5->value();time = ui->timeBar5->value();break;
            }
            dj_single[i].pwm = pwm;
            dj_single[i].time = time;
            dj_single[i].dj_single_s = combine_dj_single(i, dj_single[i].pwm, dj_single[i].time);
        }
    }
}

//组合舵机做小命令单元
QString MainWindow::combine_dj_single(int index, int pwm, int time) {
    return "#" + QString::number(index)+
           "P" +QString::number(pwm)+
           "T" +QString::number(time)+
           "!";
}

//组合动作组命令单元
QString MainWindow::combine_dj_group(void) {
    int i;
    dj_group.dj_group_s = "{G" + QString::number(dj_group_index_cur);

    unsigned char valid_byte = 0;

    //调试有效窗口
    for(i=0; i<8; i++) {
        if(dj_single[i].valid) {
           valid_byte |= 1 << i;
        }
    }

    if(dj_combine_style || dj_group_index_cur == 0) {   //全组模式
        for(i=0;i<6;i++) {
            if(dj_single[i].valid) {                //有效
                dj_group.dj_group_s += dj_single[i].dj_single_s;
            }
        }
        //qDebug() << "全组模式" << QString::number((int)(valid_byte));
    } else {                                        //散组模式
        for(i=0;i<6;i++) {
            if(dj_single[i].valid && dj_single[i].changed) {    //有效且数据有改变
                dj_group.dj_group_s += dj_single[i].dj_single_s;
            }
        }
        //qDebug() << "散组模式" << QString::number((int)(valid_byte));
    }
    dj_group.dj_group_s += "}";
    return dj_group.dj_group_s;
}

//舵机数据初始化
void MainWindow::dj_data_init(void) {
    int i;
    for(i=0;i<8;i++) {
        dj_single[i].valid = false;
        dj_single[i].changed = false;
        dj_single[i].dj_single_s = "";
    }
    dj_group.dj_group_s = "";
    dj_group_index_all = 1;
    dj_group_index_cur = 0;
    dj_combine_style = false;
    dj_action_do = false;
    dj_action_save = false;
    flag_dj_rd_action = false;
    flag_dj_rd_sector = false;
}

//清楚dj_single数组中的所有数据
void MainWindow::dj_single_clear(void) {
    int i;
    for(i=0;i<8;i++) {
        dj_single[i].changed = false;
        dj_single[i].dj_single_s = "";
    }
}


//舵机调试区控件事件
void MainWindow::on_pwmBar0_valueChanged(int value) {
    ui->pwmSpinBox0->setValue(value);
        this->pwmBarHandle(0, ui->pwmBar0->value(), ui->timeBar0->value());
}

void MainWindow::on_pwmSpinBox0_valueChanged(int arg1) {
    ui->pwmBar0->setValue(arg1);
     this->pwmBarHandle(0, ui->pwmBar0->value(), ui->timeBar0->value());
}

void MainWindow::on_timeBar0_valueChanged(int value)
{
    ui->timeSpinBox0->setValue(value);
        this->pwmBarHandle(0, ui->pwmBar0->value(), ui->timeBar0->value());
}

void MainWindow::on_timeSpinBox0_valueChanged(int arg1)
{
      ui->timeBar0->setValue(arg1);
    this->pwmBarHandle(0, ui->pwmBar0->value(), ui->timeBar0->value());
}

//--------------------------------------------
void MainWindow::on_pwmBar1_valueChanged(int value) {
    ui->pwmSpinBox1->setValue(value);
       this->pwmBarHandle(1, ui->pwmBar1->value(), ui->timeBar1->value());
}

void MainWindow::on_pwmSpinBox1_valueChanged(int arg1) {  
    ui->pwmBar1->setValue(arg1);
    this->pwmBarHandle(1, ui->pwmBar1->value(), ui->timeBar1->value());
}

void MainWindow::on_timeBar1_valueChanged(int value)
{
    ui->timeSpinBox1->setValue(value);
        this->pwmBarHandle(1, ui->pwmBar1->value(), ui->timeBar1->value());
}

void MainWindow::on_timeSpinBox1_valueChanged(int arg1)
{
    ui->timeBar1->setValue(arg1);

    this->pwmBarHandle(1, ui->pwmBar1->value(), ui->timeBar1->value());
}

//--------------------------------------------
void MainWindow::on_pwmBar2_valueChanged(int value) {

    ui->pwmSpinBox2->setValue(value);
        this->pwmBarHandle(2, ui->pwmBar2->value(), ui->timeBar2->value());
}

void MainWindow::on_pwmSpinBox2_valueChanged(int arg1) {

    ui->pwmBar2->setValue(arg1);
        this->pwmBarHandle(2, ui->pwmBar2->value(), ui->timeBar2->value());
}

void MainWindow::on_timeBar2_valueChanged(int value)
{

    ui->timeSpinBox2->setValue(value);
      this->pwmBarHandle(2, ui->pwmBar2->value(), ui->timeBar2->value());
}

void MainWindow::on_timeSpinBox2_valueChanged(int arg1)
{

    ui->timeBar2->setValue(arg1);
        this->pwmBarHandle(2, ui->pwmBar2->value(), ui->timeBar2->value());
}

//--------------------------------------------
void MainWindow::on_pwmBar3_valueChanged(int value) {

    ui->pwmSpinBox3->setValue(value);
        this->pwmBarHandle(3, ui->pwmBar3->value(), ui->timeBar3->value());
}

void MainWindow::on_pwmSpinBox3_valueChanged(int arg1) {

    ui->pwmBar3->setValue(arg1);
      this->pwmBarHandle(3, ui->pwmBar3->value(), ui->timeBar3->value());
}

void MainWindow::on_timeBar3_valueChanged(int value)
{

    ui->timeSpinBox3->setValue(value);
       this->pwmBarHandle(3, ui->pwmBar3->value(), ui->timeBar3->value());
}

void MainWindow::on_timeSpinBox3_valueChanged(int arg1)
{

    ui->timeBar3->setValue(arg1);
        this->pwmBarHandle(3, ui->pwmBar3->value(), ui->timeBar3->value());
}

//--------------------------------------------
void MainWindow::on_pwmBar4_valueChanged(int value) {

    ui->pwmSpinBox4->setValue(value);
        this->pwmBarHandle(4, ui->pwmBar4->value(), ui->timeBar4->value());
}

void MainWindow::on_pwmSpinBox4_valueChanged(int arg1) {

    ui->pwmBar4->setValue(arg1);
        this->pwmBarHandle(4, ui->pwmBar4->value(), ui->timeBar4->value());
}

void MainWindow::on_timeBar4_valueChanged(int value)
{

    ui->timeSpinBox4->setValue(value);
        this->pwmBarHandle(4, ui->pwmBar4->value(), ui->timeBar4->value());
}

void MainWindow::on_timeSpinBox4_valueChanged(int arg1)
{

    ui->timeBar4->setValue(arg1);
        this->pwmBarHandle(4, ui->pwmBar4->value(), ui->timeBar4->value());
}

//--------------------------------------------
void MainWindow::on_pwmBar5_valueChanged(int value) {

    ui->pwmSpinBox5->setValue(value);
        this->pwmBarHandle(5, ui->pwmBar5->value(), ui->timeBar5->value());
}

void MainWindow::on_pwmSpinBox5_valueChanged(int arg1) {

    ui->pwmBar5->setValue(arg1);
        this->pwmBarHandle(5, ui->pwmBar5->value(), ui->timeBar5->value());
}

void MainWindow::on_timeBar5_valueChanged(int value)
{

    ui->timeSpinBox5->setValue(value);
        this->pwmBarHandle(5, ui->pwmBar5->value(), ui->timeBar5->value());
}

void MainWindow::on_timeSpinBox5_valueChanged(int arg1)
{

    ui->timeBar5->setValue(arg1);
        this->pwmBarHandle(5, ui->pwmBar5->value(), ui->timeBar5->value());
}

//--------------------------------------------
  /*  void MainWindow::keyPressEvent( QKeyEvent* e )
    { if (e->key() == Qt::Key_A && !e->isAutoRepeat())
        {
            if (!mPressFlag_A)
            {
                 mKeyDirection=-1;
  ui->pwmSpinBox5->setValue(ui->pwmSpinBox5->value()-1);


            }

        }
       else if(e->key() == Qt::Key_A && e->isAutoRepeat()&&(ui->pwmSpinBox5->value()>100))
        {  mKeyDirection=-1;
         keypress();

        }
          mPressFlag_A = true;
}
  void MainWindow::keypress(void)
  {
  ui->pwmSpinBox5->setValue(ui->pwmSpinBox5->value()-1);

  }
*/
void MainWindow::keyPressEvent( QKeyEvent* e )
{
    switch(e->key())
    {case Qt::Key_A: ui->pwmSpinBox0->setValue(ui->pwmSpinBox0->value()+20);
        this->pwmBarHandle(0, ui->pwmBar0->value(), ui->timeBar0->value());
        break;
     case Qt::Key_D: ui->pwmSpinBox0->setValue(ui->pwmSpinBox0->value()-20);
        this->pwmBarHandle(0, ui->pwmBar0->value(), ui->timeBar0->value());
        break;
     case Qt::Key_W: ui->pwmSpinBox2->setValue(ui->pwmSpinBox2->value()+20);
        this->pwmBarHandle(2, ui->pwmBar2->value(), ui->timeBar2->value());
        break;
     case Qt::Key_S: ui->pwmSpinBox2->setValue(ui->pwmSpinBox2->value()-20);
        this->pwmBarHandle(2, ui->pwmBar2->value(), ui->timeBar2->value());
        break;
     case Qt::Key_Up:ui->pwmSpinBox1->setValue(ui->pwmSpinBox1->value()+20);
        this->pwmBarHandle(1, ui->pwmBar1->value(), ui->timeBar1->value());
        break;
    case Qt::Key_Down:ui->pwmSpinBox1->setValue(ui->pwmSpinBox1->value()-20);
       this->pwmBarHandle(1, ui->pwmBar1->value(), ui->timeBar1->value());
        break;
    case Qt::Key_4:ui->pwmSpinBox3->setValue(ui->pwmSpinBox3->value()-20);
       this->pwmBarHandle(3, ui->pwmBar3->value(), ui->timeBar3->value());
        break;
    case Qt::Key_6:ui->pwmSpinBox3->setValue(ui->pwmSpinBox3->value()+20);
       this->pwmBarHandle(3, ui->pwmBar3->value(), ui->timeBar3->value());
        break;
    case Qt::Key_8:ui->pwmSpinBox4->setValue(ui->pwmSpinBox4->value()-20);
       this->pwmBarHandle(4, ui->pwmBar4->value(), ui->timeBar4->value());
        break;
    case Qt::Key_2:ui->pwmSpinBox4->setValue(ui->pwmSpinBox4->value()+20);
       this->pwmBarHandle(4, ui->pwmBar4->value(), ui->timeBar4->value());
        break;
    }
}
    void MainWindow::keyReleaseEvent( QKeyEvent* e )
    {  if (e->key() == Qt::Key_A&&  mKeyDirection==-1&&!e->isAutoRepeat())
        {
            if (mPressFlag_A)
            {
               //ui->timeSpinBox5->setValue(1000);
                mKeyDirection=0;

            }
              mPressFlag_A = false;
        }
}


void MainWindow::on_timeSliderAll_valueChanged(int value)
{
    ui->timeSpinBoxAll->setValue(value);
    ui->timeSpinBox5->setValue(value);
    ui    ->timeBar5->setValue(value);
    ui->timeSpinBox4->setValue(value);
    ui    ->timeBar4->setValue(value);
    ui->timeSpinBox3->setValue(value);
    ui    ->timeBar3->setValue(value);
    ui->timeSpinBox2->setValue(value);
    ui    ->timeBar2->setValue(value);
    ui->timeSpinBox1->setValue(value);
    ui    ->timeBar1->setValue(value);
    ui->timeSpinBox0->setValue(value);
    ui    ->timeBar0->setValue(value);
}

void MainWindow::on_timeSpinBoxAll_valueChanged(int arg1)
{
    ui->timeSliderAll->setValue(arg1);
    ui->timeSpinBox5->setValue(arg1);
    ui    ->timeBar5->setValue(arg1);
    ui->timeSpinBox4->setValue(arg1);
    ui    ->timeBar4->setValue(arg1);
    ui->timeSpinBox3->setValue(arg1);
    ui    ->timeBar3->setValue(arg1);
    ui->timeSpinBox2->setValue(arg1);
    ui    ->timeBar2->setValue(arg1);
    ui->timeSpinBox1->setValue(arg1);
    ui    ->timeBar1->setValue(arg1);
    ui->timeSpinBox0->setValue(arg1);
    ui    ->timeBar0->setValue(arg1);
}


void MainWindow::on_pwmFocus_clicked()
{
    ui->pwmSpinBox5->setValue(1500);
    ui    ->pwmBar5->setValue(1500);
    ui->pwmSpinBox4->setValue(1500);
    ui    ->pwmBar4->setValue(1500);
    ui->pwmSpinBox3->setValue(1500);
    ui    ->pwmBar3->setValue(1500);
    ui->pwmSpinBox2->setValue(1500);
    ui    ->pwmBar2->setValue(1500);
    ui->pwmSpinBox1->setValue(1500);
    ui    ->pwmBar1->setValue(1500);
    ui->pwmSpinBox0->setValue(1500);
    ui    ->pwmBar0->setValue(1500);
}

//全组和散组切换
void MainWindow::on_chkDjStyle_toggled(bool checked)
{
    if(checked) {
        ui->chkDjStyle->setText(QStringLiteral("Record Status of Idle Servos"));
        dj_combine_style = true;
    } else {
        ui->chkDjStyle->setText(QStringLiteral("Record Status of Idle Servos"));
        dj_combine_style = false;
    }
}

//增加动作组按钮
void MainWindow::on_pbtnAddGroup_clicked()
{
    dj_group_index_all ++;
    ui->listWidget->addItem("{G"+ QString::number(dj_group_index_all-1) +"}");
    dj_group_index_cur = dj_group_index_all-1;
    dj_single_clear();
}

//删除动作组按钮
void MainWindow::on_pbtnDelGroup_clicked()
{
    //TODO 这里涉及到删除后要排序所有的group
    int i;
    QString temp;
    if(dj_group_index_all == 1) {
        ui->listWidget->item(0)->setText("{G0}");
    } else {
        //删除item
        if(ui->listWidget->currentItem()!=Q_NULLPTR)
        {
            QListWidgetItem * item = ui->listWidget->takeItem(ui->listWidget->currentRow());
            delete item;
        }

        if(dj_group_index_cur == 0) {
            dj_group_index_cur += 1;//=0?
        } else {
            dj_group_index_cur -= 1;
        }
        dj_group_index_all --;

        //if(dj_group_index_cur != dj_group_index_all-1) {//如果删除的是最后一项
            for(i=0;i<dj_group_index_all;i++) {
                ui->listWidget->item(i)->setText(djGroupChangeIndex(ui->listWidget->item(i)->text(), i));
            }
        //}
    }
}

//动作组更换组号
QString MainWindow::djGroupChangeIndex(QString str, int index) {
 /*   int len = str.length(), i=0;
    char *djg_char = str.toLatin1().data();
    int group = 0;
    while(djg_char[i] != '}' && i<len && len>13) {
        if(djg_char[i] == 'G') {
            i++;
            while(djg_char[i] != '#' && i<len){
                group = 10*group + (djg_char[i++] - '0');
            }
            return "{G" + QString::number(index) + QString(QLatin1String(djg_char+i));
        } else i++;
    }
    return ("{G" + QString::number(index) + "}");
    */
    str.replace (2, 1,QString::number(index));
    return str;



}

void MainWindow::on_gbox0_toggled(bool arg1)
{
    if(arg1) {
        dj_single[0].valid = true;
    } else {
        dj_single[0].valid = false;
    }
}

void MainWindow::on_gbox1_toggled(bool arg1)
{
    if(arg1) {
        dj_single[1].valid = true;
    } else {
        dj_single[1].valid = false;
    }
}

void MainWindow::on_gbox2_toggled(bool arg1)
{
    if(arg1) {
        dj_single[2].valid = true;
    } else {
        dj_single[2].valid = false;
    }
}

void MainWindow::on_gbox3_toggled(bool arg1)
{
    if(arg1) {
        dj_single[3].valid = true;
    } else {
        dj_single[3].valid = false;
    }
}

void MainWindow::on_gbox4_toggled(bool arg1)
{
    if(arg1) {
        dj_single[4].valid = true;
    } else {
        dj_single[4].valid = false;
    }
}

void MainWindow::on_gbox5_toggled(bool arg1)
{
    if(arg1) {
        dj_single[5].valid = true;
    } else {
        dj_single[5].valid = false;
    }
}

void MainWindow::handleRdSectorMessageBox(QString title, QString content) {
    messageBox = new QMessageBox();
    messageBox->setIcon(QMessageBox::Information);
    messageBox->setWindowTitle(title);
    messageBox->setText(content);
    QPushButton *Button1 = messageBox->addButton(tr("OK"),QMessageBox::RejectRole);
    messageBox->exec();
}


void MainWindow::handleRdActionMessageBox(QString title, QString content) {
    messageBox = new QMessageBox();
    //messageBox->setIcon(QMessageBox::Warning);
    messageBox->setIcon(QMessageBox::Information);
    messageBox->setWindowTitle(title);
    //messageBox->setText(content);
    //messageBox->setFixedWidth(1500);
    //messageBox->setFixedHeight(500);

    QTextBrowser *txtBrower = new QTextBrowser();
    txtBrower->setText(content);
    dynamic_cast< QGridLayout *>(messageBox->layout())->addWidget(txtBrower, 0, 1);

    QPushButton *Button1 = messageBox->addButton(QStringLiteral("Overwrite"),QMessageBox::AcceptRole);
    QPushButton *Button2 = messageBox->addButton(QStringLiteral("Cancel"),QMessageBox::RejectRole);
    QPushButton *Button3 = messageBox->addButton(QStringLiteral("Append"),QMessageBox::YesRole);
//  QPushButton *Button4 = messageBox->addButton(tr("No"),QMessageBox::NoRole);

    messageBox->exec();

    if (messageBox->clickedButton() == Button1){
        ui->txtCmd->append(QStringLiteral("Overwrite"));

        import_DJG_cover(content);
    } else if (messageBox->clickedButton() == Button2){
        ui->txtCmd->append(QStringLiteral("Cancel"));

    } else if (messageBox->clickedButton() == Button3){
        ui->txtCmd->append(QStringLiteral("Append"));

        import_DJG_append(content);
    }
}

void MainWindow::import_DJG_cover(QString str) {
    int len = str.length(), i=0, j=0;
    QByteArray djg_char=str.toLocal8Bit();
    QByteArray temp_djg;
    QString str_temp;

    temp_djg.resize(128);

    //移除所有动作组
    for(j=0;j<dj_group_index_all;j++) {
        ui->listWidget->removeItemWidget(ui->listWidget->item(j));
    }
    dj_group_index_all = 0;
    dj_group_index_cur = 0;

    while(djg_char[i] != '>' && i<len) {
        j = 0;
        if(djg_char[i] == '{') {
            temp_djg[j++] = djg_char[i++];
            while(djg_char[i] != '}') {
                temp_djg[j++] = djg_char[i++];
            }
            temp_djg[j++] = djg_char[i];
            temp_djg[j] = '\0';
            str_temp=QByteArray(temp_djg);
            if(dj_group_index_all == 0) {
                ui->listWidget->item(0)->setText(temp_djg);
            } else {
                ui->listWidget->addItem(str_temp);
            }
            dj_group_index_all++;

//            ui->txtCmd->append("===================");
//            ui->txtCmd->append(str_temp);
//            ui->txtCmd->append("===================");
        } else i++;
    }

}

void MainWindow::import_DJG_append(QString str) {
    int len = str.length(), i=0, j=0;
    QByteArray djg_char=str.toLocal8Bit();
    QByteArray temp_djg;
    QString str_temp;

    temp_djg.resize(128);

    while(djg_char[i] != '>' && i<len) {
        j = 0;
        if(djg_char[i] == '{') {
            temp_djg[j++] = djg_char[i++];
            while(djg_char[i] != '}') {
                temp_djg[j++] = djg_char[i++];
            }
            temp_djg[j++] = djg_char[i];
            temp_djg[j] = '\0';
            str_temp=QByteArray(temp_djg);
            str_temp = djGroupChangeIndex(str_temp, dj_group_index_all);
            ui->listWidget->addItem(str_temp);
            dj_group_index_all++;

//            ui->txtCmd->append("===================");
//            ui->txtCmd->append(str_temp);
//            ui->txtCmd->append("===================");
        } else i++;
    }
}

//读下位机的扇区信息
void MainWindow::on_pbtnRdDnIn_clicked()
{
    //handleRdActionMessageBox("提示", "$DRS!正在处理中");
    send_uart_str("$DRS!");
    flag_dj_rd_sector = true;
}

//读动作组
void MainWindow::on_pbtnRdDnAc_clicked()
{    for(int i=0;i<dj_group_index_all;i++) {
        QListWidgetItem * item = ui->listWidget->takeItem(0);
        delete item;
    }
    dj_group_index_all=0;
     dj_group_index_cur=0;
    QFile file("Solutions.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        str = str.left(str.length() - 2);
        dj_group_index_all ++;
        ui->listWidget->addItem("{G"+ QString::number(dj_group_index_all-1)+str +"}");
        dj_group_index_cur = dj_group_index_all-1;
        dj_single_clear();
        qDebug()<< str;
    }
}

//舵机复位
void MainWindow::on_pbtnEaDnAc_clicked()
{
    //handleRdActionMessageBox("提示", "$DEA!正在处理中");
    send_uart_str("{G0#0P1500T500!#1P1500T500!#2P1500T500!#3P1500T500!#4P1500T500!#5P1500T500!}");
}

//执行动作组
void MainWindow::on_pbtnDoAction_clicked() {
    if(dj_action_do == false) {
        dj_action_start = ui->spinBoxDjStart->value();
        dj_action_end   = ui->spinBoxDjEnd->value();
        dj_action_time  = ui->spinBoxDoTimes->value();


        //判断是不是全部循环 针对上位机执行动作组
        if(dj_action_start==0 && dj_action_end==0) {
            dj_action_end = dj_group_index_all-1;
        }

        //针对下位机是否循环
   /*     if(ui->spinBoxDoTimes->value() == 0 && ui->radioButtonDn->isChecked()) {
            send_uart_str("$DOC!");//设置循环发送
            qDebug() << "发送$DOC!";
        } else*/
        {
            send_uart_str("$DOS!");//设置次数发送
            qDebug() << "发送$DOS!";
        }

        ui->pbtnDoAction->setText(QStringLiteral("Stop"));
        dj_action_do = true;
    } else {
        ui->pbtnDoAction->setText(QStringLiteral("Execute"));
        dj_action_do = false;

        //针对执行下位机动作组
        send_uart_str("$DOS!");//设置次数发送
        send_uart_str("$DST!");//停止命令
    }
    dj_action_do_dn_times = true;
}

//获取动作组中执行时间最长的时间
int MainWindow::getMaxTimeOfDjAction(QString str) {
    QString str_tem;
    int time_tem;
    char *djg_char = str.toLatin1().data();
    int i = 0, len = str.length(), max_time_temp = 20, cur_time_temp = 0;

    while(djg_char[i] != '}' && i<len) {
        if(djg_char[i] == 'T') {
            i++;
            while(djg_char[i] != '!' && i<len){
                str_tem=QString(djg_char[i]);
                time_tem=str_tem.toInt();
                cur_time_temp = 10*cur_time_temp + time_tem;
                i++;
            }

            if(cur_time_temp > max_time_temp) {
                max_time_temp = cur_time_temp;
            }
            cur_time_temp = 0;
        }  else i++;
    }

    return max_time_temp;
}
int MainWindow::getMaxTimeOfDjAction1(QString str) {
    QString str_tem;
    int time_tem;
    char* djg_char;
    QByteArray ba = str.toLatin1();
    djg_char=ba.data();
    int i = 0, len = str.length(), max_time_temp = 20, cur_time_temp = 0;
    while(djg_char[i] != 'T')
    {i++;}


                while(djg_char[i] != '!' ){
                    str_tem=QString(djg_char[i]);
                    time_tem=str_tem.toInt();
                    cur_time_temp = 10*cur_time_temp + time_tem;
                    i++;
                }



                if(cur_time_temp > max_time_temp) {
                    max_time_temp = cur_time_temp;
                }

      return max_time_temp;
}











//定时器初始化
void MainWindow::dj_timer_ctl_init(void) {
    dj_timer_ctl = new QTimer(this);
    connect( dj_timer_ctl,SIGNAL(timeout()), this, SLOT(dj_timer_ctl_timerOut()) );
    dj_timer_ctl->start(1000);
}

//定时器信号槽 TODO
void MainWindow::dj_timer_ctl_timerOut(void) {
    QString dj_str_temp, saveStr;
    int max_time;
    static int i = 0;
    if(dj_action_do) {//执行动作组
       // if(ui->radioButtonUp->isChecked()) {//上位机执行
            if(dj_action_start <= dj_action_end) {
                dj_str_temp = ui->listWidget->item(dj_action_start)->text();
                send_uart_str(dj_str_temp);
                max_time = getMaxTimeOfDjAction1(dj_str_temp);
                qDebug()<<max_time;
                sleep(max_time);
                dj_action_start++;
                qDebug() << "DO"+dj_str_temp;
            } else {
                if(ui->spinBoxDoTimes->value() == 0) {
                    max_time = 20;
                } else {
                    dj_action_time --;

                    if(dj_action_time == 0) {
                        ui->edtCmdSend->setText(QStringLiteral("Successfully Executed"));
                        ui->pbtnDoAction->setText(QStringLiteral("Executing Actions"));
                        dj_action_do = false;
                    } else {
                        ui->edtCmdSend->setText(QStringLiteral("There Remains")+(QString::number(dj_action_time))+QStringLiteral("Loops"));
                    }
                    max_time = 20;
                }
                dj_action_start = ui->spinBoxDjStart->value();
            }
      //  }
      /*  else {//下位机执行
            if(dj_action_do_dn_times) {
                if(ui->spinBoxDjStart->value() == ui->spinBoxDjEnd->value()) {
                    if(ui->spinBoxDoTimes->value() == 0) {
                        dj_str_temp = "$DAT:1!";
                    } else {
                        dj_str_temp = "$DAT:" + QString::number(ui->spinBoxDoTimes->value()) + "!";
                    }
                } else {
                    if(ui->spinBoxDoTimes->value() == 0) {
                        dj_str_temp = "$DGT:" + QString::number(ui->spinBoxDjStart->value()) + "-"
                                  + QString::number(ui->spinBoxDjEnd->value()) + ",1!";

                    } else {
                        dj_str_temp = "$DGT:" + QString::number(ui->spinBoxDjStart->value()) + "-"
                                  + QString::number(ui->spinBoxDjEnd->value()) + ","
                                  + QString::number(ui->spinBoxDoTimes->value()) + "!";
                    }
                }
                send_uart_str(dj_str_temp);
                qDebug() << dj_str_temp;
                max_time = 1000;
                dj_action_do_dn_times = false;
            }
        }*/
    }/* else if(dj_action_save) {//执行保存下位机
        if(i<dj_group_index_all) {
            saveStr = "&" + ui->listWidget->item(i)->text();
            send_uart_str(saveStr);
            i++;
        } else {
            i = 0;
            ui->txtCmd->append(QStringLiteral("保存动作组成功，数量：") + QString::number(dj_group_index_all));
            ui->pbtnSaveDjAction->setText(QStringLiteral("存下位机"));
            dj_action_save = false;
        }
        max_time = 300; //200ms存一条
    } */
    else {
        max_time = 1000;
    }
    dj_timer_ctl->start(max_time);
}

void MainWindow::on_pbtnSaveDjAction_clicked() {
/*
    if(dj_action_save) {
       dj_action_save = false;
       ui->pbtnSaveDjAction->setText(QStringLiteral("存下位机"));
    } else {
       dj_action_save = true;
       ui->txtCmd->append(QStringLiteral("保存数据开始"));
       ui->pbtnSaveDjAction->setText(QStringLiteral("停止"));
    }
    */
    for(int i=1;i<dj_group_index_all;i++) {
        QListWidgetItem * item = ui->listWidget->takeItem(0);
        delete item;
    }

   // ui->listWidget->clear();
    dj_group_index_all=1;
     dj_group_index_cur=0;
       ui->listWidget->item(0)->setText("{G0}");
}

void MainWindow::on_pbtnClearTxtCmd_clicked()
{
    ui->txtCmd->setText("");
}
