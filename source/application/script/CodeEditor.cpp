#include "CodeEditor.h"

#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>

#include "LineNumbers.h"

// https://stackoverflow.com/questions/2443358/how-to-add-lines-numbers-to-qtextedit

CodeEditor::CodeEditor(QWidget* parent):
    QPlainTextEdit(parent)
{
    QFont font("Consolas", 9);
//    if (font.exactMatch())
    setFont(font);
    setTabStopDistance(4*fontMetrics().horizontalAdvance('0'));

//    QTextOption qto = document()->defaultTextOption();
//    qto.setFlags(QTextOption::ShowTabsAndSpaces);
//    document()->setDefaultTextOption(qto);

//    QTextCursor cursor = textCursor();
//    cursor.select(QTextCursor::Document);
//    QTextBlockFormat fmt = cursor.blockFormat();
//    fmt.setBackground(QColor(Qt::yellow));
//    fmt.setTopMargin(-1);
//    fmt.setBottomMargin(-1);
////    fmt.setLineHeight(20, QTextBlockFormat::LineDistanceHeight);
//    cursor.setBlockFormat(fmt);
//    setTextCursor(cursor);

    // numbers
    m_lineNumbers = new LineNumbers(this);
    updateLineNumbersWidth(0);
    connect(
        this, SIGNAL(blockCountChanged(int)),
        this, SLOT(updateLineNumbersWidth(int))
    );
    connect(
        this, SIGNAL(updateRequest(QRect,int)),
        this, SLOT(updateLineNumbersRect(QRect,int))
    );

    // highlights
    highlightCurrentLine();
    connect(
        this, SIGNAL(cursorPositionChanged()),
        this, SLOT(highlightCurrentLine())
    );
}

int CodeEditor::widthLineNumbers() const
{
    int digits = 1;
    int temp = document()->blockCount();
    while (temp >= 10) {
        temp /= 10;
        ++digits;
    }
    digits += 2;

    int space = 3 + digits*fontMetrics().horizontalAdvance('0');
    return space;
}

void CodeEditor::paintLineNumbers(QPaintEvent* event)
{
    QPainter painter(m_lineNumbers);
    painter.fillRect(event->rect(), QColor("#F0F0F0"));
    painter.setPen(Qt::black);

    QTextBlock block = firstVisibleBlock();
    int line = block.blockNumber() + 1;
    // y-axis goes down
    int yMin = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int yMax = yMin + qRound(blockBoundingRect(block).height());

    while (block.isValid() && yMin <= event->rect().bottom())
    {
        if (block.isVisible() && yMax >= event->rect().top()) {
            QString text = QString::number(line) + " ";
            painter.drawText(
                0, yMin, m_lineNumbers->width(), fontMetrics().height(),
                Qt::AlignRight, text
            );
        }

        block = block.next();
        ++line;
        yMin = yMax;
        yMax = yMin + qRound(blockBoundingRect(block).height());
    }
}

void CodeEditor::resizeEvent(QResizeEvent* event)
{
    QPlainTextEdit::resizeEvent(event);
    QRect cr = contentsRect();
    m_lineNumbers->setGeometry(QRect(cr.left(), cr.top(), widthLineNumbers(), cr.height()));
}

void CodeEditor::updateLineNumbersWidth(int /*newBlockCount*/)
{
    setViewportMargins(widthLineNumbers(), 0, 0, 0);
}

void CodeEditor::updateLineNumbersRect(const QRect& rect, int dy)
{
    if (dy)
        m_lineNumbers->scroll(0, dy);
    else
        m_lineNumbers->update(0, rect.y(), m_lineNumbers->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumbersWidth(0);
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> selections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(QColor("#C8DBE5"));
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
//        selection.format.setFontStrikeOut(true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
//        selection.cursor.select(QTextCursor::LineUnderCursor);
        selections << selection;
    }

    setExtraSelections(selections);

//    setLineSpacing(-2);
}

//void CodeEditor::setLineSpacing(int lineSpacing)
//{
//    for (QTextBlock block = this->document()->begin(); block.isValid();
//         block = block.next()) {
//        QTextCursor tc = QTextCursor(block);
//        QTextBlockFormat fmt = block.blockFormat();
//        if (fmt.topMargin() != lineSpacing
//                || fmt.bottomMargin() != lineSpacing) {
//            fmt.setTopMargin(lineSpacing);
//            fmt.setBottomMargin(lineSpacing);
//            fmt.setBackground(Qt::yellow);
//            tc.setBlockFormat(fmt);
//        }
//    }
//}
