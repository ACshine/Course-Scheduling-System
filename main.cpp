#include "mainwindow.h"
#include"Graphl.h"
#include <QApplication>
#include"HashTable.h"
#include"HashTable2.h"
#include"Queue.h"
#include"QString"
const int N=310;//最大课程容量
Graphl g(N);
HashTable h1(N);
HashTable2 h2(N);
queue<int>q(N);
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Graphl g1(N);
    HashTable h1t(N);
    HashTable2 h2t(N);
    h1t.insert("高等数学",5);
    h2t.insert(5,"高等数学");

    h1t.insert("数字逻辑",2);
    h2t.insert(2,"数字逻辑");

    h1t.insert("计算机组成原理",3);
    h2t.insert(3,"计算机组成原理");

    h1t.insert("汇编语言",4);
    h2t.insert(4,"汇编语言");

    h1t.insert("高级语言程序设计",10);
    h2t.insert(10,"高级语言程序设计");

    g=g1;

    h1=h1t;
    h2=h2t;
    MainWindow w;
    w.show();
    return a.exec();
}
