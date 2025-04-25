#include "searchcourse.h"
#include "ui_searchcourse.h"
#include "global.h"
#include "mycourse.h"
#include <QMessageBox>

//学生用-查询其他课程
searchCourse::searchCourse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchCourse)
{
    ui->setupUi(this);

    setWindowTitle(QString("桂林电子科技大学"));
    setWindowIcon(QIcon(":/img/桂电小标"));

    ui->classTimeBox->addItem("大一上");
    ui->classTimeBox->addItem("大一下");
    ui->classTimeBox->addItem("大二上");
    ui->classTimeBox->addItem("大二下");
    ui->classTimeBox->addItem("大三上");
    ui->classTimeBox->addItem("大三下");

    ui->courseID_lineEdit->setReadOnly(true);
    ui->courseName_lineEdit->setReadOnly(true);
    ui->location_lineEdit->setReadOnly(true);
    ui->period_lineEdit->setReadOnly(true);
    ui->credit_lineEdit->setReadOnly(true);
}

searchCourse::~searchCourse()
{
    delete ui;
}
//清空按钮
void searchCourse::on_clearButton_clicked()
{
    ui->courName_lineEdit->clear();
    ui->courseName_lineEdit->clear();
    ui->teacher_lineEdit->clear();
    ui->classTimeBox->setCurrentIndex(0);
    ui->courseID_lineEdit->clear();
    ui->location_lineEdit->clear();
    ui->credit_lineEdit->clear();
    ui->period_lineEdit->clear();
}

//查询按钮
void searchCourse::on_searchButton_clicked()
{

    QString courseName = ui->courName_lineEdit->text();
    QString teacher = ui->teacher_lineEdit->text();
    QString courseTime = ui->classTimeBox->currentText();
    //输入检查
    if(courseName.isEmpty() || teacher.isEmpty() || courseName.isEmpty())
    {
        QMessageBox::warning(this, "输入出错", "查找信息不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.prepare("select cId,location,period,credit,object from course where object like "
                  "? and teacher= ? and time=?");
    query.addBindValue("%" + courseName + "%");//模糊查询
    query.addBindValue(teacher);
    query.addBindValue(courseTime);
    query.exec();
    //填数据
    if(query.next())
    {
        QString courseId = query.value(0).toString();
        QString location = query.value(1).toString();
        QString period = query.value(2).toString();
        QString credit = query.value(3).toString();
        QString courseName = query.value(4).toString();

        ui->courseID_lineEdit->setText(courseId);
        ui->courseName_lineEdit->setText(courseName);
        ui->location_lineEdit->setText(location);
        ui->period_lineEdit->setText(period);
        ui->credit_lineEdit->setText(credit);
    }
    else
    {
        QMessageBox::warning(this, "查询无果", "不存在对应教学班！");
    }
}
