#include "all_stu_info.h"
#include "ui_all_stu_info.h"
#include "stuinfomanage.h"
#include "exporttable.h"

//学生信息内-展示所有信息
all_stu_info::all_stu_info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::all_stu_info)
{
    ui->setupUi(this);
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));


    for(auto i : list_all_student)//stuinfomanage.cpp内传入
    {
        int rowCount = ui->tableWidget->rowCount();//计算表宽

        ui->tableWidget->insertRow(rowCount);//插入空的一排

        QStringList rowData = i.toStringList();//传入数据

        for (int j = 0; j < rowData.size(); j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(rowData.at(j));//设置单元格文本
            ui->tableWidget->setItem(rowCount, j, item);//插入表格
        }
    }

    //获取表格长宽
    int columnCount = ui->tableWidget->columnCount();
    int rowCount = ui->tableWidget->rowCount();

    //将表内单元格设置垂直、上下居中
    for(int i = 0; i <columnCount; i++)
    {
        for(int j = 0; j < rowCount; j++)
        {
            ui->tableWidget->item(j, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

all_stu_info::~all_stu_info()
{
    delete ui;
}

//导出Excel,详见exporttable
void all_stu_info::on_export_button_clicked()
{
    Table2Excel(ui->tableWidget, "所有学生信息");
}
