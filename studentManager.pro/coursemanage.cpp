#include "coursemanage.h"
#include "ui_coursemanage.h"
#include "global.h"
#include "manager.h"
#include <QMessageBox>
#include "allcourseinfo.h"
#include "addnewcourse.h"

//管理员用-课程信息管理
courseManage::courseManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::courseManage)
{
    ui->setupUi(this);
    ui->timeBox->addItem("大一上");
    ui->timeBox->addItem("大一下");
    ui->timeBox->addItem("大二上");
    ui->timeBox->addItem("大二下");
    ui->timeBox->addItem("大三上");
    ui->timeBox->addItem("大三下");
    ui->update_button->setDisabled(true);
    ui->delete_button->setDisabled(true);
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));
}

courseManage::~courseManage()
{
    delete ui;
}

//查询按钮
void courseManage::on_searchButton_clicked()
{
    QString cId = ui->courseId_lineEdit->text();
    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    //查询课程信息
    query.prepare("select object, teacher, time, location, period, credit from course where cId = ?");
    query.addBindValue(cId);
    query.exec();

    if(query.next())
    {
        QString object = query.value(0).toString();
        QString teacher = query.value(1).toString();
        QString time = query.value(2).toString();
        QString location = query.value(3).toString();
        QString period = query.value(4).toString();
        QString credit = query.value(5).toString();

        //统计选课数量
        QString courseNum = countCourse(cId);

        ui->courseName_lineEdit->setText(object);
        ui->teacher_lineEdit->setText(teacher);
        ui->timeBox->setCurrentIndex(ui->timeBox->findText(time));
        ui->location_lineEdit->setText(location);
        ui->period_lineEdit->setText(period);
        ui->credit_lineEdit->setText(credit);
        ui->courseNum_lineEdit->setText(courseNum);
        //激活更新和删除按钮
        ui->update_button->setDisabled(false);
        ui->delete_button->setDisabled(false);
    }
    else {
        ui->update_button->setDisabled(true);
        ui->delete_button->setDisabled(true);

        ui->courseName_lineEdit->clear();
        ui->teacher_lineEdit->clear();
        ui->timeBox->setCurrentIndex(0);
        ui->location_lineEdit->clear();
        ui->period_lineEdit->clear();
        ui->credit_lineEdit->clear();
        ui->courseNum_lineEdit->clear();

        QMessageBox::warning(this, "查询无果", "不存在该课程！");

        ui->courseId_lineEdit->setFocus();

    }
}

//查看所有课程按钮
void courseManage::on_listAllCourse_button_clicked()
{
    CreateQWindow<allCourseInfo>();
}

//添加课程按钮
void courseManage::on_add_button_clicked()
{
    CreateQWindow<addNewCourse>();
}

//更新课程按钮
void courseManage::on_update_button_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "确认修改", "确定修改此课程信息？", QMessageBox::Yes|QMessageBox::No);
    if(btn == QMessageBox::Yes)
    {
        QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        QSqlQuery query(db);

        QString cId = ui->courseId_lineEdit->text();
        QString object = ui->courseName_lineEdit->text();
        QString teacher = ui->teacher_lineEdit->text();
        QString time = ui->timeBox->currentText();
        QString location = ui->location_lineEdit->text();
        QString period = ui->period_lineEdit->text();
        QString credit = ui->credit_lineEdit->text();

        query.prepare("UPDATE course SET "
                      "object = :object, "
                      "teacher = :teacher, "
                      "time = :time, "
                      "location = :location, "
                      "period = :period, "
                      "credit = :credit "
                      "WHERE cId = :cId");
        query.bindValue(":object", object);
        query.bindValue(":teacher", teacher);
        query.bindValue(":time", time);
        query.bindValue(":location", location);
        query.bindValue(":period", period);
        query.bindValue(":credit", credit);
        query.bindValue(":cId", cId);

        bool isUpdateInCourseSuccess = query.exec();

        if(isUpdateInCourseSuccess)
        {
            QMessageBox::information(this, "提示", "修改成功");

        }
        else {
            QMessageBox::warning(this, "修改失败", "请检查输入信息！");
        }
    }

}

//删除课程按钮
void courseManage::on_delete_button_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "确认删除", "确定删除此课程信息？", QMessageBox::Yes|QMessageBox::No);
    if(btn == QMessageBox::Yes)
    {
        QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        QSqlQuery query(db);

        QString cId = ui->courseId_lineEdit->text();
        //删除课程ID
        query.prepare("delete from course where cId = ?");
        query.addBindValue(cId);
        bool isDeleteSuccess = query.exec();

        if(isDeleteSuccess)
        {
            QMessageBox::information(this, "提示", "删除成功");
        }
        else {
            QMessageBox::warning(this, "警告", "删除失败！");
        }
    }
}

//统计函数
QString countCourse(QString cId){

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    QString courseNum;
    query.prepare("SELECT cId, COUNT(*) FROM student_course WHERE cId = ? GROUP BY cId ");
    query.addBindValue(cId);
    query.exec();
    if(query.next())
        courseNum = query.value(1).toString();
    else {
        courseNum = "0";
    }
    qDebug()<<courseNum;
    return  courseNum;
}
