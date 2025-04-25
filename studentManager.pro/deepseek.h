#ifndef DEEPSEEK_H
#define DEEPSEEK_H

#include <QDialog>
#include <QNetworkAccessManager>

namespace Ui {
class Deepseek;
}

class Deepseek : public QDialog
{
    Q_OBJECT

public:
    explicit Deepseek(QWidget *parent = nullptr);
    ~Deepseek();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Deepseek *ui;
    QNetworkAccessManager *manager;
    QList<QJsonObject> conversationHistory;

};

#endif // DEEPSEEK_H
