#include "gradestaticsbycourse.h"
#include "ui_gradestaticsbycourse.h"
#include "global.h"
#include "grademanage.h"
#include <QMessageBox>

//管理员用-成绩信息管理-课程成绩统计
gradeStaticsByCourse::gradeStaticsByCourse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gradeStaticsByCourse)
{
    ui->setupUi(this);
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));
}

gradeStaticsByCourse::~gradeStaticsByCourse()
{
    delete ui;
}

//统计按钮
void gradeStaticsByCourse::on_search_button_clicked()
{
    QString cId = ui->cId_lineEdit->text();
    //空输入警告
    if(cId.isEmpty())
    {
        QMessageBox::warning(this, "统计出错", "课程号不能留空！");
        ui->cName_lineEdit->clear();
        ui->teacher_lineEdit->clear();
        ui->maxScore_lineEdit->clear();
        ui->minScore_lineEdit->clear();
        ui->avgScore_lineEdit->clear();
        ui->selectSum_lineEdit->clear();
        ui->notPassSum_lineEdit->clear();
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.prepare("select cId from course where cId = ?");
    query.addBindValue(cId);
    query.exec();
    //无结果警告
    if(!query.next())
    {
        QMessageBox::warning(this, "统计出错", "该课程不存在！");
        ui->cName_lineEdit->clear();
        ui->teacher_lineEdit->clear();
        ui->maxScore_lineEdit->clear();
        ui->minScore_lineEdit->clear();
        ui->avgScore_lineEdit->clear();
        ui->selectSum_lineEdit->clear();
        ui->notPassSum_lineEdit->clear();
        return;
    }

    query.prepare("select object,teacher from course where cId = ?");
    query.addBindValue(cId);
    query.exec();
    //
    if(query.next())
    {
        QString object = query.value(0).toString();
        QString teacher = query.value(1).toString();
        ui->cName_lineEdit->setText(object);
        ui->teacher_lineEdit->setText(teacher);
    }

    query.prepare("select count(sId) from student_course where cId = ?");
    query.addBindValue(cId);
    query.exec();
    //选择此课学生数
    if(query.next())
    {
        QString selectSum = query.value(0).toString();
        ui->selectSum_lineEdit->setText(selectSum);
    }

    query.prepare("select max(score.score), min(score.score), avg(score.score) from score where score.cid = ?");
    query.addBindValue(cId);
    query.exec();
    //成绩分析函数
    if(query.next())
    {
        QString maxScore = query.value(0).toString();
        QString minScore = query.value(1).toString();
        QString avgScore = query.value(2).toString();
        ui->maxScore_lineEdit->setText(maxScore);
        ui->minScore_lineEdit->setText(minScore);
        ui->avgScore_lineEdit->setText(avgScore);
    }

    query.prepare("select count(*) from score where cid = ? and score.score < 60");
    query.addBindValue(cId);
    query.exec();
    //挂科统计
    if(query.next())
    {
        QString notPassSum = query.value(0).toString();
        ui->notPassSum_lineEdit->setText(notPassSum);
    }
}
