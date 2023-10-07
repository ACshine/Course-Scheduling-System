#include "mainwindow.h"
#include"Graphl.h"
#include <QApplication>
#include"HashTable.h"
#include"Queue.h"
#include"test.h"
const int N=110;//最大课程容量
Graphl g(N);
HashTable hash(10*N);
queue<int>q(N);
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Graphl g1(N);
    test t;

    HashTable hash1(N);
    g=g1;
    hash1.insert("高等数学",666);
    hash=hash1;
    MainWindow w;
    w.show();
    return a.exec();
}
