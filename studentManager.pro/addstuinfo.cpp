#include "addstuinfo.h"
#include "ui_addstuinfo.h"
#include "global.h"
#include "stuinfomanage.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

//学生信息内-添加学生信息
addStuInfo::addStuInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addStuInfo)
{
    ui->setupUi(this);
    ui->sexBox->addItem("男");
    ui->sexBox->addItem("女");
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));
}

addStuInfo::~addStuInfo()
{
    delete ui;
}

//清空按钮
void addStuInfo::on_clear_button_clicked()
{
    ui->sNo_lineEdit->clear();
    ui->sName_lineEdit->clear();
    ui->sexBox->setCurrentIndex(0);
    ui->age_lineEdit->clear();
    ui->major_lineEdit->clear();
    ui->address_lineEdit->clear();
    ui->classId_lineEdit->clear();
    ui->phonenum_lineEdit->clear();
}
//添加按钮
void addStuInfo::on_submit_button_clicked()
{
    QString sNo = ui->sNo_lineEdit->text();
    QString name = ui->sName_lineEdit->text();
    QString sex = ui->sexBox->currentText();
    QString age = ui->age_lineEdit->text();
    QString major = ui->major_lineEdit->text();
    QString address = ui->address_lineEdit->text();
    QString classId = ui->classId_lineEdit->text();
    QString phone = ui->phonenum_lineEdit->text();

    if(sNo.isEmpty() || name.isEmpty() || age.isEmpty() || major.isEmpty() ||
            address.isEmpty() || classId.isEmpty() || phone.isEmpty())
    {
        QMessageBox::warning(this, "警告", "输入信息不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    QString originPwd = "123456";
    QByteArray salt = getCaptcha(16);//生成盐值
    QString Hash_pwd = hashPasswordWithSalt(originPwd,salt);
    //qDebug()<<Hash_pwd;

    //判断学号
    query.prepare("select sId from student where sId = ?");
    query.addBindValue(sNo);
    query.exec();

    if(query.next())
    {
        QMessageBox::warning(this, "新增出错", "该学号已经存在！");
        return;
    }

    //判断班号
    query.prepare("select classId from class where classId = ?");
    query.addBindValue(classId);
    query.exec();
    if(!query.next())
    {
        QMessageBox::warning(this, "新增出错", "该班级号不存在！");
        return;
    }

    //插入数据
    query.prepare("insert into student (sID,name,sex,age,major,class,address,phonenumber,password,status)"
                  "VALUES              (:sID,:name,:sex,:age,:major,:class,:address,:phonenumber,:password,:status) "
                 );
    query.bindValue(":sID", sNo);
    query.bindValue(":name", name);
    query.bindValue(":sex", sex);
    query.bindValue(":age", age);
    query.bindValue(":major", major);
    query.bindValue(":class", classId);
    query.bindValue(":address", address);
    query.bindValue(":phonenumber", phone);
    query.bindValue(":password", Hash_pwd);
    query.bindValue(":status", "正常");

    bool isInsertSuccess = query.exec();

    if(isInsertSuccess)
        QMessageBox::information(this, "提示", "新增学生成功！密码为123456");
    else {
        QMessageBox::warning(this, "新增失败", "请检查输入信息是否有误");
    }

}
