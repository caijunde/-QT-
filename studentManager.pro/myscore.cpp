#include "myscore.h"
#include "ui_myscore.h"
#include "global.h"
#include "studentform.h"

//学生用-成绩查询
myScore::myScore(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myScore)
{
    ui->setupUi(this);
    ui->sNo_lineEdit->setText(QString(username_Current));
    ui->sNo_lineEdit->setReadOnly(true);
    ui->Grade_lineEdit->setReadOnly(true);
    ui->className_Edit->setPlaceholderText("请输入课程名(模糊搜索)");
    ui->classTime_box->addItem("大一上");
    ui->classTime_box->addItem("大一下");
    ui->classTime_box->addItem("大二上");
    ui->classTime_box->addItem("大二下");
    ui->classTime_box->addItem("大三上");
    ui->classTime_box->addItem("大三下");
    setWindowTitle(QString("桂林电子科技大学"));
    setWindowIcon(QIcon(":/img/桂电小标"));
}

myScore::~myScore()
{
    delete ui;
}

//查询按钮
void myScore::on_findscoreButton_clicked()
{
    QString classTime = ui->classTime_box->currentText();
    QString sId = ui->sNo_lineEdit->text();
    QString className = ui->className_Edit->text();
    QString teacher = ui->teacher_lineEdit->text();

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QSqlQuery query(db);

    if (sId.isEmpty() || className.isEmpty() || teacher.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请填写完整信息！");
        return;
    }

    query.prepare("SELECT cId FROM course WHERE object LIKE ? AND time = ? AND teacher = ?");
    query.addBindValue("%" + className + "%");//模糊查询
    query.addBindValue(classTime);
    query.addBindValue(teacher);
    if (!query.exec()) {
        QMessageBox::warning(this, "数据库错误", "查询课程失败！");
        return;
    }
    if(!query.next()){
        QMessageBox::warning(this, "无查询结果", "该课程不存在！");
        ui->Grade_lineEdit->clear();
    }
    QString cId = query.value(0).toString();

    //查询成绩
    query.prepare("SELECT score FROM score WHERE sid = ? AND cid = ?");
    query.addBindValue(sId);
    query.addBindValue(cId);
    query.exec();
    //处理查询结果
    if (query.next()) {
            QString mygrade = query.value(0).toString();
            ui->Grade_lineEdit->setText(mygrade);
        } else {
            QMessageBox::warning(this, "无查询结果", "你的该课程没有成绩！");
        }
}
