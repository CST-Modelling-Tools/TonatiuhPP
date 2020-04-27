#pragma once

#include <QWidget>

#include "kernel/photons/PhotonExportWidget.h"

#include "ui_photonexportfilewidget.h"


class PhotonExportFileWidget: public PhotonExportWidget, private Ui::PhotonExportFileWidget
{
    Q_OBJECT

public:
    PhotonExportFileWidget(QWidget* parent = 0);
    ~PhotonExportFileWidget() {}

    QStringList GetParameterNames() const;
    QString GetParameterValue(QString parameter) const;

private slots:
    void SelectSaveDirectory();

private:
    void SetupTriggers();
};
