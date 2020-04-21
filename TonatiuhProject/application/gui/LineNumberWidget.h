#pragma once

#include <QWidget>

class CodeEditArea;

class LineNumberWidget: public QWidget
{
    Q_OBJECT

public:
    LineNumberWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~LineNumberWidget();

    void SetCodeEditor(CodeEditArea* editor);
    QSize sizeHint() const;

    int LineNumberAreaWidth() const;

public slots:
    void UpdateLineNumberArea(const QRect& rect, int dy);

protected:
    void paintEvent(QPaintEvent* event);

private:
    CodeEditArea* m_codeEditArea;

};
