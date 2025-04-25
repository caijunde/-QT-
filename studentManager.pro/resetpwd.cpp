#include "resetpwd.h"
#include "ui_resetpwd.h"
#include "stuinfomanage.h"
#include "global.h"
#include "myinfo.h"
#include <QMessageBox>
#include <QProcess>
#include "mainwindow.h"
#include <QCryptographicHash>

//学生用-查看个人基本信息-修改密码按钮
resetPwd::resetPwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::resetPwd)
{
    ui->setupUi(this);
    ui->submit_button->setDisabled(true);
    ui->error_label->clear();
    ui->error_label->setStyleSheet("color:#ff0000");
    setWindowTitle(QString("桂林电子科技大学"));
    setWindowIcon(QIcon(":/img/桂电小标"));
}

resetPwd::~resetPwd()
{
    delete ui;
}

void resetPwd::on_newPwd_lineEdit_textChanged(const QString &arg1)
{
    QString newPwd = arg1;
    QString repeatedPwd = ui->repeatPwd_lineEdit->text();
    qDebug()<<newPwd;
    qDebug()<<repeatedPwd;
    if(newPwd.compare(repeatedPwd) != 0)
    {
        ui->error_label->setText("两次密码输入不一致！");
        ui->submit_button->setDisabled(true);

    }
    else {
        ui->error_label->clear();
        ui->submit_button->setDisabled(false);
    }
}
//重复密码
void resetPwd::on_repeatPwd_lineEdit_textChanged(const QString &arg1)
{
    QString repeatedPwd = arg1;
    QString newPwd = ui->newPwd_lineEdit->text();
    if(repeatedPwd.compare(newPwd) != 0)
    {
        ui->error_label->setText("两次密码输入不一致！");
        ui->submit_button->setDisabled(true);

    }
    else {
        ui->error_label->clear();
        ui->submit_button->setDisabled(false);
    }
}
//提交按钮
void resetPwd::on_submit_button_clicked()
{
    //获取数据库连接
    QSqlDatabase db = DatabaseManager::connect_to_database();
    QSqlQuery query(db);
    //用户名
    QString sId = username_Current;
    //旧密码
    QString oldPwd = ui->oldPwd_lineEdit->text();
    //新密码
    QString newPwd = ui->newPwd_lineEdit->text();
    //输入验证
    if(oldPwd.isEmpty())
    {
        QMessageBox::warning(this, "输入错误", "请输入原密码！");
        ui->error_label->setText("请输入原密码！");
    }
    else if(newPwd.isEmpty())
    {
        QMessageBox::warning(this, "输入错误", "请输入新密码！");
        ui->error_label->setText("请输入新密码！");
    }else{
        ui->error_label->clear();
    }

    //旧密码加密处理
    QByteArray salt = getCaptcha(16);//生成盐值
    QString Hash_pwd = hashPasswordWithSalt(oldPwd,salt);//输入的原密码加密

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
                query.prepare("update student set password = ? where sId = ?");
                query.addBindValue(New_Hash_pwd);
                query.addBindValue(sId);
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
        ui->repeatPwd_lineEdit->clear();
        ui->error_label->setText("请检查输入！");
    }
}
