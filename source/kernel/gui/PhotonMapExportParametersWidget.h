#pragma once

#include <QWidget>

class PhotonMapExportParametersWidget: public QWidget
{
    Q_OBJECT

public:
    PhotonMapExportParametersWidget(QWidget* parent = 0);
    ~PhotonMapExportParametersWidget();

    virtual QStringList GetParameterNames() const = 0;
    virtual QString GetParameterValue(QString parameter) const = 0;

};
