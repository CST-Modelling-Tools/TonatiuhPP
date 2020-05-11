#pragma once

#include <QWidget>

#include "kernel/photons/PhotonsWidget.h"

#include "ui_PhotonsFilewidget.h"


class PhotonsFileWidget: public PhotonsWidget, private Ui::PhotonsFileWidget
{
    Q_OBJECT

public:
    PhotonsFileWidget(QWidget* parent = 0);
    ~PhotonsFileWidget() {}

    QStringList GetParameterNames() const;
    QString GetParameterValue(QString parameter) const;

private slots:
    void SelectSaveDirectory();

private:
    void SetupTriggers();
};
