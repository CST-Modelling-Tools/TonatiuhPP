#include "LineNumbers.h"

#include "CodeEditor.h"


LineNumbers::LineNumbers(CodeEditor* editor):
    QWidget(editor),
    m_editor(editor)
{
    setFont(editor->font());
}

QSize LineNumbers::sizeHint() const
{
    return QSize(m_editor->widthLineNumbers(), 0);
}

void LineNumbers::paintEvent(QPaintEvent* event)
{
    m_editor->paintLineNumbers(event);
}
