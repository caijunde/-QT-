#include "selectcoursemanage.h"
#include "ui_selectcoursemanage.h"
#include "global.h"
#include "manager.h"
#include <QMessageBox>
#include "exporttable.h"

//管理员用-选课信息管理
selectCourseManage::selectCourseManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectCourseManage)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->currentSelectedLabel->setVisible(false);
    ui->delete_button->setDisabled(true);
    setWindowTitle(QString("桂林电子科技大学"));
    setWindowIcon(QIcon(":/img/桂电小标"));
    updateTable();
}

selectCourseManage::~selectCourseManage()
{
    delete ui;
}

//初始化-显示全部选课信息
void selectCourseManage::updateTable()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.exec("select sId,cId from student_course order by sId");
    //填入数据
    while(query.next())
    {
        QString sId = query.value(0).toString();
        QString cId = query.value(1).toString();

        QStringList q;
        q << sId << cId;

        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);

        for(int i = 0; i<q.size(); i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(q.at(i));
            ui->tableWidget->setItem(rowCount, i, item);
        }
    }
    //修改格式
    int columnCount = ui->tableWidget->columnCount();
    int rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i <columnCount; i++)
    {
        for(int j = 0; j < rowCount; j++)
        {
            ui->tableWidget->item(j, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }

}

//搜索按钮
void selectCourseManage::on_searchBySNo_button_clicked()
{
    ui->currentSelectedLabel->setVisible(false);
    //ui->updata_button->setDisabled(true);
    ui->delete_button->setDisabled(true);
    ui->add_button->setDisabled(false);
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    QString sId = ui->sNo_lineEdit->text();

    if(sId.isEmpty())
    {
        QMessageBox::warning(this, "查询出错", "学号信息不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.prepare("select sId from student where sId = ?");
    query.addBindValue(sId);
    query.exec();
    if(!query.next())
    {
        QMessageBox::warning(this, "查询出错", "该学生不存在！");
        return;
    }

    query.prepare("select sId,cId from student_course where sId = ? order by cId");
    query.addBindValue(sId);
    query.exec();

    if(query.size() != 0)
    {
        //填入数据
        while(query.next())
        {

            QString sId = query.value(0).toString();
            QString cId = query.value(1).toString();

            QStringList q;
            q << sId << cId;

            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rowCount);

            for(int i = 0; i<q.size(); i++)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setText(q.at(i));
                ui->tableWidget->setItem(rowCount, i, item);
            }
        }
        //修改格式
        int columnCount = ui->tableWidget->columnCount();
        int rowCount = ui->tableWidget->rowCount();

        for(int i = 0; i <columnCount; i++)
        {
            for(int j = 0; j < rowCount; j++)
            {
                ui->tableWidget->item(j, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
        }
    }
    else {
        QMessageBox::warning(this, "查询无果", "该学生没有选课信息！");
    }
}

//显示所有选课信息
void selectCourseManage::on_listAll_button_clicked()
{
    ui->currentSelectedLabel->setVisible(false);
    //ui->updata_button->setDisabled(true);
    ui->delete_button->setDisabled(true);
    ui->add_button->setDisabled(false);
    updateTable();
}

//按课程号查找
void selectCourseManage::on_searchByCId_button_clicked()
{
    ui->currentSelectedLabel->setVisible(false);
    //ui->updata_button->setDisabled(true);
    ui->delete_button->setDisabled(true);
    ui->add_button->setDisabled(false);
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    QString cId = ui->cId_lineEdit->text();

    if(cId.isEmpty())
    {
        QMessageBox::warning(this, "查询出错", "课程号信息不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.prepare("select cId from course where cId = ?");
    query.addBindValue(cId);
    query.exec();
    if(!query.next())
    {
        QMessageBox::warning(this, "查询出错", "该课程不存在！");
        return;
    }

    query.prepare("select sId,cId from student_course where cId = ? order by sId");
    query.addBindValue(cId);
    query.exec();

    if(query.size() != 0)
    {
        //填入数据
        while(query.next())
        {
            QString sId = query.value(0).toString();
            QString cId = query.value(1).toString();

            QStringList q;
            q << sId << cId;

            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rowCount);

            for(int i = 0; i<q.size(); i++)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setText(q.at(i));
                ui->tableWidget->setItem(rowCount, i, item);
            }
        }
        //修改格式
        int columnCount = ui->tableWidget->columnCount();
        int rowCount = ui->tableWidget->rowCount();

        for(int i = 0; i <columnCount; i++)
        {
            for(int j = 0; j < rowCount; j++)
            {
                ui->tableWidget->item(j, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
        }
    }
    else {
        QMessageBox::warning(this, "查询无果", "该课程没有选课记录！");
    }
}

//添加选课信息
void selectCourseManage::on_add_button_clicked()
{
    QString sId = ui->sNo_lineEdit->text();
    QString cId = ui->cId_lineEdit->text();

    ui->currentSelectedLabel->setVisible(false);
    //ui->updata_button->setDisabled(true);
    ui->delete_button->setDisabled(true);

    if(sId.isEmpty() || cId.isEmpty())
    {
        QMessageBox::warning(this, "添加出错", "学生号和课程号不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.prepare("select sId from student where sId = ?");
    query.addBindValue(sId);
    query.exec();

    if(!query.next())
    {
        QMessageBox::warning(this, "添加出错", "该学生不存在！");
        return;
    }

    query.prepare("select cId from course where cId = ?");
    query.addBindValue(cId);
    query.exec();

    if(!query.next())
    {
        QMessageBox::warning(this, "添加出错", "该课程不存在！");
        return;
    }
    //重复检查
    query.prepare("select sId, cId from student_course where sId = ? and cId = ?");
    query.addBindValue(sId);
    query.addBindValue(cId);
    query.exec();
    if(query.next())
    {
        QMessageBox::warning(this, "添加出错", "已存在该信息\n请勿重复添加！");
        return;
    }
    //插入数据
    query.prepare("insert into student_course (sID,cId,)"
                  "VALUES              (:sID,:cId) ");
    query.bindValue(":sID", sId);
    query.bindValue(":name", cId);
    bool isInsertSuccess = query.exec();

    if(isInsertSuccess)
    {
        QMessageBox::information(this, "提示", "添加成功！");
        updateTable();
    }
    else {
        QMessageBox::warning(this, "添加出错", "请检查输入信息！");
    }
}
//点击单元格
void selectCourseManage::on_tableWidget_cellClicked(int row, int column)
{
    column = 0;
    ui->add_button->setDisabled(true);
    ui->currentSelectedLabel->setVisible(true);;
    //ui->updata_button->setDisabled(false);
    ui->delete_button->setDisabled(false);
    QString sId = ui->tableWidget->item(row, 0)->text();
    QString cId = ui->tableWidget->item(row, 1)->text();
    ui->sNo_lineEdit->setText(sId);
    ui->cId_lineEdit->setText(cId);
}
//删除按钮
void selectCourseManage::on_delete_button_clicked()
{
    QString sId = ui->sNo_lineEdit->text();
    QString cId = ui->cId_lineEdit->text();
    //输入检测
    if(sId.isEmpty() || cId.isEmpty())
    {
        QMessageBox::warning(this, "删除出错", "学号和课程号不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);
    //对象检测
    query.prepare("select sId from student where sId = ?");
    query.addBindValue(sId);
    query.exec();
    if(!query.next())
    {
        QMessageBox::warning(this, "删除出错", "该学生不存在！");
        return;
    }
    //对象检测2
    query.prepare("select cId from course where cId = ?");
    query.addBindValue(cId);
    query.exec();
    if(!query.next())
    {
        QMessageBox::warning(this, "删除出错", "该课程不存在！");
        return;
    }
    //二次确认
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "确认删除", "确定删除该选课信息？", QMessageBox::Yes|QMessageBox::No);

    if(btn == QMessageBox::Yes)
    {
        query.prepare("delete from student_course where sId = ? and cId = ?");
        query.addBindValue(sId);
        query.addBindValue(cId);
        bool isDeleteSuccess = query.exec();

        if(isDeleteSuccess)
        {
            QMessageBox::information(this, "提示", "删除成功");
            ui->currentSelectedLabel->setVisible(false);
            ui->delete_button->setDisabled(true);
            ui->sNo_lineEdit->clear();
            ui->cId_lineEdit->clear();
            updateTable();
        }
        else {
            QMessageBox::warning(this, "删除失败", "请检查信息！");
        }
    }


}
//导出按钮
void selectCourseManage::on_export_button_clicked()
{
    Table2Excel(ui->tableWidget, "选课信息");
}
