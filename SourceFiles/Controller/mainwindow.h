#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QSocketNotifier>
#include <QGridLayout>
#include "mythread.h"
#include "myStruct.h"
#include<QProcess>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MyThread threadA;
    void closeEvent(QCloseEvent *event);
    void sleep(unsigned int msec);
    void ui_init(void);
    void statusBar_init(void);
    void send_uart_str(QString str);
    QString get_uart_str(void);
    QProcess *process=new QProcess(this);
    void dj_add_item(void);     //向后增加item
    void dj_remove_item(void);  //移除某个item并重新排列

    void keyPressEvent( QKeyEvent* e );
    void keyReleaseEvent( QKeyEvent* e );


    void pwmBarHandle(int index, int pwm, int time);
    void dj_data_init(void);
    void dj_single_clear(void);
    void combine_dj_single_all(void);
    QString combine_dj_single(int index,  int pwm, int time);
    QString combine_dj_group(void);
    void dj_group_to_dj_single(QString str);
    void djSingleupToKongJian(int index, int pwm, int time);
    QString djGroupChangeIndex(QString str, int index);
    void handleRdActionMessageBox(QString title, QString content);
    void handleRdSectorMessageBox(QString title, QString content);
    void dj_timer_ctl_init(void);

    int  getMaxTimeOfDjAction(QString str);
    int getMaxTimeOfDjAction1(QString str);
    void serial_monitor_init(void);
    void serial_monitor(void);

    void import_DJG_cover(QString str);
    void import_DJG_append(QString str);
    void keypress(void);

private:
    Ui::MainWindow *ui;
    DJ_single dj_single[8];
    DJ_group  dj_group;
    int dj_group_index_all, dj_group_index_cur;
    bool dj_combine_style;
    QMessageBox *messageBox;
    QLabel *statusBarLabel;
    QTimer *dj_timer_ctl;
    bool dj_action_do, dj_action_save, dj_action_do_dn_times, flag_dj_rd_action, flag_dj_rd_sector;
    QString seria_str;
    QSocketNotifier *m_notifier;
    int dj_action_start, dj_action_end, dj_action_time,dj_action_time_buf;
    bool flag_serial_open;
    bool simflag;
    bool mPressFlag_A=false;
    int  mKeyDirection;

private slots:
    void handleRxData(void);
    void on_pbtnOpen_clicked();
    void on_pbtnSend_clicked();
    void on_listWidget_clicked(const QModelIndex &index);

    void on_pwmBar0_valueChanged(int value);
    void on_pwmSpinBox0_valueChanged(int arg1);
    void on_timeBar0_valueChanged(int value);
    void on_timeSpinBox0_valueChanged(int arg1);

    void on_pwmBar1_valueChanged(int value);
    void on_pwmSpinBox1_valueChanged(int arg1);
    void on_timeBar1_valueChanged(int value);
    void on_timeSpinBox1_valueChanged(int arg1);

    void on_pwmBar2_valueChanged(int value);
    void on_pwmSpinBox2_valueChanged(int arg1);
    void on_timeBar2_valueChanged(int value);
    void on_timeSpinBox2_valueChanged(int arg1);

    void on_pwmBar3_valueChanged(int value);
    void on_pwmSpinBox3_valueChanged(int arg1);
    void on_timeBar3_valueChanged(int value);
    void on_timeSpinBox3_valueChanged(int arg1);

    void on_pwmBar4_valueChanged(int value);
    void on_pwmSpinBox4_valueChanged(int arg1);
    void on_timeBar4_valueChanged(int value);
    void on_timeSpinBox4_valueChanged(int arg1);

    void on_pwmBar5_valueChanged(int value);
    void on_pwmSpinBox5_valueChanged(int arg1);
    void on_timeBar5_valueChanged(int value);
    void on_timeSpinBox5_valueChanged(int arg1);

    void on_timeSliderAll_valueChanged(int value);
    void on_timeSpinBoxAll_valueChanged(int arg1);
    void on_pwmFocus_clicked();
    void on_chkDjStyle_toggled(bool checked);
    void on_pbtnAddGroup_clicked();
    void on_pbtnDelGroup_clicked();
    void on_gbox0_toggled(bool arg1);
    void on_gbox1_toggled(bool arg1);
    void on_gbox2_toggled(bool arg1);
    void on_gbox3_toggled(bool arg1);
    void on_gbox4_toggled(bool arg1);
    void on_gbox5_toggled(bool arg1);
    void on_pbtnRdDnIn_clicked();
    void on_pbtnRdDnAc_clicked();
    void on_pbtnEaDnAc_clicked();
    void on_pbtnDoAction_clicked();
    void dj_timer_ctl_timerOut(void);
    void on_pbtnSaveDjAction_clicked();
    void on_pbtnClearTxtCmd_clicked();
    void on_SimBtn_clicked();
};

#endif // MAINWINDOW_H
