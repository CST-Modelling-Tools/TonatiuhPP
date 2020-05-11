#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QWidget>


class TONATIUH_KERNEL PhotonsWidget: public QWidget
{
    Q_OBJECT

public:
    PhotonsWidget(QWidget* parent = 0): QWidget(parent) {}
    ~PhotonsWidget() {}

    virtual QStringList GetParameterNames() const {return QStringList();}
    virtual QString GetParameterValue(QString /*parameter*/) const {return QString();}
};
