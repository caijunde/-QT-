#include "mainwindow.h"
#include <QApplication>
#include <QSslSocket>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug() << "SSL supported:" << QSslSocket::supportsSsl();
    qDebug() << "SSL version:" << QSslSocket::sslLibraryVersionString();
    return a.exec();
}

//各文件功能简介
//addclass              管理员用-班级信息管理-添加班级
//addnewcourse          管理员用-课程信息管理-添加课程按钮
//addnewgrade           管理员用-成绩信息管理-添加成绩信息
//addstuinfo            管理员用-学生信息管理界面-添加学生信息
//all_stu_info          管理员用-学生信息管理界面-展示所有信息
//allclassinfo          管理员用-班级信息管理-查看所有班级
//allcourseinfo         管理员用-课程信息管理-查看所有课程按钮
//classinfomanage       管理员用-班级信息管理
//coursemanage          管理员用-课程信息管理
//exporttable           导出Excel的函数
//deepseek              学生用-登录页面-AI助手
//global                数据库参数,连接数据库过程,以及一些全局变量
//grademanage           管理员用-成绩信息管理
//gradestaticsbycourse  管理员用-成绩信息管理-课程成绩统计
//gradestaticsbystu     管理员用-成绩信息管理-学生个人成绩统计
//main                  当前页面-主函数
//mainwindow            登录页面，也为初始页面,哈希密码校验
//manage                管理员用-登录页面
//manager_manage        管理员用-管理员账户管理
//myclass               学生用-班级基本情况
//mycourse              学生用-课程查询
//myinfo                学生用-查看个人基本信息
//myscore               学生用-成绩查询
//resetpwd              学生用-查看个人基本信息-修改密码按钮
//searchcourse          学生用-查询其他课程
//selectcoursemanage    管理员用-选课信息管理
//studentform           学生用-登录页面
//stuinfomanage         管理员用-学生信息管理界面
