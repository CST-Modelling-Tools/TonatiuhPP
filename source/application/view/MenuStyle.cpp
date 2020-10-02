#include "MenuStyle.h"

int MenuStyle::styleHint(QStyle::StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* data) const
{
    if (hint == QStyle::SH_MenuBar_AltKeyNavigation)
        return 0;
    return QProxyStyle::styleHint(hint, option, widget, data);
}
