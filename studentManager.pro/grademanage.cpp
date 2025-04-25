#include "grademanage.h"
#include "ui_grademanage.h"
#include "manager.h"
#include "global.h"
#include <QMessageBox>
#include "addnewgrade.h"
#include <QInputDialog>
#include "gradestaticsbystu.h"
#include "gradestaticsbycourse.h"
#include "exporttable.h"

//管理员用-成绩信息管理
gradeManage::gradeManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gradeManage)
{
    ui->setupUi(this);
    ui->gradeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->currentSelected_label->setVisible(false);
    ui->update_gradeButton->setDisabled(true);
    ui->delete_button->setDisabled(true);
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));
    updataTable();
}

gradeManage::~gradeManage()
{
    delete ui;
}
//读表函数
void gradeManage::updataTable()
{
    ui->gradeTable->clearContents();
    ui->gradeTable->setRowCount(0);

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.exec("select sid,cid,object,score,teacher from score order by sid");

    while(query.next())
    {
        QString sId = query.value(0).toString();
        QString cId = query.value(1).toString();
        QString object = query.value(2).toString();
        QString score = query.value(3).toString();
        QString teacher = query.value(4).toString();

        QStringList q;
        q << sId << cId << object << score << teacher;
    //填入数据
        int rowCount = ui->gradeTable->rowCount();
        ui->gradeTable->insertRow(rowCount);

        for(int i = 0; i<q.size(); i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(q.at(i));
            ui->gradeTable->setItem(rowCount, i, item);
        }
    }
    //设置格式
    int columnCount = ui->gradeTable->columnCount();
    int rowCount = ui->gradeTable->rowCount();

    for(int i = 0; i <columnCount; i++)
    {
        for(int j = 0; j < rowCount; j++)
        {
            ui->gradeTable->item(j, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

//查看所有成绩
void gradeManage::on_listAll_button_clicked()
{
    updataTable();
    //ui->currentSelected_label->setVisible(false);
    ui->update_gradeButton->setDisabled(true);
    ui->delete_button->setDisabled(true);
}

//根据学号查找
void gradeManage::on_searchBysNo_clicked()
{
    ui->gradeTable->clearContents();
    ui->gradeTable->setRowCount(0);

    //ui->currentSelected_label->setVisible(false);
    ui->update_gradeButton->setDisabled(true);
    ui->delete_button->setDisabled(true);

    QString sId = ui->sId_lineEdit->text();
    //输入判断
    if(sId.isEmpty())
    {
        QMessageBox::warning(this, "查询出错", "学号不能留空！");
        return;
    }
    //链接数据库
    QSqlDatabase db = DatabaseManager::connect_to_database();
    QSqlQuery query(db);
    //查询
    query.prepare("select sId from student where sId = ?");
    query.addBindValue(sId);
    query.exec();
    if(!query.next())
    {
        QMessageBox::warning(this, "查询出错", "该学号不存在！");
        return;
    }
    //获得目标表数据
    query.prepare("select sid,cid,object,score,teacher from score where sid = ? order by cid");
    query.addBindValue(sId);
    query.exec();

    if(query.next())
    {
        while(query.next())
        {
            QString sId = query.value(0).toString();
            QString cId = query.value(1).toString();
            QString object = query.value(2).toString();
            QString score = query.value(3).toString();
            QString teacher = query.value(4).toString();

            QStringList q;
            q << sId << cId << object << score << teacher;
            //填入数据
            int rowCount = ui->gradeTable->rowCount();
            ui->gradeTable->insertRow(rowCount);

            for(int i = 0; i<q.size(); i++)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setText(q.at(i));
                ui->gradeTable->setItem(rowCount, i, item);
            }
        }
        //排版
        int columnCount = ui->gradeTable->columnCount();
        int rowCount = ui->gradeTable->rowCount();

        for(int i = 0; i <columnCount; i++)
        {
            for(int j = 0; j < rowCount; j++)
            {
                ui->gradeTable->item(j, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
        }
    }
    else {
        QMessageBox::warning(this, "查询无果", "该学生没有成绩记录！");
    }

}

//根据课程号查找
void gradeManage::on_searchBycId_clicked()
{
    ui->gradeTable->clearContents();
    ui->gradeTable->setRowCount(0);

    //ui->currentSelected_label->setVisible(false);
    ui->update_gradeButton->setDisabled(true);
    ui->delete_button->setDisabled(true);

    QString cId = ui->cId_lineEdit->text();
    //输入检查
    if(cId.isEmpty())
    {
        QMessageBox::warning(this, "查询出错", "课程号不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database();
    QSqlQuery query(db);
    //查询是否存在
    query.prepare("select cId from course where cId = ?");
    query.addBindValue(cId);
    query.exec();

    if(!query.next())
    {
        QMessageBox::warning(this, "查询出错", "该课程号不存在！");
        return;
    }

    query.prepare("select sid,cid,object,score,teacher from score where cid = ? order by sid");
    query.addBindValue(cId);
    query.exec();
    //绘制表格
    if(query.next())
    {
        while(query.next())
        {
            QString sId = query.value(0).toString();
            QString cId = query.value(1).toString();
            QString object = query.value(2).toString();
            QString score = query.value(3).toString();
            QString teacher = query.value(4).toString();

            QStringList q;
            q << sId << cId << object << score << teacher;

            int rowCount = ui->gradeTable->rowCount();
            ui->gradeTable->insertRow(rowCount);

            for(int i = 0; i<q.size(); i++)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setText(q.at(i));
                ui->gradeTable->setItem(rowCount, i, item);
            }
        }
        //修改格式
        int columnCount = ui->gradeTable->columnCount();
        int rowCount = ui->gradeTable->rowCount();

        for(int i = 0; i <columnCount; i++)
        {
            for(int j = 0; j < rowCount; j++)
            {
                ui->gradeTable->item(j, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
        }
    }
    else {
        QMessageBox::warning(this, "查询无果", "该课程没有成绩记录！");
    }
}

//添加成绩信息
void gradeManage::on_addGrade_button_clicked()
{
    CreateQWindow<addNewGrade>();
}

//点击单元格
void gradeManage::on_gradeTable_cellClicked(int row, int column)
{
    column = 0;
    //ui->currentSelected_label->setVisible(true);
    ui->update_gradeButton->setDisabled(false);
    ui->delete_button->setDisabled(false);

    QString sId = ui->gradeTable->item(row, 0)->text();
    QString cId = ui->gradeTable->item(row, 1)->text();
    //将单元格数值填入
    ui->sId_lineEdit->setText(sId);
    ui->cId_lineEdit->setText(cId);
}

//成绩修改按钮
void gradeManage::on_update_gradeButton_clicked()
{
    bool isOk;
    QString score = QInputDialog::getText(this, "成绩修改", "请输入要修改的成绩", QLineEdit::Normal, "输入成绩", &isOk);

    if(isOk)
    {
        QString sId = ui->sId_lineEdit->text();
        QString cId = ui->cId_lineEdit->text();

        QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        QSqlQuery query(db);

        //bool isUpdateSuccess = query.exec(QString("update score set score = '%0' where sid = '%1' and cid = '%2'").arg(score).arg(sId).arg(cId));
        query.prepare("UPDATE score SET score = :score WHERE sid = :sId AND cid = :cId");
        query.bindValue(":score", score);
        query.bindValue(":sId", sId);
        query.bindValue(":cId", cId);
        bool isUpdateSuccess = query.exec();
        if(isUpdateSuccess)
        {
            QMessageBox::information(this, "提示", "成绩修改成功");
            //ui->currentSelected_label->setVisible(false);
            ui->update_gradeButton->setDisabled(true);
            ui->delete_button->setDisabled(true);
            ui->sId_lineEdit->clear();
            ui->cId_lineEdit->clear();
            updataTable();
        }
        else {
            QMessageBox::warning(this, "修改失败", "请检查输入信息！");
            //ui->currentSelected_label->setVisible(false);
            ui->update_gradeButton->setDisabled(true);
            ui->delete_button->setDisabled(true);
            ui->sId_lineEdit->clear();
            ui->cId_lineEdit->clear();
        }
    }
}

//删除选中成绩信息
void gradeManage::on_delete_button_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "确定删除", "确定删除此课程成绩信息？", QMessageBox::Yes|QMessageBox::No);

    if(btn == QMessageBox::Yes)
    {
        QString sId = ui->sId_lineEdit->text();
        QString cId = ui->cId_lineEdit->text();

        QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        QSqlQuery query(db);

        //bool isDeleteSuccess = query.exec(QString("delete from score where sid = '%0' and cid = '%1'").arg(sId).arg(cId));
        query.prepare("DELETE FROM score WHERE sid = :sId AND cid = :cId");
        query.bindValue(":sId", sId);
        query.bindValue(":cId", cId);
        bool isDeleteSuccess = query.exec();

        if(isDeleteSuccess)
        {
            QMessageBox::information(this, "提示", "删除成功");
            //ui->currentSelected_label->setVisible(false);
            ui->update_gradeButton->setDisabled(true);
            ui->delete_button->setDisabled(true);
            ui->sId_lineEdit->clear();
            ui->cId_lineEdit->clear();
            updataTable();
        }
        else {
            QMessageBox::warning(this, "删除失败", "请检查输入信息！");
            //ui->currentSelected_label->setVisible(false);
            ui->update_gradeButton->setDisabled(true);
            ui->delete_button->setDisabled(true);
            ui->sId_lineEdit->clear();
            ui->cId_lineEdit->clear();
        }
    }
}

//学生个人成绩统计
void gradeManage::on_student_statistics_clicked()
{
    CreateQWindow<gradeStaticsByStu>();
}

//课程成绩统计
void gradeManage::on_course_statistics_clicked()
{
    CreateQWindow<gradeStaticsByCourse>();
}

//导出excel按钮
void gradeManage::on_export_button_clicked()
{
    Table2Excel(ui->gradeTable, "成绩信息");
}
