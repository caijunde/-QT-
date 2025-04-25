#ifndef MANAGER_MANAGE_H
#define MANAGER_MANAGE_H

#include <QDialog>

namespace Ui {
class Manager_Manage;
}

class Manager_Manage : public QDialog
{
    Q_OBJECT

public:
    explicit Manager_Manage(QWidget *parent = nullptr);
    ~Manager_Manage();

private slots:
    void on_Change_password_clicked();

    void on_delete_Manager_clicked();

    void on_add_manager_clicked();

    void on_reset_password_clicked();

    void on_repeatPwd_lineEdit_textChanged(const QString &arg1);

    void on_newPwd_lineEdit_textChanged(const QString &arg1);

private:
    Ui::Manager_Manage *ui;
};

#endif // MANAGER_MANAGE_H
