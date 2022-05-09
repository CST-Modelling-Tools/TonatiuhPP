#include "StyledItemDelegate.h"

StyledItemDelegate::StyledItemDelegate(QObject* parent):
    QStyledItemDelegate{parent}
{

}

void StyledItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    QStyledItemDelegate::initStyleOption(option, index);

    QVariant value = index.data(Qt::ForegroundRole);
    if (value.canConvert<QBrush>()) {
        option->palette.setBrush(QPalette::HighlightedText, qvariant_cast<QBrush>(value));
    } else
        option->palette.setColor(QPalette::Normal, QPalette::HighlightedText, Qt::black);
}

// https://stackoverflow.com/questions/43035378/qtreeview-item-hover-selected-background-color-based-on-current-color
// selection background font color

//style hover override
// https://code.woboq.org/qt5/qtbase/src/widgets/styles/qcommonstyle.cpp.html
//style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
// https://code.woboq.org/qt5/qtbase/src/widgets/itemviews/qstyleditemdelegate.cpp.html

// https://code.woboq.org/qt5/qtbase/src/widgets/styles/qstylesheetstyle.cpp.html

//    QPalette pal = QApplication::palette();
//    pal.setColor( QPalette::Normal, QPalette::HighlightedText, QColor("#FF0000"));
//    QApplication::setPalette(pal);
