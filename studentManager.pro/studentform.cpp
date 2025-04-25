#include "studentform.h"
#include "ui_studentform.h"
#include "global.h"
#include "mainwindow.h"
#include "myinfo.h"
#include "myclass.h"
#include "myscore.h"
#include "mycourse.h"
#include "deepseek.h"
#include "QDebug"
#include <QProcess>

//学生用-登录页面
studentForm::studentForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::studentForm)
{
    ui->setupUi(this);

    ui->label->setText("当前用户:" );
    ui->label_user->setText( sName_Current);
    ui->label_user->setStyleSheet("color:#0022ff");

    setWindowTitle(QString("桂林电子科技大学"));
    setWindowIcon(QIcon(":/img/桂电小标"));

    ui->frame->resize(this->width(),this->height());
    ui->frame->setStyleSheet("QFrame#frame{border-image:url(:/img/背景)}" );
    //堆栈窗口
    myInfo *MInfo = new myInfo;
    myclass *Mclass = new myclass;
    myScore *Mscore = new myScore;
    mycourse *Msourse = new mycourse;
    ui->stackedWidget->insertWidget(0,MInfo); //查看个人基本信息
    ui->stackedWidget->insertWidget(1,Mclass);//班级基本情况
    ui->stackedWidget->insertWidget(2,Mscore);//成绩查询
    ui->stackedWidget->insertWidget(3,Msourse);//课程查询

}

studentForm::~studentForm()
{

    delete ui;
}

//注销按钮
void studentForm::on_pushButton_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "确认", "确定要注销账号吗？需要重新登录。", QMessageBox::Yes|QMessageBox::No);
    if(btn == QMessageBox::Yes){
        QMessageBox::information(this, "提示", "注销成功！请重新登录！", QMessageBox::Yes);
        qApp->quit(); // 退出当前应用程序
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments()); // 重启程序
    }else{
        QMessageBox::information(this, "提示", "取消操作", QMessageBox::Yes);
    }

}
//查看个人基本信息
void studentForm::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//班级基本情况
void studentForm::on_stu_findclass_inforButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
//成绩查询
void studentForm::on_pushButton_student_findscore_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
//课程查询
void studentForm::on_stu_find_coursebutton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
//打开deepseek
void studentForm::on_Xiao_Gui_clicked()
{
    CreateQWindow<Deepseek>();
}
