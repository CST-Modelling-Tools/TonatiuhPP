#pragma once


#include <QProxyStyle>

class MenuStyle: public QProxyStyle
{
public:
    int styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* data) const;
};

