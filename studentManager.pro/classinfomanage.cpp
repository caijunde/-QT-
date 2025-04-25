#include "classinfomanage.h"
#include "ui_classinfomanage.h"
#include <QMessageBox>
#include "global.h"
#include "manager.h"
#include "allclassinfo.h"
#include "addclass.h"

//管理员用-班级信息管理
classInfoManage::classInfoManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::classInfoManage)
{
    ui->setupUi(this);
    ui->delete_button->setDisabled(true);
    ui->update_button->setDisabled(true);
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));
}

classInfoManage::~classInfoManage()
{
    delete ui;
}

//查询按钮
void classInfoManage::on_search_button_clicked()
{
    //获取输入内容
    QString classId = ui->classId_lineEdit->text();
    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        if (!db.isOpen()) {
            QMessageBox::critical(this, "错误", "数据库未连接！");
            return;
        }
    QSqlQuery query(db);

    //查询
    query.prepare("select className, major, sNum, master from class where classId = ? ");
    query.addBindValue(classId);
    query.exec();
    if(query.next())
    {
        //加载班级信息
        QString className = query.value(0).toString();
        QString major = query.value(1).toString();
        QString sNum = query.value(2).toString();
        QString master = query.value(3).toString();
        //填入输入框
        ui->className_lineEdit->setText(className);
        ui->classMajor_Edit->setText(major);
        ui->classNum_lineEdit->setText(sNum);
        ui->classMaster_lineEdit->setText(master);
        //激活更新、删除按钮
        ui->update_button->setDisabled(false);
        ui->delete_button->setDisabled(false);
    }
    else {
        //隐藏更新、删除按钮
        ui->update_button->setDisabled(true);
        ui->delete_button->setDisabled(true);
        //清空
        ui->className_lineEdit->clear();
        ui->classMajor_Edit->clear();
        ui->classNum_lineEdit->clear();
        ui->classMaster_lineEdit->clear();

        QMessageBox::warning(this, "查询无果", "不存在该班级！");
        //设置焦点
        ui->classId_lineEdit->setFocus();

    }

}

//查看所有班级
void classInfoManage::on_listAll_button_clicked()
{
    CreateQWindow<allClassInfo>();
}

//添加班级
void classInfoManage::on_add_button_clicked()
{
    CreateQWindow<addClass>();
}

//更新班级
void classInfoManage::on_update_button_clicked()
{
    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        if (!db.isOpen()) {
            QMessageBox::critical(this, "错误", "数据库未连接！");
            return;
        }
    QSqlQuery query(db);

    QString classId = ui->classId_lineEdit->text();
    QString className = ui->className_lineEdit->text();
    QString major = ui->classMajor_Edit->text();
    QString sNum = ui->classNum_lineEdit->text();
    QString master = ui->classMaster_lineEdit->text();

    query.prepare("UPDATE class SET "
                  "className = :className, "
                  "major = :major, "
                  "sNum = :sNum, "
                  "master = :master"
                  "WHERE classId = :classId");
    query.bindValue(":className", className);
    query.bindValue(":major", major);
    query.bindValue(":sNum", sNum);
    query.bindValue(":master", master);
    query.bindValue(":classId", classId);
    bool isUpdateSuceess = query.exec();

    if(isUpdateSuceess)
    {
        QMessageBox::information(this, "提示", "更新班级信息成功");
    }
    else {
        QMessageBox::warning(this, "更新失败", "请检查输入信息！");
    }
}

//删除班级
void classInfoManage::on_delete_button_clicked()
{
    QString classId = ui->classId_lineEdit->text();

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "确认删除", "确定删除此班级信息？", QMessageBox::Yes|QMessageBox::No);

    if(btn == QMessageBox::Yes)
    {
        query.prepare("delete from class where classId = ?");
        query.addBindValue(classId);
        bool isUpdateSuceess = query.exec();
        if(isUpdateSuceess)
        {
            QMessageBox::information(this, "提示", "删除成功");

            ui->classId_lineEdit->clear();
            ui->className_lineEdit->clear();
            ui->classMajor_Edit->clear();
            ui->classNum_lineEdit->clear();
            ui->classMaster_lineEdit->setText(classId);

            ui->classId_lineEdit->setFocus();
        }
        else {
            QMessageBox::warning(this, "警告", "删除失败！");
        }
    }

}
