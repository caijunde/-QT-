#include "allclassinfo.h"
#include "ui_allclassinfo.h"
#include "global.h"
#include "classinfomanage.h"
#include "exporttable.h"

//班级信息管理-查看所有班级
allClassInfo::allClassInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::allClassInfo)
{
    ui->setupUi(this);
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.exec("select classId,className,major,sNum,master from class");
    while(query.next())//逐行读取
    {
        QString classId = query.value(0).toString();
        QString className = query.value(1).toString();
        QString major = query.value(2).toString();
        QString sNum = query.value(3).toString();
        QString master =query.value(4).toString();

        QStringList q;
        q << classId << className << major << sNum << master;

        int rowCount = ui->classInfoTable->rowCount();
        ui->classInfoTable->insertRow(rowCount);
        //插入数据
        for(int i = 0; i<q.size(); i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(q.at(i));
            ui->classInfoTable->setItem(rowCount, i, item);
        }
    }

    int columnCount = ui->classInfoTable->columnCount();//列
    int rowCount = ui->classInfoTable->rowCount();//行

    //遍历表格，设置对齐
    for(int i = 0; i <columnCount; i++)
    {
        for(int j = 0; j < rowCount; j++)
        {
            ui->classInfoTable->item(j, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

allClassInfo::~allClassInfo()
{
    delete ui;
}

//导出为excel
void allClassInfo::on_export_button_clicked()
{
    Table2Excel(ui->classInfoTable, "所有班级信息");
}
