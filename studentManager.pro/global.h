#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QVariantList>
#include <QStringList>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QDialog>
#include <QCryptographicHash>

#include <QPointer>

extern QString sqlUser;
extern QString sqlPwd;
extern QString sqlLocalHost;

extern QString username_Current;
extern QString sName_Current;
extern QString pwd_Current;

extern QVariantList list_all_student;


class DatabaseManager {
public:
    static QSqlDatabase& connect_to_database();

};
//单例窗口模板
template <typename T>
T* CreateQWindow() {
    static T* instance = nullptr;
    if (!instance) {
        instance = new T();
        instance->setAttribute(Qt::WA_DeleteOnClose);
        QObject::connect(instance, &T::destroyed, []() {
           instance = nullptr;  // 释放后重置指针，允许下次重新创建
        });
    }
    instance->show();
    instance->raise();
    instance->activateWindow();
    return instance;
}

//哈希密码校验
extern bool verifyPassword(const QString &inputPassword, const QString &storedHash);

#endif // GLOBAL_H
