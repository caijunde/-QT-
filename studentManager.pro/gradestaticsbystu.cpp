#include "gradestaticsbystu.h"
#include "ui_gradestaticsbystu.h"
#include "grademanage.h"
#include "global.h"
#include <QMessageBox>

//管理员用-成绩信息管理-学生个人成绩统计
gradeStaticsByStu::gradeStaticsByStu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gradeStaticsByStu)
{
    ui->setupUi(this);
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));
}

gradeStaticsByStu::~gradeStaticsByStu()
{
    delete ui;
}

//统计按钮
void gradeStaticsByStu::on_start_button_clicked()
{
    QString sId = ui->sId_lineEdit->text();

    if(sId.isEmpty())
    {
        QMessageBox::warning(this, "统计出错", "学号信息不能为空");
        ui->creditSum_lineEdit->clear();
        ui->avgGrade_lineEdit->clear();
        ui->maxGrade_lineEdit->clear();
        ui->minGrade_lineEdit->clear();
        ui->notpassSum_lineEdit->clear();
        ui->selectedSum_lineEdit->clear();
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.prepare("select sId from student where sId = ?");
    query.addBindValue(sId);
    query.exec();

    if(!query.next())
    {
        QMessageBox::warning(this, "统计出错", "该学生不存在！");
        ui->creditSum_lineEdit->clear();
        ui->avgGrade_lineEdit->clear();
        ui->maxGrade_lineEdit->clear();
        ui->minGrade_lineEdit->clear();
        ui->notpassSum_lineEdit->clear();
        ui->selectedSum_lineEdit->clear();
        return;
    }

    query.prepare("select sId from student_course where sId = ?");
    query.addBindValue(sId);
    query.exec();

    if(!query.next())
    {
        QMessageBox::warning(this, "统计出错", "该学生没有选课信息！");
        ui->creditSum_lineEdit->clear();
        ui->avgGrade_lineEdit->clear();
        ui->maxGrade_lineEdit->clear();
        ui->minGrade_lineEdit->clear();
        ui->notpassSum_lineEdit->clear();
        ui->selectedSum_lineEdit->clear();
        return;
    }

    query.prepare("select count(cId) from student_course where sId = ?");
    query.addBindValue(sId);
    query.exec();

    if(query.next())
    {
        QString courseSum = query.value(0).toString();
        ui->selectedSum_lineEdit->setText(courseSum);
    }
    //不及格课程数
    query.prepare("select count(*) from score where score < 60 and sid = ?");
    query.addBindValue(sId);
    query.exec();
    if(query.next())
    {
        QString notPassSum = query.value(0).toString();
        ui->notpassSum_lineEdit->setText(notPassSum);
    }
    //课程最高分
    query.prepare("select max(score) from score where sid = ?");
    query.addBindValue(sId);
    query.exec();
    if(query.next())
    {
        QString maxScore = query.value(0).toString();
        ui->maxGrade_lineEdit->setText(maxScore);
    }
    //课程最低分
    query.prepare("select min(score) from score where sid = ?");
    query.addBindValue(sId);
    query.exec();
    if(query.next())
    {
        QString minScore = query.value(0).toString();
        ui->minGrade_lineEdit->setText(minScore);
    }
    //平均分
    query.prepare("select avg(score) from score where sid = ?");
    query.addBindValue(sId);
    query.exec();
    if(query.next())
    {
        QString avgScore = query.value(0).toString();
        ui->avgGrade_lineEdit->setText(avgScore);
    }
    //已选课程数
    query.prepare("select sum(credit) from course where cId in (select cid from score where sid = ? and score > 60)");
    query.addBindValue(sId);
    query.exec();
    if(query.next())
    {
        QString creditSum = query.value(0).toString();
        ui->creditSum_lineEdit->setText(creditSum);
    }
}
