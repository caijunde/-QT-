#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTime>
#include <QMouseEvent>
#include <QtDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <global.h>
#include <QCryptographicHash>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    //输入框内默认文字
    ui->Login_password->setEchoMode(QLineEdit::Password);
    ui->Login_username->setPlaceholderText("请输入学号或管理员账号");
    ui->Login_password->setPlaceholderText("请输入密码");
    //默认选择学生账号
    ui->studentButton->setChecked(true);

    loginFailTimes = 0;
    //设置左下角提示框
    ui->captcha_lineEdit->setVisible(false);
    ui->captcha_Label->setVisible(false);
    ui->loginFailTimes_Label->setVisible(false);
    ui->loginFailTimes_Label->setStyleSheet("color:#ff0000");

    ui->guanwang->setStyleSheet("QPushButton { "
                                "border: none; " // 去除边框
                                "background: transparent; "
                                "} "
                                "QPushButton:hover{"
                                "background-color: rgba(255, 255, 255, 180);"
                                "border-radius: 20px;"
                                "} "
                                );


    //随机数种子
    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());

    //4位随机字母验证码（大小写）
    captcha = getCaptcha();
    //四位随机颜色
    color = getColor();
    setWindowIcon(QIcon(":/img/桂电小标"));
    //验证码颜色刷新
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer.start(500);

}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow 被销毁";
    delete ui;
    //timer.stop();
}

//实时变幻验证码颜色函数
void MainWindow::onTimeOut()
{
    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
    color = getColor();//重新加载颜色
    update();//刷新小部件
}

//重新绘制验证码
void MainWindow::mousePressEvent(QMouseEvent *evt)
{
    int x = ui->captcha->x();  // 根据实际控件位置获取坐标
    int y = ui->captcha->y();
    if(QRect(x, y, 84, 24).contains(evt->pos()))
    {
        captcha = getCaptcha();
        repaint();
    }
}

//绘制函数
void MainWindow::paintEvent(QPaintEvent *evt)
{

    if(loginFailTimes >= 3)
    {
        QPainter painter(this);

        int x = ui->captcha->x();  // 根据实际控件位置获取坐标
        int y = ui->captcha->y();
        int Width =ui->captcha->width();
        int Height =ui->captcha->height();
        painter.fillRect(x, y, Width, Height, Qt::darkRed);//绘制底图（灰色部分）

        painter.setFont(QFont("Comic Sans MS"));

        for(int i = 0; i < 4; i++)//绘制字母
        {

            painter.setPen(color[i]);
            painter.drawText(x + 20 * i, y, 20, 24, Qt::AlignCenter, QString(captcha[i]));
        }

        for(int i = 0; i < 100; i++)//绘制噪点
        {
            painter.setPen(color[i % 4]);
            painter.drawPoint(x + qrand()%83, y + qrand() % 23);
        }
    }
}

//获得验证码
QString MainWindow::getCaptcha()
{
    QString ret = "";

    for(int i = 0; i < 4; i++)
    {
        int c = (qrand() % 2)? 'a' : 'A';//qrand生成随机数0或1，获得a或A
        ret += static_cast<QChar>(c + qrand() % 26);//取余26，获得随机的字母
    }

    return ret;
}

//获得验证码颜色
Qt::GlobalColor* MainWindow::getColor()
{
    static Qt::GlobalColor colors[4];

    for(int i = 0; i < 4; i++)
    {
        colors[i] = static_cast<Qt::GlobalColor>((qrand() % 16) + 2);
    }

    return colors;
}

//哈希密码校验
bool verifyPassword(const QString &inputPassword, const QString &storedHash) {
    QStringList parts = storedHash.split(":");//分割盐值
    if (parts.size() != 2) return false; // 格式错误
    QByteArray salt = QByteArray::fromBase64(parts[0].toUtf8());//盐值
    QByteArray expectedHash = QByteArray::fromHex(parts[1].toUtf8());//密码值

    QByteArray inputSalted = salt + inputPassword.toUtf8();
    QByteArray inputHashed = QCryptographicHash::hash(inputSalted, QCryptographicHash::Sha256);

    return inputHashed == expectedHash;
}

//密码校验
void MainWindow::on_LoginButton_clicked()
{
    QString input_userName = ui->Login_username->text().trimmed();
    QString input_password = ui->Login_password->text();
    QString input_captcha = ui->captcha_lineEdit->text();

    //链接数据库
    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
        if (!db.isOpen()) {
            QMessageBox::critical(this, "错误", "数据库未连接！");
            return;
        }
    QSqlQuery query(db);

    //验证码错误
    if(loginFailTimes >= 3)
    {
        if(input_captcha.compare(captcha) != 0)
        {
            QMessageBox::warning(this, "登录失败", "验证码错误！");
            return;
        }
    }

    //管理员被点击
    if(ui->adminButton->isChecked())
    {
        query.prepare("SELECT adminName FROM admin WHERE adminName = ?");
        query.addBindValue(input_userName);

        // 执行查询并检查是否成功
        if (!query.exec()) {
            qDebug() << "查询失败：" << query.lastError().text();
            QMessageBox::critical(this, "错误", "数据库查询失败！");
            return;
        }

        if(!query.next())
        {
            qDebug()<<input_userName;
            QMessageBox::warning(this, "登录失败", "该用户不存在！");
            return;
        }

        QString Hash_pwd;
        QString userName;
        //查询
        query.prepare("SELECT aid, adminName, password FROM admin WHERE adminName = ? ");
        query.addBindValue(input_userName);
        query.exec();

        if (query.next()) {
            Hash_pwd = query.value("password").toString();
            userName = query.value("adminName").toString();
        }else{
            qDebug() << "未找到用户";
            return;
        }
        qDebug()<<Hash_pwd;

        if(verifyPassword(input_password,Hash_pwd))
        {
            //读取数据库中的管理员数据
            username_Current = userName;
            pwd_Current = Hash_pwd;

            this->hide();
            CreateQWindow<manager>();
        }else{
            QMessageBox::warning(this, "登录失败", "密码错误！");
            ui->Login_username->setFocus();
        }

    }

    //点击学生按钮情况
    if(ui->studentButton->isChecked())
    {
        //用户查询
        query.prepare("SELECT sId, name, password, status FROM student WHERE sId = ?");//防注入预处理
        query.addBindValue(input_userName);
        if(!query.exec() || !query.next())
        {
            QMessageBox::warning(this, "登录失败", "该用户不存在！");
            return;
        }

        //检查是否被冻结
        QString status = query.value("status").toString();
        if(status == "冻结")
        {
            QMessageBox::warning(this, "登录失败", "该用户已被冻结\n请联系管理员进行解冻！");
            return;
        }

        //错误计数
        bool isLoginSuccess = false;
        //登录
        query.prepare("SELECT sID, name, password FROM student WHERE sID = ?");
        query.addBindValue(input_userName);
        query.exec();
        QString Hash_pwd;
        QString sNo;
        QString sName;

        if (query.next()) {
            Hash_pwd = query.value("password").toString();
        }else{
            qDebug() << "未找到用户";
            return;
        }

        if(verifyPassword(input_password, Hash_pwd))
        {
            sNo = query.value("sID").toString();
            sName = query.value("name").toString();

            username_Current = sNo;
            pwd_Current = Hash_pwd;
            sName_Current = sName;

            isLoginSuccess = true;

            //打开studentform页面
            this->hide();
            CreateQWindow<studentForm>();
        }

        //密码错误
        if(!isLoginSuccess)
        {
            QMessageBox::warning(this, "警告", "密码错误！");
            loginFailTimes++;
            ui->Login_username->setFocus();
            ui->loginFailTimes_Label->setVisible(true);
            ui->loginFailTimes_Label->setText(QString("登录失败次数：%0 次").arg(loginFailTimes));
        }
    }

    if(loginFailTimes >= 3)//登录失败超过三次，显示验证码块
    {
        ui->captcha_Label->setVisible(true);
        ui->captcha_lineEdit->setVisible(true);
    }

    //失败过多冻结流程
    if(loginFailTimes >= 5 && ui->studentButton->isChecked())
    {
        QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
            if (!db.isOpen()) {
                QMessageBox::critical(this, "错误", "数据库未连接！");
                return;
            }
        QSqlQuery query(db);

        QString username = ui->Login_username->text();

        query.prepare("update student set status = '冻结' where sId = ?");
        query.addBindValue(username);
        query.exec();

        ui->LoginButton->setDisabled(true);
        ui->clearButton->setDisabled(true);

        qDebug() << "用户" << username << "因登录失败次数过多被冻结。";
        QMessageBox::warning(this, "登录失败", "登录失败次数过多\n您的账户已被冻结！\n请联系管理员进行解冻");

    }

}

//清除按钮
void MainWindow::on_clearButton_clicked()
{
    ui->Login_username->clear();
    ui->Login_password->clear();
    ui->captcha_lineEdit->clear();
}

//官网按钮
void MainWindow::on_guanwang_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.guet.edu.cn/"));
}
