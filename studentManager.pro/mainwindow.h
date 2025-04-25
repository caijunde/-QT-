#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <manager.h>
#include <studentform.h>
#include <QTimer>
#include <QSqlDatabase>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_LoginButton_clicked();

    void on_clearButton_clicked();

    void onTimeOut();

    void on_guanwang_clicked();

private:
    Ui::MainWindow *ui;
    manager *m;
    studentForm *f;
    QString captcha;//验证码
    Qt::GlobalColor *color;
    void paintEvent(QPaintEvent* evt);
    void mousePressEvent(QMouseEvent *evt);
    QString getCaptcha();
    Qt::GlobalColor* getColor();
    QTimer timer;
    int loginFailTimes;
};


#endif // MAINWINDOW_H
