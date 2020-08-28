#pragma once

#include <QWidget>

class CodeEditor;


class LineNumbers: public QWidget
{
    Q_OBJECT

public:
    LineNumbers(CodeEditor* editor);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent* event);

private:
    CodeEditor* m_editor;
};
