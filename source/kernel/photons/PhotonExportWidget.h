#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QWidget>


class TONATIUH_KERNEL PhotonExportWidget: public QWidget
{
    Q_OBJECT

public:
    PhotonExportWidget(QWidget* parent = 0): QWidget(parent) {}
    ~PhotonExportWidget() {}

    virtual QStringList GetParameterNames() const {return QStringList();}
    virtual QString GetParameterValue(QString /*parameter*/) const {return QString();}
};
