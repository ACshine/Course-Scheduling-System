#ifndef SELECTCOURSESDIALOG_H
#define SELECTCOURSESDIALOG_H

#include <QDialog>

namespace Ui {
class SelectCoursesDialog;
}

class SelectCoursesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectCoursesDialog(QWidget *parent = nullptr);
    ~SelectCoursesDialog();

private slots:
    void on_showcourses_btn_clicked();

    void on_add_btn_clicked();

    void on_confirm_btn_clicked();

private:
    Ui::SelectCoursesDialog *ui;
    void updateshow();
    QString lastText = "";
    int lastSet=0;
};

#endif // SELECTCOURSESDIALOG_H
