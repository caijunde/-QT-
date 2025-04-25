#include "addclass.h"
#include "ui_addclass.h"
#include "global.h"
#include "classinfomanage.h"
#include <QMessageBox>

//班级信息管理-添加班级
addClass::addClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addClass)
{
    ui->setupUi(this);
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));
}

addClass::~addClass()
{
    delete ui;
}

//确认添加
void addClass::on_submit_button_clicked()
{
    QString classId = ui->classId_lineEdit->text();
    QString className = ui->className_lineEdit->text();
    QString classMajor = ui->classMajor_lineEdit->text();
    QString classNum = ui->classNum_lineEdit->text();
    QString classMaster = ui->classMaster_lineEdit->text();

    if(classId.isEmpty() || className.isEmpty() || classMajor.isEmpty() || classNum.isEmpty() || classMaster.isEmpty())
    {
        QMessageBox::warning(this, "输入出错", "班级信息不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        if (!db.isOpen()) {
            QMessageBox::critical(this, "错误", "数据库未连接！");
            return;
        }
    QSqlQuery query(db);

    //query.exec("select classId from class where class = '" + classId + "'");
    query.prepare("select classId from class where class = ? ");
    query.addBindValue(classId);
    query.exec();
    //检查班号
    if(query.next())
    {
        QMessageBox::warning(this, "添加出错", "该班级号已存在！");
        return;
    }
    //插入新数据
    query.prepare("insert into class    ( classId, className, Major,      sNo,      Master)"
                  "values               (:classId,:className,:classMajor,:classNum,:classMaster)"
                  );
    query.bindValue(":classId",classId);
    query.bindValue(":className",className);
    query.bindValue(":classMajor",classMajor);
    query.bindValue(":classNum",classNum);
    query.bindValue(":classMaster",classMaster);
    //bool isInsertSuccess = query.exec(QString("insert into class values('%0','%1','%2','%3','%4')").arg(classId).arg(className).arg(classMajor).arg(classNum).arg(classMaster));
    bool isInsertSuccess = query.exec();
    if(isInsertSuccess)
    {
        QMessageBox::information(this, "提示", "添加成功！");
    }
    else {
        QMessageBox::warning(this, "添加出错", "请检查输入信息！");
    }
}
