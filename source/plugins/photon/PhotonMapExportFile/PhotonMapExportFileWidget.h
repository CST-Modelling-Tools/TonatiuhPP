#pragma once

#include <QWidget>

#include "kernel/photons/PhotonMapExportParametersWidget.h"

#include "ui_photonmapexportfilewidget.h"

class PhotonMapExportFileWidget: public PhotonMapExportParametersWidget, private Ui::PhotonMapExportFileWidget
{
    Q_OBJECT

public:
    PhotonMapExportFileWidget(QWidget* parent = 0);
    ~PhotonMapExportFileWidget();

    QStringList GetParameterNames() const;
    QString GetParameterValue(QString parameter) const;

private slots:
    void SelectSaveDirectory();

private:
    void SetupTriggers();
};
