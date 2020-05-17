#include "CodeEditArea.h"

#include <QPainter>
#include <QTextBlock>


CodeEditArea::CodeEditArea(QWidget* parent):
    QPlainTextEdit(parent)
{
    connect(this, SIGNAL( cursorPositionChanged() ), this, SLOT( UpdateCurrentLine() ) );
    /*connect(this, SIGNAL( blockCountChanged( int ) ), this, SLOT( UpdateCodeEditAreaWidth( int ) ) );
    connect(this, SIGNAL( updateRequest( QRect, int ) ), m_LineNumberWidget, SLOT( UpdateLineNumberArea( QRect, int ) ) );

*/
    //UpdateCodeEditAreaWidth(0);
    UpdateCurrentLine();
}

int CodeEditArea::BlockTop(const QTextBlock& block) const
{
    return (int) blockBoundingGeometry(block).translated(contentOffset()).top();
}

int CodeEditArea::BlockHeight(const QTextBlock& block) const
{
    return (int) blockBoundingRect(block).height();
}

 void CodeEditArea::UpdateCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if( !isReadOnly() )
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}
