#include "myinfo.h"
#include "ui_myinfo.h"
#include "global.h"
#include "studentform.h"
#include "resetpwd.h"
#include "QMessageBox"

//学生用-查看个人基本信息
myInfo::myInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myInfo)
{
    ui->setupUi(this);

    QString sId = username_Current;

    setWindowTitle(QString("桂林电子科技大学"));
    setWindowIcon(QIcon(":/img/桂电小标"));

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        if (!db.isOpen()) {
            QMessageBox::critical(this, "错误", "数据库未连接！");
            return;
        }
    QSqlQuery query(db);

    query.prepare("select sId,name,sex,age,major,class,address,phonenumber from student where sId = ?");
    query.addBindValue(sId);
    query.exec();

    while(query.next())
    {
        ui->sNo_lineEdit->setText(query.value(0).toString());
        QString name = query.value(1).toString();
        QString sex = query.value(2).toString();
        QString age = query.value(3).toString();
        QString major = query.value(4).toString();
        QString Class = query.value(5).toString();
        QString address = query.value(6).toString();
        QString phone = query.value(7).toString();

        ui->name_lineEdit->setText(name);
        ui->sex_lineEdit->setText(sex);
        ui->age_lineEdit->setText(age);
        ui->major_lineEdit->setText(major);
        ui->address_lineEdit->setText(address);
        ui->phone_lineEdit->setText(phone);
    }
}

myInfo::~myInfo()
{
    delete ui;
}

//重置密码按钮
void myInfo::on_resetPwd_button_clicked()
{
    CreateQWindow<resetPwd>();
}
