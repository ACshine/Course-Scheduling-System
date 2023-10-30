#include "selectcoursesdialog.h"
#include "ui_selectcoursesdialog.h"
#include"HashTable.h"
#include"HashTable2.h"
extern HashTable h1;
extern HashTable2 h2;
#define MAX_COURSES 300
extern QString nowCourses[MAX_COURSES];
extern int nowcnt;
QString selectCourses[MAX_COURSES];
int selectcnt;
extern int setID;//拓扑子集ID 从1开始
extern int res[MAX_COURSES][MAX_COURSES];
extern int everytermcnt[MAX_COURSES];//存储每学期安排的课程数
SelectCoursesDialog::SelectCoursesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectCoursesDialog)
{
    QString buttonStyleNoIcon = "QPushButton {"
                                "    color: #ffffff; /*文字颜色*/"
                                "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #aa55ff, stop: 1 #1296db); /*背景色*/"
                                "    border: none; /*边框风格*/"
                                "    border-radius: 10px; /*边框倒角*/"
                                "    font: bold 10px; /*字体*/"
                                "    font-family: Segoe UI;"
                                "    min-width: 40px; /*控件最小宽度*/"
                                "    min-height: 10px; /*控件最小高度*/"
                                "    padding: 4px; /*内边距*/"
                                "}"
                                "QPushButton:hover {"
                                "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ff55ff, stop: 1 #1296db); /*背景色*/"
                                "}"
                                "QPushButton:pressed {"
                                "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ff55ff, stop: 1 #aa00ff); /*背景色*/"
                                "}";
    ui->setupUi(this);
    ui->selectedCourses->clear();
    ui->showcourses_btn->setEnabled(true);
    ui->confirm_btn->setStyleSheet(buttonStyleNoIcon);
    ui->showcourses_btn->setStyleSheet(buttonStyleNoIcon);
    ui->add_btn->setStyleSheet(buttonStyleNoIcon);
    ui->selectedCourses->append("第" + QString::number(setID) + "学期：");
    connect(ui->selectedCourses, &QTextEdit::textChanged, this, &SelectCoursesDialog::updateshow);
    //显示已编排结果：
    for(int i=lastSet+1;i<=setID;i++)
    {
        if(i!=1)
        {
            QListWidgetItem *item = new QListWidgetItem("****************************");
            ui->havedlist->addItem(item);
        }
        for(int j=0;j<everytermcnt[i];j++)
        {
            QListWidgetItem *item = new QListWidgetItem(h2.find(res[i][j]));
            ui->havedlist->addItem(item);
        }


    }
    lastSet=setID;
}

void SelectCoursesDialog::updateshow()
{
    QString text = ui->selectedCourses->toPlainText();
    QStringList lines = text.split("\n");
    if (!text.isEmpty() && text != lastText)
    {
        QStringList lastLines = lastText.split('\n');
        for (const QString &line : lastLines)
        {
            if (!text.contains(line)&&h1.find(line)!=-1) {
                // 用户删除了这一行，将其添加到 QListWidget
                QListWidgetItem *item = new QListWidgetItem(line);
                ui->listWidget->addItem(item);
            }
        }
        // 更新上次的文本
        lastText = text;
    }
    for(auto&s:lines)
    {
        for (int i = 0; i < ui->listWidget->count(); ++i)
        {
            QListWidgetItem *item = ui->listWidget->item(i);
            QString itemText = item->text();
            if(itemText==s)
                delete item;
        }
    }
}
SelectCoursesDialog::~SelectCoursesDialog()
{
    delete ui;
}

void SelectCoursesDialog::on_showcourses_btn_clicked()
{
    for(int i=0;i<nowcnt;i++)
        ui->listWidget->addItem(nowCourses[i]);
    ui->showcourses_btn->setEnabled(false);
}


void SelectCoursesDialog::on_add_btn_clicked()
{
    QListWidgetItem *selectedItem = ui->listWidget->currentItem();
    if (selectedItem)
    {
        QString selectedText = selectedItem->text();
        delete selectedItem;
        ui->selectedCourses->append(selectedText);
    }
}


void SelectCoursesDialog::on_confirm_btn_clicked()
{
    selectcnt=0;
    QString text = ui->selectedCourses->toPlainText();

    // 将文本分割成行
    QStringList lines = text.split("\n");

    // 迭代每一行文本并添加非空行到数组
    for (const QString& line : lines)
    {
        if (!line.trimmed().isEmpty())
        {
           selectCourses[selectcnt++]=line.trimmed();
        }
    }
    this->hide();
}

