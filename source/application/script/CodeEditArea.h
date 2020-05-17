#pragma once

#include <QPlainTextEdit>


class CodeEditArea: public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditArea(QWidget * parent = 0);

    int BlockTop(const QTextBlock& block) const;
    int BlockHeight(const QTextBlock& block) const;

//public slots:
    //void UpdateCodeEditAreaWidth( int width );

//protected:
    //void resizeEvent(QResizeEvent* event );

private slots:
    void UpdateCurrentLine();

//private:
    //LineNumberWidget* m_LineNumberWidget;
};
