#include "addnewgrade.h"
#include "ui_addnewgrade.h"
#include "global.h"
#include "grademanage.h"
#include <QMessageBox>

//管理员用-成绩信息管理-添加成绩信息
addNewGrade::addNewGrade(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addNewGrade)
{
    ui->setupUi(this);
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));
}

addNewGrade::~addNewGrade()
{
    delete ui;
}

void addNewGrade::on_cId_lineEdit_textChanged(const QString &arg1)
{
    QString cId = arg1;

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.prepare("select object,teacher from course where cId = ?");
    query.addBindValue(cId);
    query.exec();

    if(query.next())
    {
        QString object = query.value(0).toString();
        QString teacher = query.value(1).toString();
        ui->cName_lineEdit->setText(object);
        ui->teacher_lineEdit->setText(teacher);
    }
    else {
        ui->cName_lineEdit->clear();
        ui->teacher_lineEdit->clear();
    }
}

void addNewGrade::on_submit_button_clicked()
{
    QString sId = ui->sId_lineEdit->text();
    QString cId = ui->cId_lineEdit->text();
    QString object = ui->cName_lineEdit->text();
    QString score = ui->score_lineEdit->text();
    QString teacher = ui->teacher_lineEdit->text();

    if(sId.isEmpty() || cId.isEmpty() || object.isEmpty() || score.isEmpty() || teacher.isEmpty())
    {
        QMessageBox::warning(this, "插入出错", "信息不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.exec("select sId from student where sId = '" + sId + "'");

    if(!query.next())
    {
        QMessageBox::warning(this, "插入出错", "该学生不存在！");
        return;
    }

    query.exec("select sId,cId from student_course where sId = '" + sId + "' and cId = '" + cId + "'");

    if(!query.next())
    {
        QMessageBox::warning(this, "插入出错", "该学生未选择该课程！");
        return;
    }

    query.exec("select sId,cId from score where sid = '" + sId + "' and cid = '" + cId + "'");

    if(query.next())
    {
        QMessageBox::warning(this, "插入出错", "该成绩信息已存在！\n请勿重复添加！");
        return;
    }


    bool isInsertSuccess = query.exec(QString("insert into score values('%0','%1','%2','%3','%4')").arg(sId).arg(cId).arg(object).arg(score).arg(teacher));

    if(isInsertSuccess)
    {
        QMessageBox::information(this, "提示", "添加成功");
    }
    else {
        QMessageBox::warning(this, "插入出错", "请检查输入信息！");
    }
}
