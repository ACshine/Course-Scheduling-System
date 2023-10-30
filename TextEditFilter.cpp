#include "TextEditFilter.h"

TextEditFilter::TextEditFilter(QTextEdit *textEdit) : textEdit(textEdit) {
    textEdit->installEventFilter(this);
}

bool TextEditFilter::eventFilter(QObject *obj, QEvent *event) {
    if (obj == textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        QTextCursor cursor = textCursor();

        if (cursor.block().position() == 0 && cursor.block().length() == cursor.columnNumber()) {
            // 确保光标在第一行，并且位于行末尾时禁止编辑
            if (event->key() != Qt::Key_Backspace) {
                event->ignore();
                return;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}
