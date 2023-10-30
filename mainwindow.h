#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTreeWidgetItem>
#include "newwindow.h" // 包含新窗口的头文件
#include<addcoursedialog.h>
#include"Course.h"
#include <QTableWidget>
#include "coursegraphwindow.h"
#include"selectcoursesdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    newwindow *Newwindow;   // 新窗口对象
    virtual void keyPressEvent(QKeyEvent*event);

private slots:
    void on_open_file_clicked();

    void on_add_course_btn_clicked();
    void on_select_compulsory_stateChanged(int arg1);
    void on_select_elective_stateChanged(int arg1);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_search_btn_clicked();
    void onCellClicked(int row, int column);
    void on_search_val_textEdited(const QString &arg1);
    void showContextMenu(const QPoint& pos);
    void deleteCourse();
    void on_show_graph_btn_clicked();

    void on_arrangement_btn_clicked();

    void on_reset_btn_clicked();

    void on_exit_button_clicked();

private:
    void updateCourseTable();
    void updateSelectCourseTable();
    void showCourseTableHeader();
    void showCourseArrangeTableHeader();
    void showCourseTableItem(Course c,int row);
    bool deleteCourseReal(QString name);
    bool eventFilter(QObject *obj, QEvent *event);
    int ExtendTopSort(double credit[]);
private:
    Ui::MainWindow *ui;
    AddCourseDialog addcourse_dlg;
    CourseGraphWindow *courseGraphWindow;
    //SelectCoursesDialog selectcoursesdialog;
};


#endif // MAINWINDOW_H
