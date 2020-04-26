#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QWidget>


class TONATIUH_KERNEL PhotonMapExportParametersWidget: public QWidget
{
    Q_OBJECT

public:
    PhotonMapExportParametersWidget(QWidget* parent = 0);
    ~PhotonMapExportParametersWidget();

    virtual QStringList GetParameterNames() const {return QStringList();}
    virtual QString GetParameterValue(QString /*parameter*/) const {return QString();}
};
