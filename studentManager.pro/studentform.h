#ifndef STUDENTFORM_H
#define STUDENTFORM_H

#include <QWidget>
#include <QStack>

namespace Ui {
class studentForm;
}

class studentForm : public QWidget
{
    Q_OBJECT

public:
    explicit studentForm(QWidget *parent = nullptr);
    ~studentForm();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_stu_findclass_inforButton_clicked();

    void on_pushButton_student_findscore_clicked();

    void on_stu_find_coursebutton_clicked();

    void on_Xiao_Gui_clicked();

private:
    Ui::studentForm *ui;
    QStack<QWidget*> stack;
    //QWidget *parentWindow;
};

#endif // STUDENTFORM_H
