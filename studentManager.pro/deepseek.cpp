#include "deepseek.h"
#include "ui_deepseek.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSslSocket>

//AI助手类
Deepseek::Deepseek(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Deepseek)
{
    ui->setupUi(this);
    setWindowTitle("桂林电子科技大学");
    setWindowIcon(QIcon(":/img/桂电小标"));
    ui->textEdit_output->setTextInteractionFlags(Qt::NoTextInteraction);
    manager = new QNetworkAccessManager;
}

Deepseek::~Deepseek()
{
    delete ui;
}

//发送
void Deepseek::on_pushButton_clicked()
{
    //qDebug() << "SSL支持状态：" << QSslSocket::supportsSsl();
    //获取文本框
    QString text = ui->textEdit_input->toPlainText().trimmed();
    ui->textEdit_input->clear();
    ui->textEdit_output->insertPlainText("-->"+text+"\n\n");
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.deepseek.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Authorization","Bearer sk-968179a54a5a4ced981c5dd2244d4e10");
    request.setRawHeader("Accept", "text/event-stream");
    request.setRawHeader("Cache-Control", "no-cache");
    request.setRawHeader("Connection", "keep-alive");

    QJsonArray messages;
    // 如果是第一次对话，添加系统消息
    if(conversationHistory.isEmpty()) {
        QJsonObject systemMsg;
        systemMsg["role"] = "system";
        systemMsg["content"] = "你是桂林电子科技大学的向导，你叫小桂，接下来请用中文回答";
        conversationHistory.append(systemMsg);
    }

    QJsonObject UserMsg;
    UserMsg["role"] ="user";
    UserMsg["content"] = text;
    conversationHistory.append(UserMsg);

    for(const QJsonObject &msg : conversationHistory) {
            messages.append(msg);
        }
    //messages.append(systemMsg);
    //messages.append(UserMsg);

    QJsonObject requestBody;
    requestBody["messages"] = messages;
    requestBody["model"] = "deepseek-chat";
    requestBody["frequency_penalty"]=0;
    requestBody["max_tokens"]=1024;
    requestBody["stream"]= true;
    requestBody["temperature"]= 1.3;
    requestBody["web_search"] = true;

    //发送请求
    QNetworkReply *reply = manager->post(request,QJsonDocument(requestBody).toJson());

    //处理数据
    connect(reply,&QNetworkReply::readyRead,this,[=](){
       //qDebug()<<reply->readAll();
       while(reply->canReadLine()){
           QString line =reply->readLine().trimmed();

           if(line.startsWith("data"))//以data开头的数据
           {
               line.remove(0,6);

               QJsonParseError error;
               QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(),&error);

               if(error.error == QJsonParseError::NoError)     //确认Json对象
               {
                   QString content = doc.object()["choices"].toArray().first().toObject()["delta"].
                                       toObject()["content"].toString();
                   if(!content.isEmpty()){
                       ui->textEdit_output->moveCursor(QTextCursor::End);
                       ui->textEdit_output->insertPlainText(content);
                   }
               }
           }
       }
    });
    qDebug() << "History:" << conversationHistory;

    connect(reply,&QNetworkReply::finished,this,[=]{
        // 获取AI的完整回复并添加到历史记录
        QString fullText = ui->textEdit_output->toPlainText();
        int lastUserInputPos = fullText.lastIndexOf("-->" + text);
        QString aiReply = fullText.mid(lastUserInputPos + text.length() + 2).trimmed();

        // 添加到历史记录
        if (!aiReply.isEmpty()) {
            QJsonObject aiMsg;
            aiMsg["role"] = "assistant";
            aiMsg["content"] = aiReply;
            conversationHistory.append(aiMsg);
        }

        ui->textEdit_output->moveCursor(QTextCursor::End);
        ui->textEdit_output->insertPlainText("\n\n\n\n");

        reply->deleteLater();
    });
}
