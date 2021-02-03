#include "LineEditPlaceHolder.h"

#include <QPainter>
#include <QStyle>

LineEditPlaceHolder::LineEditPlaceHolder(QWidget* parent):
    QLineEdit(parent)
{
    QPalette p = palette();
    //        p.setColor(QPalette::Text, Qt::green);
    p.setColor(QPalette::PlaceholderText, Qt::gray);
    setPalette(p);
}
