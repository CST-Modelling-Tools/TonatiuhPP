#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QWidget>


class TONATIUH_KERNEL PhotonsWidget: public QWidget
{
    Q_OBJECT

public:
    PhotonsWidget(QWidget* parent = 0): QWidget(parent) {}
    ~PhotonsWidget() {}

    virtual QStringList getParameterNames() const {return QStringList();}
    virtual QString getParameterValue(QString /*name*/) const {return QString();}
};
