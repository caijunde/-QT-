#include "exporttable.h"
#include "QDebug"

//导出Excel函数
void Table2Excel(QTableWidget *table, QString title)//传入当前可见表格和"所有学生信息"
{
    //弹出保存文件页面，选择保存地址
    QString fileName = QFileDialog::getSaveFileName(
                table, "保存",
                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                "Excel 文件(*.xls *.xlsx)"
                );
    if(fileName != "")//点击取消fileName返回空
    {
        QAxObject *excel = new QAxObject;
        if (!excel)
        {
         qDebug() << "无法创建 Excel 应用程序对象！";
         QMessageBox::warning(NULL,"错误","未能创建 Excel 对象，请检查是否安装Microsoft Excel",QMessageBox::Apply);
        }
        if(excel->setControl("Excel.Application"))//调用Excel的ActiveX接口，绑定在excel类上
            {
                //调整execl为不可见，后台运行
                excel->dynamicCall("SetVisible (bool Visible)","false");
                //关闭警告窗
                excel->setProperty("DisplayAlerts", false);

                //获取所有工作簿
                QAxObject *workbooks = excel->querySubObject("WorkBooks");
                //添加新工作簿
                workbooks->dynamicCall("Add");
                //获取激活的工作簿
                QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
                //获取第一个激活的工作表
                QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

                int i,j;
                //获取表格长宽
                int colcount = table->columnCount();
                int rowcount = table->rowCount();
                //设置标题
                QAxObject *cell,*col;
                cell=worksheet->querySubObject("Cells(int,int)", 1, 1);
                cell->dynamicCall("SetValue(const QString&)", title);
                cell->querySubObject("Font")->setProperty("Size", 18);
                //调整行高
                worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
                //合并标题行
                QString cellTitle;
                cellTitle.append("A1:");
                cellTitle.append(QChar(colcount - 1 + 'A'));
                cellTitle.append(QString::number(1));
                QAxObject *range = worksheet->querySubObject("Range(const QString&)", cellTitle);

                //自动换行
                range->setProperty("WrapText", true);
                //合并单元格
                range->setProperty("MergeCells", true);
                //水平居中
                range->setProperty("HorizontalAlignment", -4108);
                //垂直居中
                range->setProperty("VerticalAlignment", -4108);
                //设置表头
                for(i=0;i<colcount;i++)
                {
                    //将列索引转变为字母
                    QString columnName;
                    columnName.append(QChar(i + 'A'));
                    columnName.append(":");
                    columnName.append(QChar(i + 'A'));
                    //获得列对象
                    col = worksheet->querySubObject("Columns(const QString&)", columnName);
                    //设置列宽度
                    col->setProperty("ColumnWidth", table->columnWidth(i)/6);

                    cell = worksheet->querySubObject("Cells(int,int)", 2, i+1);
                    //获取原表中第i个文本
                    columnName = table->horizontalHeaderItem(i)->text();
                    //写入文本
                    cell->dynamicCall("SetValue(const QString&)", columnName);
                    //设置单元格格式
                    cell->querySubObject("Font")->setProperty("Bold", true);
                    cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
                    cell->setProperty("HorizontalAlignment", -4108);//惯例水平居中
                    cell->setProperty("VerticalAlignment", -4108);//惯例垂直居中
                }
                //遍历单元格
                for(i=0;i<rowcount;i++)
                {
                    for (j=0;j<colcount;j++)
                    {
                        worksheet->querySubObject("Cells(int,int)", i+3, j+1)//从第三行，第1列起
                                 ->dynamicCall("SetValue(const QString&)", //写入语句
                                               table->item(i,j)?table->item(i,j)->text():"");//根据单元格是否为空判断执行语句
                        cell = worksheet->querySubObject("Cells(int,int)", i+3, j+1);
                        if(j==0){
                            cell->setProperty("NumberFormat", "0");//将第一列设置为文本格式
                        }
                        cell->setProperty("HorizontalAlignment", -4108);//惯例水平居中
                        cell->setProperty("VerticalAlignment", -4108);//惯例垂直居中
                    }
                }

                //设置边框
                QString lrange;
                lrange.append("A2:");
                lrange.append(colcount - 1 + 'A');
                lrange.append(QString::number(table->rowCount() + 2));
                range = worksheet->querySubObject("Range(const QString&)", lrange);
                range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
                range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
                //调整行高
                QString rowsName;
                rowsName.append("2:");
                rowsName.append(QString::number(table->rowCount() + 2));
                range = worksheet->querySubObject("Range(const QString&)", rowsName);
                range->setProperty("RowHeight", 20);
                //保存退出excel
                workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));
                workbook->dynamicCall("Close()");
                excel->dynamicCall("Quit()");
                delete excel;

                excel = NULL;

                if (QMessageBox::question(NULL,"完成","文件已经导出，是否现在打开？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
                {
                     QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
                }
            }
    }else
    {
        QMessageBox::warning(NULL,"错误","用户取消。",QMessageBox::Apply);
    }
}
