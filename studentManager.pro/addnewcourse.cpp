#include "addnewcourse.h"
#include "ui_addnewcourse.h"
#include "global.h"
#include <QMessageBox>
#include "coursemanage.h"

//管理员用-课程信息管理-添加课程按钮
addNewCourse::addNewCourse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addNewCourse)
{
    ui->setupUi(this);
    ui->timeBox->addItem("大一上");
    ui->timeBox->addItem("大一下");
    ui->timeBox->addItem("大二上");
    ui->timeBox->addItem("大二下");
    ui->timeBox->addItem("大三上");
    ui->timeBox->addItem("大三下");
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));
}

addNewCourse::~addNewCourse()
{
    delete ui;
}
//提交按钮
void addNewCourse::on_submitButton_clicked()
{
    QString cId = ui->courseId_lineEdit->text();
    QString object = ui->object_lineEdit->text();
    QString teacher = ui->teacher_lineEdit->text();
    QString time = ui->timeBox->currentText();
    QString location = ui->location_lineEdit->text();
    QString period = ui->period_lineEdit->text();
    QString credit = ui->credit_lineEdit->text();

    if(cId.isEmpty() || object.isEmpty() || teacher.isEmpty() || time.isEmpty() || location.isEmpty()
            || period.isEmpty() || credit.isEmpty())
    {
        QMessageBox::warning(this, "信息出错", "课程信息不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    //查询步骤
    query.prepare("select cId from course where cId = ?");
    query.addBindValue(cId);
    query.exec();
    if(query.next())
    {
        QMessageBox::warning(this, "添加出错", "该课程号已存在！");
        return;
    }
    //插入步骤
    query.prepare("INSERT INTO course (cId,object,teacher,time,location,period,credit)"
                  "VALUES             (:cId,:object,:teacher,:time,:location,:period,:credit)");
    query.bindValue(":cId",cId);
    query.bindValue(":object",object);
    query.bindValue(":teacher",teacher);
    query.bindValue(":time", time);
    query.bindValue(":location",location);
    query.bindValue(":period",period);
    query.bindValue(":credit",credit);
    bool isInsertSuccess = query.exec();

    if(isInsertSuccess)
    {
        QMessageBox::information(this, "提示", "添加成功");
    }
    else {
        QMessageBox::warning(this, "添加出错", "请检查填写信息！");
    }
}
