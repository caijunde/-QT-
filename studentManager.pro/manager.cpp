#include "manager.h"
#include "ui_manager.h"
#include "global.h"
#include "mainwindow.h"
#include "stuinfomanage.h"
#include "classinfomanage.h"
#include "coursemanage.h"
#include "selectcoursemanage.h"
#include "grademanage.h"
#include "manager_manage.h"
#include <QProcess>

//管理员账号初始界面
manager::manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::manager)
{
    ui->setupUi(this);
    //提示当前管理员
    QLabel *lab = ui->label_useradmin;
    lab->setStyleSheet("color:#ff0000");
    lab->setText(username_Current);
    //设置标题
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));

    //ui->frame->setObjectName("myframe");
    ui->frame->resize(this->width(),this->height());
    ui->frame->setStyleSheet("QFrame#frame{border-image:url(:/img/背景)}" );
    init();
}

manager::~manager()
{
    delete ui;
}

//初始化
void manager::init(){

    stuinfomanage *stuinfo = new stuinfomanage;
    classInfoManage *classInfo = new classInfoManage;
    courseManage *couManage = new courseManage;
    selectCourseManage *selectCou = new selectCourseManage;
    gradeManage *gradeMana = new gradeManage;
    Manager_Manage *Man_Manager = new Manager_Manage;

    ui->stackedWidget->insertWidget(0,stuinfo); //学生信息管理
    ui->stackedWidget->insertWidget(1,classInfo);//班级信息管理
    ui->stackedWidget->insertWidget(2,couManage);//课程信息管理
    ui->stackedWidget->insertWidget(3,selectCou);//选课信息管理
    ui->stackedWidget->insertWidget(4,gradeMana);//成绩信息管理
    ui->stackedWidget->insertWidget(5,Man_Manager);//管理员账号管理
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->update();

}

//注销账户
void manager::on_pushButton_9_clicked()
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

//学生信息管理
void manager::on_stu_inform_guanlipushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//班级信息管理
void manager::on_class_managepushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//课程信息管理
void manager::on_course_manageButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//选课信息管理
void manager::on_selectCourse_manageButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

//成绩信息管理
void manager::on_scoremanagepushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

//管理员账号管理
void manager::on_Manager_Manage_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}
