#include "mycourse.h"
#include "ui_mycourse.h"
#include "studentform.h"
#include "global.h"
#include "searchcourse.h"
#include "exporttable.h"

//学生用-课程查询
mycourse::mycourse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mycourse)
{
    ui->setupUi(this);
    setWindowTitle(QString("桂林电子科技大学"));
    setWindowIcon(QIcon(":/img/桂电小标"));

    QString sId = username_Current;
    QStringList myCourseIdList;

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.prepare("select cId from student_course where sId = ?");
    query.addBindValue(sId);
    query.exec();

    while(query.next())
    {
        QString getCId = query.value(0).toString();

        //qDebug() << getCId;

        myCourseIdList << getCId;
    }

    QVariantList myCourseList;

    for(auto i : myCourseIdList)
    {
        query.prepare("select cId, object, teacher, time, location, period, credit from course where cId = ?");
        query.addBindValue(i);
        query.exec();
        while(query.next())
        {
            QString courseId = query.value(0).toString();
            QString object = query.value(1).toString();
            QString teacher = query.value(2).toString();
            QString time = query.value(3).toString();
            QString location = query.value(4).toString();
            QString period = query.value(5).toString();
            QString credit = query.value(6).toString();
            QStringList q;
            q << courseId << object << teacher << time << location << period << credit;
            myCourseList.append(q);
        }
    }

    //插入数据
    for(auto i : myCourseList)
    {
        int rowCount = ui->myCourseTable->rowCount();

        ui->myCourseTable->insertRow(rowCount);

        QStringList rowData = i.toStringList();

        for (int j = 0; j < rowData.size(); j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(rowData.at(j));
            ui->myCourseTable->setItem(rowCount, j, item);
        }
    }
    //设置格式
    int columnCount = ui->myCourseTable->columnCount();
    int rowCount = ui->myCourseTable->rowCount();

    for(int i = 0; i <columnCount; i++)
    {
        for(int j = 0; j < rowCount; j++)
        {
            ui->myCourseTable->item(j, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }


}

mycourse::~mycourse()
{
    delete ui;
}

//查询其他课程
void mycourse::on_searchButton_clicked()
{
    CreateQWindow<searchCourse>();
}

//导出excel
void mycourse::on_export_button_clicked()
{
    Table2Excel(ui->myCourseTable, "我的课程信息");
}
