#ifndef ADDCOURSEDIALOG_H
#define ADDCOURSEDIALOG_H

#include <QDialog>

namespace Ui {
class AddCourseDialog;
}

class AddCourseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCourseDialog(QWidget *parent = nullptr);
    ~AddCourseDialog();

private slots:
    void on_save_btn_clicked();

    void on_cancel_btn_clicked();

private:
    Ui::AddCourseDialog *ui;
};

#endif // ADDCOURSEDIALOG_H
