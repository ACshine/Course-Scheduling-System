#include "addcoursedialog.h"
#include "ui_addcoursedialog.h"
#include"Course.h"
#include"HashTable.h"
#include"HashTable2.h"
#include"Graphl.h"
#include<QMessageBox>
#define MAX_COURSES 300
extern double credits[MAX_COURSES];
extern Graphl g;
extern HashTable h1;
extern HashTable2 h2;
extern bool valid[MAX_COURSES];
extern Course courses[MAX_COURSES];
extern int courseNum;//当前课程总数
extern int courseID;
extern int needcnt[MAX_COURSES];
extern int needthis[MAX_COURSES][MAX_COURSES];
extern bool Compulsory[MAX_COURSES];
bool is_appear_1(QString name)//判断当前课程名是否出现过
{
    for(int i=1;i<=courseNum;i++)
        if(courses[i].name==name)
            return true;
    return false;
}
AddCourseDialog::AddCourseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCourseDialog)
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
    ui->cancel_btn->setStyleSheet(buttonStyleNoIcon);
    ui->save_btn->setStyleSheet(buttonStyleNoIcon);
    ui->precourses_val->setPlaceholderText("无先修课程请填无或不填");
}

AddCourseDialog::~AddCourseDialog()
{
    delete ui;
}
bool has(int v)
{
    if (g.Mark[v])
    {
        return true;
    }
    g.Mark[v] = 1;
    for (Edge e = g.FirstEdge(v); g.IsEdge(e); e = g.NextEdge(e))
        if (has(e.to)) return true;
    g.Mark[v] = 0;//回溯
    return false;
}
bool checkRound()//检查是否含有回路
{
    for (int i = 1; i <=courseNum; i++)
    {
        int id=courses[i].id;
        g.Mark[id] = 0;
    }

    for (int i = 1; i <= courseNum; i++)
    {
        int id=courses[i].id;
        if (!g.Mark[id] && has(id))//遍历每个连通块找环
            return true;
    }
    return false;
}
//先修课程必须已经出现，并且添加完后图中不能有环
void AddCourseDialog::on_save_btn_clicked() //add_course
{
    QString name=ui->name_val->text();
    if(is_appear_1(name))
    {
        QMessageBox::warning(this, tr("添加失败"), tr("该课程已存在！"));
        this->hide();
        ui->precourses_val->clear();
        ui->coursetype_val->clearEditText();
        ui->name_val->clear();
        ui->credit_val->cleanText();
        return;
    }
    if(name.isEmpty())
    {
        QMessageBox::warning(this, tr("添加失败"), tr("课程名称不能为空！"));
        this->hide();
        ui->precourses_val->clear();
        ui->coursetype_val->clearEditText();
        ui->name_val->clear();
        ui->credit_val->cleanText();
        return;
    }
    double credit=ui->credit_val->value();
    if(credit==0)
    {
        QMessageBox::warning(this, tr("添加失败"), tr("学分不能为0！"));
        this->hide();
        ui->precourses_val->clear();
        ui->coursetype_val->clearEditText();
        ui->name_val->clear();
        ui->credit_val->cleanText();
        return;
    }
    int id;
    if(h1.find(name)==-1)
    {
        id=courseID++;
        //同时更新哈希表
        h1.insert(name,id);
        h2.insert(id,name);
    }
    else
        id=h1.find(name);

    QString textContent = ui->precourses_val->toPlainText();
    // 使用空格或换行符进行分割
    QRegularExpression regex("[\n\\s]+");
    QStringList precourses = textContent.split(regex, Qt::SkipEmptyParts);
    for (auto&c:precourses)
    {
        int preCourse=h1.find(c);//图中存的都是ID
        if(preCourse==-1)
        {
            QMessageBox::warning(this, tr("添加失败"), tr("当前存在未出现的先修课程！"));
            this->hide();
            ui->precourses_val->clear();
            ui->coursetype_val->clearEditText();
            ui->name_val->clear();
            ui->credit_val->cleanText();
            return;
        }
        if(preCourse==id)
        {
            QMessageBox::warning(this, tr("添加失败"), tr("当前存在环形依赖"));
            this->hide();
            ui->precourses_val->clear();
            ui->coursetype_val->clearEditText();
            ui->name_val->clear();
            ui->credit_val->cleanText();
            return;
        }
    }

    QString type=ui->coursetype_val->currentText();
    bool isCompulsory;
    if(type=="必修课程")
        isCompulsory=true;
    else
        isCompulsory=false;
    if(isCompulsory)
        Compulsory[id]=true;


    int preCoursesNum=precourses.count();
    int* preCourses = new int[preCoursesNum+10];
    valid[id]=true;
    int preCoursesCount = 0; // 用于跟踪有效的先修课程数量
    for (auto&c:precourses)
    {
        if (c=="无"||c.isEmpty())
            break;
        int preCourse=h1.find(c);//图中存的都是ID
        g.setEdge(preCourse, id, 0);
        preCourses[++preCoursesCount] = preCourse; //先修课程数组下标从1起
        needthis[preCourse][needcnt[preCourse]++]=id;
    }
    // 创建Course对象并存储
    Course c(id, name, credit, preCoursesCount, preCourses, isCompulsory);
    courses[++courseNum] = c;//下标从1起
    this->hide();
    ui->precourses_val->clear();
    ui->coursetype_val->clearEditText();
    ui->name_val->clear();
    ui->credit_val->cleanText();
    QMessageBox successBox;
    successBox.setIcon(QMessageBox::Information); // 设置图标为信息图标
    successBox.setWindowTitle("Success");
    successBox.setText("添加成功");
    successBox.exec(); // 显示对话框
}
void AddCourseDialog::on_cancel_btn_clicked()
{
    this->hide();
}

