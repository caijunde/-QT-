#include "mainwindow.h"
#include "stuinfomanage.h"
#include "ui_stuinfomanage.h"
#include "global.h"
#include "manager.h"
#include <QMessageBox>
#include "addstuinfo.h"
#include "all_stu_info.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QSqlError>


//管理员用-学生信息管理界面
stuinfomanage::stuinfomanage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stuinfomanage)
{
    ui->setupUi(this);
    //禁用输入事件-修改、删除、重置
    ui->update_button->setDisabled(true);
    ui->delete_button->setDisabled(true);
    ui->reset_button->setDisabled(true);

    ui->sexBox->addItem("男");
    ui->sexBox->addItem("女");
    ui->statusBox->addItem("正常");
    ui->statusBox->addItem("冻结");
    setWindowTitle(QString("桂林电子科技大学"));
    setWindowIcon(QIcon(":/img/桂电小标"));
}

stuinfomanage::~stuinfomanage()
{
    delete ui;
}

//查询
void stuinfomanage::on_pushButton_clicked()
{
    QString sNo = ui->sNo_lineEdit->text();//导入输入框的学号

    if(sNo.isEmpty())
    {
        //禁用输入事件-修改、删除、重置
        ui->update_button->setDisabled(true);
        ui->delete_button->setDisabled(true);
        ui->reset_button->setDisabled(true);
        //重置8个信息框
        ui->sName_lineEdit->clear();
        ui->sexBox->setCurrentIndex(0);
        ui->age_lineEdit->clear();
        ui->major_lineEdit->clear();
        ui->classId_lineEdit->clear();
        ui->address_lineEdit->clear();
        ui->phone_lineEdit->clear();
        ui->statusBox->setCurrentIndex(0);

        QMessageBox::warning(this, "输入错误", "学号信息不能留空！");

        //设置焦点
        ui->sNo_lineEdit->setFocus();
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    //搜索
    query.prepare("select name,sex,age,major,class,address,phonenumber,status from student where sId = ?");
    query.addBindValue(sNo);
    query.exec();

    //搜索成功
    if(query.next())
    {
        QString name = query.value(0).toString();
        QString sex = query.value(1).toString();
        QString age = query.value(2).toString();
        QString major = query.value(3).toString();
        QString Class = query.value(4).toString();
        QString address = query.value(5).toString();
        QString phonenum = query.value(6).toString();
        QString status = query.value(7).toString();

        //qDebug() << status;
        //填入数据
        ui->sName_lineEdit->setText(name);
        ui->sexBox->setCurrentIndex(ui->sexBox->findText(sex));
        ui->age_lineEdit->setText(age);
        ui->major_lineEdit->setText(major);
        ui->classId_lineEdit->setText(Class);
        ui->address_lineEdit->setText(address);
        ui->phone_lineEdit->setText(phonenum);
        ui->statusBox->setCurrentIndex(ui->statusBox->findText(status));

        ui->update_button->setDisabled(false);
        ui->delete_button->setDisabled(false);
        ui->reset_button->setDisabled(false);
    }
    //失败
    else
    {
        //激活编辑按钮
        ui->update_button->setDisabled(true);
        ui->delete_button->setDisabled(true);
        ui->reset_button->setDisabled(true);

        ui->sName_lineEdit->clear();
        ui->sexBox->setCurrentIndex(0);
        ui->age_lineEdit->clear();
        ui->major_lineEdit->clear();
        ui->classId_lineEdit->clear();
        ui->address_lineEdit->clear();
        ui->phone_lineEdit->clear();
        ui->statusBox->setCurrentIndex(0);

        QMessageBox::warning(this, "查询无果", "该学生不存在");
        //重置焦点
        ui->sNo_lineEdit->setFocus();
    }
}

//修改信息按钮
void stuinfomanage::on_update_button_clicked()
{
    QString name = ui->sName_lineEdit->text();
    QString sex = ui->sexBox->currentText();
    QString age = ui->age_lineEdit->text();
    QString major = ui->major_lineEdit->text();
    QString Class = ui->classId_lineEdit->text();
    QString address = ui->address_lineEdit->text();
    QString phonenum = ui->phone_lineEdit->text();
    QString sId = ui->sNo_lineEdit->text();
    QString status = ui->statusBox->currentText();

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.prepare("select classId from class where classId = ?");
    query.addBindValue(Class);
    query.exec();

    //优先检查是否有班级
    if(query.next() == 0)
    {
        QMessageBox::warning(this, "修改失败", "该班级号不存在！");
        return;
    }
    //防注入修改数据流程
    query.prepare("UPDATE student SET "
                  "name = :name, "
                  "sex = :sex, "
                  "age = :age, "
                  "major = :major, "
                  "class = :class, "
                  "address = :address, "
                  "phonenumber = :phonenumber, "
                  "status = :status "
                  "WHERE sId = :sId");
    query.bindValue(":name", name);
    query.bindValue(":sex", sex);
    query.bindValue(":age", age);
    query.bindValue(":major", major);
    query.bindValue(":class", Class);
    query.bindValue(":address", address);
    query.bindValue(":phonenumber", phonenum);
    query.bindValue(":status", status);
    query.bindValue(":sId", sId);

    bool isUpdateSuccess = query.exec();

    if(isUpdateSuccess)
    {
        //qDebug() << status;
        QMessageBox::information(this, "提示", "修改成功");
    }
    else {
        QMessageBox::warning(this, "提示", "修改失败！请检查输入信息是否有误！");
    }
}

//添加学生信息
void stuinfomanage::on_addStuInfo_button_clicked()
{
    CreateQWindow<addStuInfo>();
}

//展示全部学生信息按钮
void stuinfomanage::on_listAllStu_button_clicked()
{
    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    query.exec("select sId,name,sex,age,major,class,address,phonenumber,status from student");
    while(query.next())
    {
        QStringList q;
        QString sId = query.value(0).toString();
        QString name = query.value(1).toString();
        QString sex = query.value(2).toString();
        QString age = query.value(3).toString();
        QString major = query.value(4).toString();
        QString classId = query.value(5).toString();
        QString address = query.value(6).toString();
        QString phonenum = query.value(7).toString();
        //QString password = query.value(8).toString();
        QString status = query.value(8).toString();
        q << sId << name << sex << age << major << classId << address << phonenum << status;
        //为展示全部学生信息做准备
        list_all_student.append(q);
    }

    CreateQWindow<all_stu_info>();
}

//删除学生信息
void stuinfomanage::on_delete_button_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "确认删除", "确定要删除此学生信息吗？", QMessageBox::Yes|QMessageBox::No);
    if(btn == QMessageBox::Yes)
    {
        QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        QSqlQuery query(db);

        QString sId = ui->sNo_lineEdit->text();

        query.prepare("delete from student where sId = ?");
        query.addBindValue(sId);
        bool isDeleteSuccess = query.exec();

        if(isDeleteSuccess)
        {
            QMessageBox::information(this, "提示", "删除成功");
            ui->sNo_lineEdit->clear();
            ui->sName_lineEdit->clear();
            ui->sexBox->setCurrentIndex(0);
            ui->major_lineEdit->clear();
            ui->classId_lineEdit->clear();
            ui->address_lineEdit->clear();
            ui->phone_lineEdit->clear();
            ui->statusBox->setCurrentIndex(0);
            //重置焦点
            ui->sNo_lineEdit->setFocus();

            ui->update_button->setDisabled(true);
            ui->delete_button->setDisabled(true);
            ui->reset_button->setDisabled(true);

        }
        else {
            QMessageBox::warning(this, "提示", "删除失败"+ query.lastError().text());
        }
    }
}

//重置密码按钮
void stuinfomanage::on_reset_button_clicked()
{
    //二次验证
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "确认重置", "确定要重置此学生密码吗？", QMessageBox::Yes|QMessageBox::No);

    if(btn == QMessageBox::Yes)
    {
        QString sId = ui->sNo_lineEdit->text();
        QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        QSqlQuery query(db);

        QString originPwd = "123456";
        QByteArray salt = getCaptcha(16);//生成盐值
        QString Hash_pwd = hashPasswordWithSalt(originPwd,salt);
        //qDebug()<<Hash_pwd;

        query.prepare("update student set password = ? where sId = ?");
        //qDebug()<<sId;
        query.addBindValue(Hash_pwd);
        query.addBindValue(sId);
        bool isResetPwdSuccess = query.exec();

        if(isResetPwdSuccess)
        {
            QMessageBox::information(this, "提示", "已将此学生密码重置为初始密码123456");
        }
        else {
            QMessageBox::warning(this, "警告", "重置失败！");
        }
    }
}

// 生成随机盐值
QByteArray getCaptcha(int length = 16)
{
    QByteArray salt;
        for (int i = 0; i < length; ++i) {
            salt.append(QRandomGenerator::global()->bounded(256)); // 生成 0-255 之间的随机字节
        }
        return salt.toBase64(); // 以 Base64 形式存储
}

// 生成带盐哈希密码，SHA-256加密
QString hashPasswordWithSalt(const QString &password, const QByteArray &salt) {
    QByteArray saltedPassword = salt + password.toUtf8(); // 盐值前置或后置
    QByteArray hashedPassword = QCryptographicHash::hash(saltedPassword, QCryptographicHash::Sha256);
    return QString(salt.toBase64() + ":" + hashedPassword.toHex()); // 组合存储
};
