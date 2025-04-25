#include "myclass.h"
#include "ui_myclass.h"
#include <mainwindow.h>
#include "studentform.h"
#include "global.h"
#include <QtCharts/QChartView>
#include <QtPrintSupport/QPrinter>
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>

//学生用-班级基本情况
myclass::myclass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myclass)
{
    ui->setupUi(this);
    ui->classId_lineEdit->setReadOnly(true);
    ui->className_lineEdit->setReadOnly(true);
    ui->classMajor_lineEdit->setReadOnly(true);
    ui->classNum_lineEdit->setReadOnly(true);
    ui->classMaster_lineEdit->setReadOnly(true);
    setWindowTitle(QString("桂林电子科技大学"));
    setWindowIcon(QIcon(":/img/桂电小标"));

    QSqlDatabase db = DatabaseManager::connect_to_database(); // 获取全局唯一的数据库连接
    QString uid = username_Current;
    qDebug()<<uid;
    QString myClass = "";
    QSqlQuery query(db);

    query.prepare("select class from student where sId = ?");
    query.addBindValue(uid);
    query.exec();

    if(query.next())
    {
        myClass = query.value(0).toString();
        qDebug()<<myClass;
    }

    query.prepare("select classId, className, major, sNum, master from class where classId = ?");
    query.addBindValue(myClass);
    query.exec();

    if(query.next())
    {
        QString cId = query.value(0).toString();
        QString cName = query.value(1).toString();
        QString cMajor = query.value(2).toString();
        QString cNum = query.value(3).toString();
        QString cMaster = query.value(4).toString();

        ui->classId_lineEdit->setText(cId);
        ui->className_lineEdit->setText(cName);
        ui->classMajor_lineEdit->setText(cMajor);
        ui->classNum_lineEdit->setText(cNum);
        ui->classMaster_lineEdit->setText(cMaster);
    }

    initChart_bar();
    QBarSet *set = new QBarSet("选课人数");
    QMap<int, int> studentCountMap;  // 存储所有课程的选课人数,第一个是cid，第二个是选课人数
    query.prepare("SELECT DISTINCT cId FROM course");
    if (query.exec()) {
        while (query.next()) {
            int cId = query.value(0).toInt();
            studentCountMap[cId] = 0;  // 先默认所有课程的选课人数为 0
        }
    } else {
        qDebug() << "获取课程列表失败:" << query.lastError().text();
        return;
    }

    //统计选课人数，并更新到studentCountMap
    query.prepare("SELECT cId, COUNT(*) FROM student_course GROUP BY cId");
    if (query.exec()) {
        while (query.next()) {
            int cId = query.value(0).toInt();
            int studentCount = query.value(1).toInt();
            studentCountMap[cId] = studentCount;  // 更新选课人数
        }
    } else {
        qDebug() << "统计选课人数失败:" << query.lastError().text();
        return;
    }
    //按照cId顺序填充QBarSet
    for (auto it = studentCountMap.begin(); it != studentCountMap.end(); ++it) {
        *set << it.value();  // 添加选课人数到柱状图
        qDebug() << "课程 ID:" << it.key() << " 选课人数:" << it.value();
    }
    //搞定
    bar->append(set);

}

myclass::~myclass()
{
    delete ui;
}

void myclass::initChart_bar()//柱形图初始化
{
    //创建图标数学模型
    if(m_chart == nullptr){
        m_chart = new QChart();
    }
    //创建一个柱型对象
    if(bar == nullptr){
        bar = new QBarSeries();
        bar->setName("Bar");
        m_chart->addSeries(bar);
    }
    //创建一个X轴对象
    if(m_axisX == nullptr)
    {
        m_axisX = new QBarCategoryAxis(this);//轴对象

        QStringList categories = {"计算机网络", "Java程序", "微积分I-1","微积分I-2","大学物理C","XML设计基础","数据结构与算法"};
                                    //101,       102,         103,      104,       105,         106,        107
        m_axisX->append(categories);
        m_axisX->setGridLineVisible(true);//格子线可见
    }
    //创建一个Y轴对象
    if(m_axisY == nullptr)
    {
        m_axisY = new QValueAxis();//轴对象
        m_axisY->setRange(0,10);//Y轴范围
        m_axisY->setGridLineVisible(true);//格子线可见
        m_axisY->setTickCount(11);//大小刻度参数
        m_axisY->setMinorTickCount(2);
    }
    //将X和Y添加到QChart中
    m_chart->addAxis(m_axisX,Qt::AlignBottom);
    m_chart->addAxis(m_axisY,Qt::AlignLeft);
    //将柱型对象（bar对象）附加到X轴和Y轴上
    bar->attachAxis(m_axisX);
    bar->attachAxis(m_axisY);

    //隐藏图列
    m_chart->legend()->hide();
    //将数学模型和图表试图绑定
    ui->graphicsView->setChart(m_chart);
    //抗锯齿
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

void myclass::on_JPG_Button_clicked()
{
    QPixmap screenshot = QPixmap::grabWidget(ui->graphicsView);
    QString filePath = QFileDialog::getSaveFileName(nullptr, "保存截图", "C:/", "PNG 文件 (*.png);;所有文件 (*.*)");
    if (!filePath.isEmpty()) {
        screenshot.save(filePath);
        qDebug() << "截图已保存至：" << filePath;
    }
}

void myclass::on_PDF_Button_clicked()
{
    QPrinter printer(QPrinter::PrinterResolution);//高分辨率
    QString filePath = QFileDialog::getSaveFileName(nullptr, "保存PDF", "C:/", "PDF 文件 (*.pdf);;所有文件 (*.*)");
    if (!filePath.isEmpty()) {
        printer.setOutputFileName(filePath);
        printer.setOutputFormat(QPrinter::PdfFormat);
        QPainter painter(&printer);
        ui->graphicsView->render(&painter);
        qDebug() << "截图已保存至：" << filePath;
    }
}
