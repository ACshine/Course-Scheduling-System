#ifndef COURSEGRAPHWINDOW_H
#define COURSEGRAPHWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>  // 包含 QGraphicsScene 头文件
#include <QGraphicsView>
namespace Ui {
class CourseGraphWindow;
}

class CourseGraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CourseGraphWindow(QWidget *parent = nullptr);
    ~CourseGraphWindow();

    // 添加用于绘制课程图的函数
    void drawCourseGraph();

private:
    Ui::CourseGraphWindow *ui;
    QGraphicsView *view;
    QGraphicsScene *scene;
};

#endif // COURSEGRAPHWINDOW_H
