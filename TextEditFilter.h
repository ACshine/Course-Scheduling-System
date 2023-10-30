#ifndef TEXTEDITFILTER_H
#define TEXTEDITFILTER_H
#include <QObject>
#include <QTextEdit>
#include <QKeyEvent>

class TextEditFilter : public QObject
{
    Q_OBJECT

public:
    TextEditFilter(QTextEdit *textEdit);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QTextEdit *textEdit;
};
#endif // TEXTEDITFILTER_H
