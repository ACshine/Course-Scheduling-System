#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>
#include"Graphl.h"
#include<cmath>
#include <QFileDialog>
#include<QMessageBox>
#include<fstream>
#include<string>
#include <sstream>
#include<iostream>
#include <QFile>
#include <QTextStream>
#include <QListWidgetItem>
#include <QKeyEvent>
#include"Graphl.h"
#include "newwindow.h"
#include"Course.h"
#include"HashTable.h"
#include"HashTable2.h"
#include"queue.h"
#include "coursegraphwindow.h"
#define INF 0x3f3f3f3f
#define MAX_COURSES 300
const int N = 310;
int res[MAX_COURSES][MAX_COURSES];
int D[N];
int ans[N], cnt;//存拓扑序列
bool st[N];
extern double credits[N];
extern int Depth[N];
extern Graphl g;
extern int needcnt[MAX_COURSES];
extern HashTable h1;
extern HashTable2 h2;
extern bool valid[N];
extern Course courses[MAX_COURSES];
extern int courseNum;//当前课程总数
extern int courseID;
extern bool Compulsory[MAX_COURSES];
extern int needthis[MAX_COURSES][MAX_COURSES];
int state;
int setNum;//共编排的学期数
bool search_state;
int maxclasshours;
int setID=1;//拓扑子集ID 从1开始
QString nowCourses[MAX_COURSES];
int nowcnt;
int everytermcnt[MAX_COURSES];//存储每学期安排的课程数
extern QString selectCourses[MAX_COURSES];
extern int selectcnt;
bool is_appear(QString name)//判断当前课程名是否出现过
{
    for(int i=1;i<=courseNum;i++)
        if(courses[i].name==name)
            return true;
    return false;
}
bool readCourseFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        if (parts.size()>=4)
        {
            QString name = parts[0];
            if(is_appear(name))
                continue;
            int id;
            if(h1.find(name)==-1) id=courseID++;
            else
                id=h1.find(name);
            double credit = parts[1].toDouble();
            bool isCompulsory=parts[2].toInt();
            if(isCompulsory) Compulsory[id]=true;
            int preCoursesNum = parts.size();
            credits[id]=credit;
            int* preCourses = new int[preCoursesNum+10];
            int preCoursesCount = 0; // 用于跟踪有效的先修课程数量
            valid[id]=true;
            for (int i = 3; i < parts.size(); ++i)
            {
                QString s = parts[i];
                if(s=="无")
                    break;
                int preCourse=h1.find(s);//图中存的都是ID
                if (preCourse != -1)
                {
                    g.setEdge(preCourse, id, 0);
                    preCourses[++preCoursesCount] = preCourse; //先修课程数组下标从1起
                }
                else
                {
                    h1.insert(s,courseID);
                    h2.insert(courseID,s);
                    g.setEdge(courseID, id, 0);
                    preCourse=courseID;
                    preCourses[++preCoursesCount] = courseID; //先修课程数组下标从1起
                    courseID++;
                }
                needthis[preCourse][needcnt[preCourse]++]=id;
            }
            // 创建Course对象并存储
            Course c(id, name, credit, preCoursesCount, preCourses, isCompulsory);
            courses[++courseNum] = c;//下标从1起
        }
        else
        {
            file.close();
            return false;
        }
    }
    //qDebug() <<g.numEdge;
    file.close();
    return true;
}

int MainWindow::ExtendTopSort(double credit[])//credit存储所有课程的学分
{
    memset(everytermcnt,0,sizeof everytermcnt);
    for(int i=1;i<=courseNum;i++)
    {
        int id=courses[i].id;
        if(!valid[id]) continue;
        g.Indegree[id]=courses[i].preCoursesNum;
    }
    int classhours = 0;//当前总学时
    queue<int>s(100), t(100);
    bool st[N];
    memset(st, false, sizeof st);
    setID=1;
    nowcnt=0;
    for (int i = 1; i <= courseNum; i++)
    {   int id=courses[i].id;
        if (valid[id] && g.Indegree[id] == 0)
        {
            QString courseName = courses[i].name;
            nowCourses[nowcnt++]=courseName;
        }
    }
    SelectCoursesDialog selectcoursesdialog;
    selectcoursesdialog.exec();

    for(int i=1;i<selectcnt;i++) //第一行为提示信息！
    {
        int id=h1.find(selectCourses[i]);
        s.push(id);//先让用户手动选择的课程进队
        st[id]=true;
    }
    //其它入读为0的点入队
    for (int i = 1; i <= courseNum; i++)
    {   int id=courses[i].id;
        if (valid[id] && !st[id]&&g.Indegree[id] == 0)
        {
            s.push(id);//先让用户手动选择的课程进队
            st[id]=true;
        }
    }
    int fg=1;//为1在s栈
    //先用数组存当前新的入度为0的课程，让用户手动选择，这样，在排下学期课程时，会优先编排。如果满了，就都统一往后推
    nowcnt=0;
    while ((fg == 1 && !s.empty()) || (fg == 2 && !t.empty()))
    {
        if (fg == 1)
        {
            int v = s.front();
            if (classhours + credit[v] * 16 <= maxclasshours) //1学分等于16学时
            {
                s.pop();
                classhours += credit[v] * 16;
                res[setID][everytermcnt[setID]++] = v;//出栈时加入集合
                for (Edge e = g.FirstEdge(v); g.IsEdge(e); e = g.NextEdge(e))
                {
                    int to = e.to;
                    if (valid[to]&&--g.Indegree[to]==0)
                    {
                        nowCourses[nowcnt++]=h2.find(to);
                        //t.push(to);
                    }
                }
                if (s.empty())
                {
                    setID++;
                    if(nowcnt)
                    {
                        SelectCoursesDialog selectcoursesdialog;
                        selectcoursesdialog.exec();
                        memset(st,false,sizeof st);
                        for(int i=1;i<selectcnt;i++) //第一行为提示信息！
                        {
                            int id=h1.find(selectCourses[i]);
                            t.push(id);//先让用户手动选择的课程进队
                            st[id]=true;
                        }
                        for(int i=0;i<nowcnt;i++)
                        {
                            int id=h1.find(nowCourses[i]);
                            if(!st[id])
                            {
                                st[id]=true;
                                t.push(id);
                            }
                        }
                    }
                    fg = 2;
                    classhours = 0;
                    nowcnt=0;
                }
            }
            else//本学期已经无法再修其它课程了，统一往后延迟
            {
                while (!s.empty())
                {
                    nowCourses[nowcnt++]=h2.find(s.front());
                    s.pop();
                }
                setID++;
                if(nowcnt)
                {
                    SelectCoursesDialog selectcoursesdialog;
                    selectcoursesdialog.exec();
                    memset(st,false,sizeof st);
                    for(int i=1;i<selectcnt;i++) //第一行为提示信息！
                    {
                        int id=h1.find(selectCourses[i]);
                        t.push(id);//先让用户手动选择的课程进队
                        st[id]=true;
                    }
                    for(int i=0;i<nowcnt;i++)
                    {
                        int id=h1.find(nowCourses[i]);
                        if(!st[id])
                        {
                            st[id]=true;
                            t.push(id);
                        }
                    }
                }

                fg = 2;
                classhours = 0;
                nowcnt=0;
            }
        }
        else
        {
            int v = t.front();
            if (classhours + credit[v] * 16 <= maxclasshours)
            {
                t.pop();
                classhours += credit[v] * 16;
                res[setID][everytermcnt[setID]++] = v;//出栈时加入集合
                for (Edge e = g.FirstEdge(v); g.IsEdge(e); e = g.NextEdge(e))
                {
                    int to = e.to;
                    if (valid[to]&&--g.Indegree[to]==0)
                    {
                        nowCourses[nowcnt++]=h2.find(to);
                        //s.push(to);
                    }
                }
                if (t.empty())
                {
                    setID++;
                    if(nowcnt)
                    {
                        SelectCoursesDialog selectcoursesdialog;
                        selectcoursesdialog.exec();
                        memset(st,false,sizeof st);
                        for(int i=1;i<selectcnt;i++) //第一行为提示信息！
                        {
                            int id=h1.find(selectCourses[i]);
                            s.push(id);//先让用户手动选择的课程进队
                            st[id]=true;
                        }
                        for(int i=0;i<nowcnt;i++)
                        {
                            int id=h1.find(nowCourses[i]);
                            if(!st[id])
                            {
                                st[id]=true;
                                s.push(id);
                            }
                        }
                    }

                    fg = 1;
                    classhours = 0;
                    nowcnt=0;
                }
            }
            else//本学期已经无法再修其它课程了，统一往后延迟
            {
                while (!t.empty())
                {
                    nowCourses[nowcnt++]=h2.find(t.front());
                    t.pop();
                    //t.push(s.front());
                }
                setID++;
                if(nowcnt)
                {
                    SelectCoursesDialog selectcoursesdialog;
                    selectcoursesdialog.exec();
                    memset(st,false,sizeof st);
                    for(int i=1;i<selectcnt;i++) //第一行为提示信息！
                    {
                        int id=h1.find(selectCourses[i]);
                        s.push(id);//先让用户手动选择的课程进队
                        st[id]=true;
                    }
                    for(int i=0;i<nowcnt;i++)
                    {
                        int id=h1.find(nowCourses[i]);
                        if(!st[id])
                        {
                            st[id]=true;
                            s.push(id);
                        }
                    }
                }
                fg = 1;
                classhours = 0;
                nowcnt=0;
            }
        }
    }
    return setID - 1;
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->search_btn) {
        if (event->type() == QEvent::Enter) {
            QIcon originalIcon(":/icons/search_un.png");

            ui->search_btn->setIcon(originalIcon);
        } else if (event->type() == QEvent::Leave) {
             QIcon newIcon(":/icons/search.png");
            ui->search_btn->setIcon(newIcon);
        }
    }
    return false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

    QString exitButtonStyle = "QPushButton{"
                               "background-color: red;"
                               "color: white;"
                                 "border: none;"  // 移除边框
                               "border-radius: 5px;"
                               "padding: 5px 10px;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: darkred;"
                          "    min-width: 40px; /*控件最小宽度*/"
                          "    min-height: 10px; /*控件最小高度*/"
                               "}";



    // 设置表格样式
    QString style = "QTableWidget {"
                    "background-color: #f0f0f0;" // 表格背景色
                    "alternate-background-color: #e0e0e0;" // 交替行背景色
                    "border: 1px solid #d0d0d0;" // 表格边框
                    "gridline-color: #d0d0d0;" // 网格线颜色
                    "}"

                    // 设置表头样式
                    "QHeaderView::section {"
                    "background-color: #4c7da4;" // 表头背景色
                    "color: white;" // 表头文字颜色
                    "border: 1px solid #34495e;" // 表头边框
                    "}"

                    // 设置表格项样式
                    "QTableWidget::item {"
                    "background-color: white;" // 单元格背景色
                    "color: #333;" // 单元格文字颜色
                    "border: 1px solid #d0d0d0;" // 单元格边框
                    "}"

                    "QTableWidget::item:hover {"
                    "background-color: #f5f5f5;" // 鼠标悬停时的背景色
                    "}"

                    "QTableWidget::item:selected {"
                    "background-color: #e0e0e0;" /* 更深的选中单元格的背景色 */
                    "border: 1px solid #ff8c00;" /* 选中单元格的边框颜色，橙色 */
      "}";
    QString buttonStyle = "QPushButton {"
                          "    border: none;" // 去掉边框
                          "    background: transparent;" // 使背景透明
                          "}";
    ui->setupUi(this);
    ui->treeWidget->expandAll();
    ui->search_val->setPlaceholderText("请输入课程名称");
    ui->tableWidget->setStyleSheet(style);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setStyleSheet(style);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->add_course_btn->setStyleSheet(buttonStyleNoIcon);
    ui->search_btn->setStyleSheet(buttonStyle);
    QIcon originalIcon(":/icons/search.png");  // 使用资源文件中的路径
    ui->search_btn->setIcon(originalIcon);
    ui->open_file->setStyleSheet(buttonStyleNoIcon);
    ui->exit_button->setStyleSheet(exitButtonStyle);
    ui->show_graph_btn->setStyleSheet(buttonStyleNoIcon);
    ui->arrangement_btn->setStyleSheet(buttonStyleNoIcon);
    ui->reset_btn->setStyleSheet(buttonStyleNoIcon);
    ui->reset_btn->setEnabled(false);
    // 去掉按钮的文本
    ui->search_btn->setText("");
    QTreeWidgetItem *courseShow = ui->treeWidget->topLevelItem(0)->child(0); // 获取第一个子项目
    QTreeWidgetItem *courseArrangement = ui->treeWidget->topLevelItem(0)->child(1); // 获取第二个子项目
    courseShow->setData(0, Qt::UserRole, 0); // 设置标识为0，表示课程显示页面
    courseArrangement->setData(0, Qt::UserRole, 1); // 设置标识为1，表示课程安排页面
    QObject::connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::on_treeWidget_itemClicked);
    QObject::connect(ui->tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onCellClicked(int, int)));

    QTableWidget *tableWidget = ui->tableWidget;

    // 设置表格的上下文菜单策略为CustomContextMenu，这样只有在右键点击时才会触发
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // 连接表格的customContextMenuRequested信号到自定义的槽
    connect(tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    ui->search_btn->installEventFilter(this);

    // 在事件过滤器中实现悬浮效果

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_F6)//F6刷新样式
    {
        QFile f;
        auto str=QCoreApplication::applicationDirPath();
        f.setFileName(str+"//"+"myqss.css");
        f.open(QIODevice::ReadOnly);
        QString s=f.readAll();
        this->setStyleSheet(s);
    }
}
void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    int pageIndex = item->data(column, Qt::UserRole).toInt();
    ui->system->setCurrentIndex(pageIndex);
}
void MainWindow::on_open_file_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择txt文件"), QDir::homePath(), tr("文本文件 (*.txt)"));
    if (!filePath.isEmpty())
    {
        bool readSuccess = readCourseFile(filePath);
        if (readSuccess)
        {
            QMessageBox::information(this, tr("成功"), tr("文件读取成功！"));
            updateCourseTable();
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("文件读取失败，请检查文件格式"));
        }
    }
}
void MainWindow::on_add_course_btn_clicked()
{
    addcourse_dlg.exec();
    if(state==0||state==3)
        updateSelectCourseTable();
    else
        updateSelectCourseTable();
}
void MainWindow::updateCourseTable()//输出courses数组中的所有课
{
    showCourseTableHeader();
    ui->tableWidget->setRowCount(courseNum);
    for(int i=1;i<=courseNum;i++)
    {
        auto&c=courses[i];
        showCourseTableItem(c,i);
    }
}
void MainWindow::updateSelectCourseTable()
{
    QString name=ui->search_val->text();
    showCourseTableHeader();
    int cnt1=0,cnt2=0;
    for(int i=1;i<=courseNum;i++)
    {
        cnt1+=courses[i].isCompulsory;
        cnt2+=!courses[i].isCompulsory;
    }
    if(state==0||state==3)
    {
        if(!search_state)
            updateCourseTable();
        else
        {
            bool find=false;
            ui->tableWidget->setRowCount(1);
            for(int i=1;i<=courseNum;i++)
            {
                auto&c=courses[i];
                if(c.name==name)
                {
                    showCourseTableItem(c,1);
                    find=true;
                    break;
                }
            }
            if(!find) ui->tableWidget->setRowCount(0);
        }
    }
    else if(state==1)//必修选了，选修未选
    {
        if(!search_state)
        {
            int idx=0;
            ui->tableWidget->setRowCount(cnt1);
            for(int i=1;i<=courseNum;i++)
            {
                auto&c=courses[i];
                if(!c.isCompulsory) continue;
                showCourseTableItem(c,idx+1);
                idx++;
            }
        }
        else
        {
            bool find=false;
            for(int i=1;i<=courseNum;i++)
            {
                auto&c=courses[i];
                if(c.isCompulsory&&c.name==name)
                {
                    ui->tableWidget->setRowCount(1);
                    find=true;
                    showCourseTableItem(c,1);
                    break;
                }
            }
            if(!find)
            {
                showCourseTableHeader();
                ui->tableWidget->setRowCount(0);
            }
        }
    }
    else if(state==2)
    {
        if(!search_state)
        {
            int idx=0;
            ui->tableWidget->setRowCount(cnt2);
            for(int i=1;i<=courseNum;i++)
            {
                auto&c=courses[i];
                if(c.isCompulsory) continue;
                showCourseTableItem(c,idx+1);
                idx++;
            }
        }
        else
        {
            bool find=false;
            for(int i=1;i<=courseNum;i++)
            {
                auto&c=courses[i];
                if(!c.isCompulsory&&c.name==name)
                {
                    ui->tableWidget->setRowCount(1);
                    find=true;
                    showCourseTableItem(c,1);
                    break;
                }
            }
            if(!find)
            {
                showCourseTableHeader();
                ui->tableWidget->setRowCount(0);
            }
        }
    }
}
void MainWindow::showCourseTableHeader()
{
    ui->tableWidget->clearContents();
    QStringList headerLabels;
    headerLabels << "序号"<<"课程ID"<<"课程名称" <<"学分"<<"课程性质"<<"先修课程";
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
}
void MainWindow::showCourseArrangeTableHeader()
{
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setColumnCount(setNum);
    // 设置表头内容
    for (int i = 0; i < setNum; i++)
    {
        QString headerText = QString("学期%1").arg(i + 1);
        ui->tableWidget_2->setHorizontalHeaderItem(i, new QTableWidgetItem(headerText));
    }

}
void MainWindow::showCourseTableItem(Course c, int row)
{
    ui->tableWidget->setItem(row-1,0,new QTableWidgetItem(QString::number(row)));
    ui->tableWidget->setItem(row-1,1,new QTableWidgetItem(QString::number(c.id)));
    ui->tableWidget->setItem(row-1,2,new QTableWidgetItem(c.name));
    ui->tableWidget->setItem(row-1,3,new QTableWidgetItem(QString::number(c.credits)));
    QString Compulsory;
    if(c.isCompulsory)
        Compulsory="必修";
    else
        Compulsory="选修";
    ui->tableWidget->setItem(row-1,4,new QTableWidgetItem(Compulsory));
    QString precoursesString;
    if(c.preCoursesNum==0)
    {
        precoursesString="无";
    }
    else
    {
        for (int j = 1; j <= c.preCoursesNum;j++)
        {

            QString s=h2.find(c.preCourses[j]);
            precoursesString += s;
            if (j <= c.preCoursesNum - 1)
            {
                precoursesString += " "; // 在非最后一个元素后面添加空格
            }
        }
    }
    ui->tableWidget->setItem(row-1,5,new QTableWidgetItem(precoursesString));
}


void MainWindow::on_select_compulsory_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)//只展示必修课
    {
        if(state==0||state==2) state++;
    }
    else
    {
        if(state==1||state==3) state--;
    }
    updateSelectCourseTable();
}

void MainWindow::on_select_elective_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)//只展示选修课
    {
        if(state==0)
            state=2;
        else if(state==1)
            state=3;
    }
    else
    {
        if(state==2)
            state=0;
        else if(state==3)
            state=1;
    }
    updateSelectCourseTable();
}

void MainWindow::on_search_btn_clicked()//搜索 不受必修/选修checkbox影响
{
    QString goal=ui->search_val->text();
    if(goal.isEmpty())
    {
        QMessageBox::information(nullptr,"警告","搜索名称不能为空！");
        search_state=false;
    }
    else
    {
        search_state=true;
        if(state==0||state==3)
        {
            bool isfind=false;
            showCourseTableHeader();
            ui->tableWidget->setRowCount(1);
            for(int i=1;i<=courseNum;i++)
            {
                auto&c=courses[i];
                if(c.name==goal)
                {
                    showCourseTableItem(c,1);
                    isfind=true;
                    break;
                }
            }
            if(!isfind)
            {
                showCourseTableHeader();
                ui->tableWidget->setRowCount(0);
                QMessageBox::information(nullptr,"提示","未找到该课程！");
            }
        }
        else if(state==1)
        {
            bool isfind=false;
            showCourseTableHeader();
            ui->tableWidget->setRowCount(1);
            for(int i=1;i<=courseNum;i++)
            {
                auto&c=courses[i];
                if(c.name==goal&&c.isCompulsory)
                {
                    showCourseTableItem(c,1);
                    isfind=true;
                    break;
                }
            }
            if(!isfind)
            {
                showCourseTableHeader();
                ui->tableWidget->setRowCount(0);
                QMessageBox::information(nullptr,"提示","未找到该课程！");
            }
        }
        else
        {
            bool isfind=false;
            showCourseTableHeader();
            ui->tableWidget->setRowCount(1);
            for(int i=1;i<=courseNum;i++)
            {
                auto&c=courses[i];
                if(c.name==goal&&!c.isCompulsory)
                {
                    showCourseTableItem(c,1);
                    isfind=true;
                    break;
                }
            }
            if(!isfind)
            {
                showCourseTableHeader();
                ui->tableWidget->setRowCount(0);
                QMessageBox::information(nullptr,"提示","未找到该课程！");
            }
        }
    }
}
void MainWindow::on_search_val_textEdited(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        search_state=false;
        updateSelectCourseTable();
    }
}

void MainWindow::showContextMenu(const QPoint& pos) {
    QTableWidget *tableWidget = ui->tableWidget;
    QTableWidgetItem *item = tableWidget->itemAt(pos);

    if (item && item->column() == 0) { // 如果点击的是第0列
        QMenu contextMenu(this);

        QAction *deleteAction = new QAction("删除", this);
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteCourse()));
        contextMenu.addAction(deleteAction);

        contextMenu.exec(tableWidget->viewport()->mapToGlobal(pos));
    }
}
void MainWindow::deleteCourse() {
    QTableWidget *tableWidget = ui->tableWidget;
    int currentRow = tableWidget->currentRow();

    if (currentRow >= 0) {
        QString courseName = tableWidget->item(currentRow, 2)->text();
        //qDebug()<<courseName;
        // 弹出确认对话框
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认删除", "确认删除当前选中的课程吗?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // 用户确认删除，执行删除操作
            //tableWidget->removeRow(currentRow);
            // 执行删除操作，可以根据需要进行实际的删除操作
            bool suc=deleteCourseReal(courseName);
            if(!suc)
                QMessageBox::warning(this, tr("删除失败"), tr("该课程为其它课程的先修课程！"));
        }
    }
}
bool MainWindow::deleteCourseReal(QString name) //如果当前的课程作为其它课程的先修课程，则不能被删除
{
    int id=h1.find(name);
    if(needcnt[id]) return false;
    valid[id]=false;
    int courseIndex=0;
    for(int i=1;i<=courseNum;i++)
        if(courses[i].id==id)
        {
            courseIndex=i;
            break;
        }
    for(int i=1;i<=courses[courseIndex].preCoursesNum;++i)
    {
        int c=courses[courseIndex].preCourses[i];
        g.delEdge(c,id);
        int pos=0;
        for(int j=0;j<needcnt[c];j++)
            if(needthis[c][j]==id)
            {
                pos=j;
                break;
            }
        for(int j=pos;j<needcnt[c]-1;j++)
            needthis[c][j]=needthis[c][j+1];
        needcnt[c]--;
    }

    // 删除课程
    for (int i = courseIndex; i <= courseNum - 1; ++i)
    {
        courses[i] = courses[i + 1];
    }
    courseNum--;

    updateSelectCourseTable();
    return true;
}
void MainWindow::onCellClicked(int row, int column) {
    QTableWidget *tableWidget = ui->tableWidget;
    if (column == 0) { // 如果点击的是第0列
        for (int r = 0; r < tableWidget->rowCount(); ++r) {
            for (int c = 0; c < tableWidget->columnCount(); ++c) {
                QTableWidgetItem *item = tableWidget->item(r, c);
                if (r == row) {
                    item->setSelected(true);
                } else {
                    item->setSelected(false);
                }
            }
        }
    }
}

void MainWindow::on_show_graph_btn_clicked()
{
    CourseGraphWindow *courseGraphWindow = new CourseGraphWindow;

    // 调用 drawCourseGraph 函数来绘制课程图
    courseGraphWindow->drawCourseGraph();

    courseGraphWindow->show();
}


void MainWindow::on_arrangement_btn_clicked()
{
    maxclasshours= ui->maxhours->value();
    setNum=ExtendTopSort(credits);
    QMessageBox successBox;
    successBox.setIcon(QMessageBox::Information); // 设置图标为信息图标
    successBox.setWindowTitle("编排成功！");
    successBox.setText("课程编排成功！");
    QIcon Icon(":/icons/success.png");
    successBox.setWindowIcon(Icon);
    ui->arrangement_btn->setEnabled(false);
    ui->reset_btn->setEnabled(true);
    successBox.exec(); // 显示对话框
    showCourseArrangeTableHeader();
    int maxv=0;
    for(int i=1;i<=setNum;i++) maxv=std::max(maxv,everytermcnt[i]);
    ui->tableWidget_2->setRowCount(maxv);
    //显示编排结果：
    for(int i=1;i<=setNum;i++)
        for(int j=0;j<everytermcnt[i];j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(h2.find(res[i][j]));
            ui->tableWidget_2->setItem(j, i-1, item);
        }
}


void MainWindow::on_reset_btn_clicked() //清空课表，让用户可以重新排
{
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(0);
    ui->tableWidget_2->clear();
    ui->arrangement_btn->setEnabled(true);
    ui->reset_btn->setEnabled(false);
}


void MainWindow::on_exit_button_clicked()
{
    exit(0);
}

