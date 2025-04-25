#ifndef MYCLASS_H
#define MYCLASS_H

#include <QWidget>
#include <QStringList>
#include <QtCharts>
#include <QBarSeries>
#include <QAreaSeries>
#include <QBarSet>
#include <QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class myclass;
}

class myclass : public QWidget
{
    Q_OBJECT

public:
    explicit myclass(QWidget *parent = nullptr);
    ~myclass();

private slots:

    void on_JPG_Button_clicked();

    void on_PDF_Button_clicked();

private:
    Ui::myclass *ui;

    QChart *m_chart =nullptr;//Qchart指针
    QBarCategoryAxis * m_axisX = nullptr;//轴对象指针
    QValueAxis * m_axisY = nullptr;//轴对象指针

    //QLineSeries * m_lineSeries = nullptr;//折线对象指针
    QBarSeries * bar = nullptr;//柱形对象指针
private:
    void initChart_bar();
};

#endif // MYCLASS_H
