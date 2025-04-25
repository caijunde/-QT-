#include "global.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

//数据库参数
QString sqlUser = "root";
QString sqlPwd = "root";
QString sqlLocalHost = "127.0.0.1";
QString username_Current = "";
QString pwd_Current = "";
QString sName_Current = "";
QVariantList list_all_student;

QSqlDatabase& DatabaseManager::connect_to_database()
{
    //链接现成的数据库
    static QSqlDatabase db= QSqlDatabase::database("qt_sql_default_connection", false);
    //判断是否连接上数据库
    if (!db.isValid()) {
        //发现没有链接，链接数据库，名字设置为默认
        db = QSqlDatabase::addDatabase("QMYSQL", "qt_sql_default_connection");
        //登录SQL步骤
        db.setHostName(sqlLocalHost);
        db.setPort(3306);
        db.setUserName(sqlUser);
        db.setPassword(sqlPwd);
        db.setDatabaseName("student_manager");
    }
    //检验登录是否成功
    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
    } else {
        qDebug() << "Database connected successfully";
    }
    return db;
};

