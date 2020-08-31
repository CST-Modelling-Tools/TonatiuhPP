#pragma once

#include <QTextEdit>
#include <QPLainTextEdit>


class CodeEditor: public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget* parent = 0);

    int widthLineNumbers() const;
    void paintLineNumbers(QPaintEvent* event);

protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void updateLineNumbersWidth(int newBlockCount);
    void updateLineNumbersRect(const QRect& rect, int dy);
    void highlightCurrentLine();

private:
     QWidget* m_lineNumbers;
};
