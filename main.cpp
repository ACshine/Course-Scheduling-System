#include "mainwindow.h"
#include"Graphl.h"
#include <QApplication>
#include"HashTable.h"
#include"HashTable2.h"
#include"Queue.h"
#include"QString"
#include"Course.h"
#define MAX_COURSES 300
Graphl g(MAX_COURSES);
HashTable h1(MAX_COURSES);
HashTable2 h2(MAX_COURSES);
queue<int>q(MAX_COURSES);
int courseNum;//当前课程总数
bool valid[MAX_COURSES];
Course courses[MAX_COURSES];
double credits[MAX_COURSES];
int courseID=1;
int Depth[MAX_COURSES];

int needthis[MAX_COURSES][MAX_COURSES];//维护每门课程是哪些课程的先修课程
int needcnt[MAX_COURSES];
bool Compulsory[MAX_COURSES];
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Graphl g1(MAX_COURSES);
    HashTable h1t(MAX_COURSES);
    HashTable2 h2t(MAX_COURSES);
    QString name[MAX_COURSES] = {
                                    "高级语言程序设计",
                                    "集合与图论",
                                    "数据结构与算法",
                                    "数据结构课设Ⅰ",
                                    "高级语言程序设计课设",
                                    "计算机软件类综合性课程设计",
                                    "电路分析基础-1",
                                    "模拟电子技术",
                                    "数字逻辑Ⅰ",
                                    "计算机组成原理",
                                    "计算机系统结构Ⅱ",
                                    "计算机组成原理课设",
                                    "汇编语言程序设计",
                                    "面向对象程序设计",
                                    "C++语言程序设计",
                                    "高等数学(工)-1",
                                    "高等数学(工)-2",
                                    "线性代数(工)",
                                    "大学英语（综合）",
                                    "大学英语（高级）",
                                    "概率论与数理统计（工）",
                                    "大学物理Ⅰ-1",
                                    "大学物理Ⅰ-2",
                                    "代数与逻辑",
                                    "机器学习基础",
                                    "计算机网络",
                                    "操作系统原理",
                                    "编译原理Ⅰ",
                                    "算法设计与分析"
};

    int n=29;
    for(int i=0;i<n;i++)
    {
        h1t.insert(name[i],courseID);
        h2t.insert(courseID,name[i]);
        courseID++;
    }
    g=g1;
    h1=h1t;
    h2=h2t;
    MainWindow w;
    w.show();
    return a.exec();
}
