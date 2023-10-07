#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>
#include"Graphl.h"
#include <QFileDialog>
#include<QMessageBox>
#include<fstream>
#include<string>
#include <sstream>
#include<iostream>
#include <QFile>
#include <QTextStream>
#include"Graphl.h"
#include "newwindow.h"
#include"Course.h"
#include"HashTable.h"
#include"queue.h"
#define INF 0x3f3f3f3f
#define MAX_COURSES 300
const int N = 110, maxclasshours = 280;
int res[N][N];
int D[N];
int ans[N], cnt;//存拓扑序列
bool st[N];
double credits[N];
extern Graphl g;
extern HashTable hash;
Course courses[MAX_COURSES];
int courseNum;//当前课程总数
bool readCourseFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.size()>=5) {
            int id = parts[0].toInt();
            QString name = parts[1];
            double credit = parts[2].toDouble();
            bool isCompulsory=parts[3].toInt();
            int preCoursesNum = parts.size() - 4;
            credits[id]=credit;
            int* preCourses = new int[preCoursesNum];
            int preCoursesCount = 0; // 用于跟踪有效的先修课程数量
            for (int i = 4; i < parts.size(); ++i) {
                QString s = parts[i];
                int preCourse=hash.find(s);
                if (preCourse != -1) {
                    g.setEdge(preCourse, id, 0);
                    preCourses[++preCoursesCount] = preCourse; //先修课程数组下标从1起
                }
            }
            // 创建Course对象并存储
            Course c(id, name, credit, preCoursesNum, preCourses, isCompulsory);
            courses[++courseNum] = c;//下标从1起
            delete[] preCourses; // 释放动态分配的内存
        } else {
            file.close();
            return false;
        }
    }
    //qDebug() <<g.numEdge;
    file.close();
    return true;
}
bool addCourse(QString name, double credits, int preCoursesNum, int* preCourses, bool isCompulsory)
{
    // 检查是否超过课程数组的最大容量
    if (courseNum > MAX_COURSES) {
        qDebug() << "课程数量已达上限，新建失败!";
        return false;
    }
    int id=courseNum+1;
    // 创建 Course 对象并存储课程信息
    courses[++courseNum] = Course(id, name, credits, preCoursesNum, preCourses, isCompulsory);

    //更新图
    for(int i=1;i<=preCoursesNum;i++)
    {
        g.setEdge(preCourses[i],id,0);
    }
    return true;
}
bool deleteCourse(int id)
{
    int courseIndex = -1; // 用于存储要删除课程的索引
    int l=1,r=courseNum;
    while(l<r)
    {
        int mid=l+r>>1;
        if(courses[mid].id>=id)
            r=mid;
        else
            l=mid+1;
    }
    if(courses[r].id==id)
        courseIndex=r;
    else //未找到
    {
        qDebug() << "Error: Course with ID" << id << "not found!";
        return false;
    }
    for(int i=1;i<=courses[courseIndex].preCoursesNum;++i)
    {
        g.delEdge(courses[courseIndex].preCourses[i],id);
    }
    // 删除课程
    for (int i = courseIndex; i <= courseNum - 1; ++i)
    {
        courses[i] = courses[i + 1];
    }
    courseNum--;

    return true;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_toolButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择txt文件"), QDir::homePath(), tr("文本文件 (*.txt)"));
    if (!filePath.isEmpty()) {
        // 调用函数来读取文件内容
        bool readSuccess = readCourseFile(filePath);

        if (readSuccess) {
            QMessageBox::information(this, tr("成功"), tr("文件读取成功！"));
        } else {
            QMessageBox::warning(this, tr("错误"), tr("文件读取失败，请检查文件格式"));
        }
    }
}






void MainWindow::on_pushButton_clicked()
{

}

