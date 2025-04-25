#include "manager_manage.h"
#include "ui_manager_manage.h"
#include "global.h"
#include "stuinfomanage.h"
#include <QMessageBox>
#include <QSqlError>
#include <QProcess>
#include "mainwindow.h"
#include <QCryptographicHash>

//管理员账户管理
Manager_Manage::Manager_Manage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Manager_Manage)
{
    ui->setupUi(this);
    ui->error_label->clear();
    setWindowTitle(QString("桂林电子科技大学"));
    setWindowIcon(QIcon(":/img/桂电小标"));
}

Manager_Manage::~Manager_Manage()
{
    delete ui;
}

//重复密码校验->新密码
void Manager_Manage::on_newPwd_lineEdit_textChanged(const QString &arg1)
{
    QString newPwd = arg1;
    QString repeatPwd = ui->repeatPwd_lineEdit->text();
    if(newPwd.compare(repeatPwd) != 0)
    {
        ui->error_label->setText("两次密码输入不一致！");

    }
    else {
        ui->error_label->clear();
    }
}

//重复密码校验->重复密码
void Manager_Manage::on_repeatPwd_lineEdit_textChanged(const QString &arg1)
{
    QString repeatedPwd = arg1;
    QString newPwd = ui->newPwd_lineEdit->text();
    if(repeatedPwd.compare(newPwd) != 0)
    {
        ui->error_label->setText("两次密码输入不一致！");
    }
    else {
        ui->error_label->clear();
    }
}

//修改密码
void Manager_Manage::on_Change_password_clicked()
{
    //获取数据库连接
    QSqlDatabase db = DatabaseManager::connect_to_database();
    QSqlQuery query(db);
    //用户名
    QString adminName = username_Current;
    qDebug()<<adminName;
    //旧密码
    QString oldPwd = ui->oldPwd_lineEdit->text();
    //新密码
    QString newPwd = ui->newPwd_lineEdit->text();
    //输入验证
    if(oldPwd.isEmpty())
    {
        QMessageBox::warning(this, "输入错误", "请输入原密码！");
        ui->error_label->setText("请输入原密码！");
        return;
    }
    else if(newPwd.isEmpty())
    {
        QMessageBox::warning(this, "输入错误", "请输入新密码！");
        ui->error_label->setText("请输入新密码！");
        return;
    }else{
        ui->error_label->clear();
    }

    //旧密码加密处理
    QByteArray salt = getCaptcha(16);//生成盐值
    QString Hash_pwd = hashPasswordWithSalt(oldPwd,salt);//输入的原密码加密
    qDebug()<<pwd_Current;
    //比对新旧密码（不等通过）//比对原密码(相等通过)
    if(!verifyPassword(newPwd, pwd_Current)&&verifyPassword(oldPwd, pwd_Current))
    {
        //密码不一致
        ui->error_label->clear();
        //二次确认
        QMessageBox::StandardButton btn;
        btn = QMessageBox::question(this, "确认提交", "确定要修改个人密码吗？", QMessageBox::Yes|QMessageBox::No);
            if(btn == QMessageBox::Yes)
            {
                //新密码加密处理
                QByteArray salt = getCaptcha(16);//生成盐值
                QString New_Hash_pwd = hashPasswordWithSalt(newPwd,salt);//输入的原密码加密

                //插入数据
                query.prepare("update admin set password = ? where adminName = ?");
                query.addBindValue(New_Hash_pwd);
                query.addBindValue(adminName);
                bool isResetPwdSuccess = query.exec();

                if(isResetPwdSuccess)
                {
                    QMessageBox::information(this, "提示", "修改成功！请重新登录！", QMessageBox::Yes);
                    qApp->quit(); // 退出当前应用程序
                    QProcess::startDetached(qApp->arguments()[0], qApp->arguments()); // 重启程序

                }
                else {
                    QMessageBox::warning(this, "修改失败", "请检查密码格式！");
                }
            }else {
                QMessageBox::warning(this, "修改失败", "用户取消！");
            }
    } else {
        //密码一致
        QMessageBox::warning(this, "输入错误", "新旧密码一致或密码输入错误！");
        ui->oldPwd_lineEdit->clear();
        ui->newPwd_lineEdit->clear();
        ui->error_label->setText("请检查输入！");
    }

}

//添加管理员账户
void Manager_Manage::on_add_manager_clicked()
{
    QString a_manager = ui->account_for_add_manager->text();

    if(a_manager.isEmpty())
    {
        QMessageBox::warning(this, "警告", "输入信息不能留空！");
        return;
    }

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    QString originPwd = "123456";
    QByteArray salt = getCaptcha(16);//生成盐值
    QString Hash_pwd = hashPasswordWithSalt(originPwd,salt);

    //判断学号
    query.prepare("select adminName from admin where adminName = ?");
    query.addBindValue(a_manager);
    query.exec();

    if(query.next())
    {
        QMessageBox::warning(this, "新增出错", "该学号已经存在！");
        return;
    }

    //插入数据
    query.prepare("insert into admin (adminName,password)"
                  "VALUES              (:adminName,:password) "
                 );
    query.bindValue(":adminName", a_manager);
    query.bindValue(":password", Hash_pwd);

    bool isInsertSuccess = query.exec();

    if(isInsertSuccess)
        QMessageBox::information(this, "提示", "新增管理员成功！密码为123456");
    else {
        QMessageBox::warning(this, "新增失败", "请检查输入信息是否有误");
    }
}

//删除账户
void Manager_Manage::on_delete_Manager_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "确认删除", "确定要删除此学生信息吗？", QMessageBox::Yes|QMessageBox::No);
    if(btn == QMessageBox::Yes)
    {
        QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        QSqlQuery query(db);

        //当前管理员密码
        QString paw = ui->password_for_delete_and_reset->text();
        //目标管理员账号
        QString adminName = ui->account_for_delete_and_reset->text();

        //校验当前密码（正确进入）
        if(verifyPassword(paw,pwd_Current))
        {
            query.prepare("delete from admin where adminName = ?");
            query.addBindValue(adminName);
            bool isDeleteSuccess = query.exec();

            if(isDeleteSuccess)
            {
                QMessageBox::information(this, "提示", "删除成功");
            }
            else {
                QMessageBox::warning(this, "提示", "删除失败"+ query.lastError().text());
            }
        }else{
            QMessageBox::warning(this, "提示", "密码错误");
        }

    }
}


//重置密码
void Manager_Manage::on_reset_password_clicked()
{
    //二次验证
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "确认重置", "确定要重置此管理员密码吗？", QMessageBox::Yes|QMessageBox::No);

    if(btn == QMessageBox::Yes)
    {
        //当前管理员密码
        QString paw = ui->password_for_delete_and_reset->text();
        //目标管理员账号
        QString adminName = ui->account_for_delete_and_reset->text();
        // 获取全局唯一的数据库连接
        QSqlDatabase db = DatabaseManager::connect_to_database();
        QSqlQuery query(db);

        if(verifyPassword(paw,pwd_Current))
        {
            QString originPwd = "123456";
            QByteArray salt = getCaptcha(16);//生成盐值
            QString Hash_pwd = hashPasswordWithSalt(originPwd,salt);

            query.prepare("update admin set password = ? where adminName = ?");
            query.addBindValue(Hash_pwd);
            query.addBindValue(adminName);
            bool isResetPwdSuccess = query.exec();

            if(isResetPwdSuccess)
            {
                QMessageBox::information(this, "提示", "已将此管理员密码重置为初始密码123456");
                ui->password_for_delete_and_reset->clear();
                ui->account_for_delete_and_reset->clear();
            }
            else {
                QMessageBox::warning(this, "警告", "重置失败！");
            }
        }else{
            QMessageBox::warning(this, "提示", "密码错误");
        }
    }
}



