#include "CodeEditor.h"

#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>

#include "LineNumbers.h"


CodeEditor::CodeEditor(QWidget* parent):
    QPlainTextEdit(parent)
{
    QFont font("Consolas", 9);
//    if (font.exactMatch())
    setFont(font);
    setTabStopDistance(4*fontMetrics().horizontalAdvance('0'));

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
    int temp = blockCount();
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
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        selections << selection;
    }

    setExtraSelections(selections);
}
