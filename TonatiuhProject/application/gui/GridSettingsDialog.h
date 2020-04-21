#pragma once

#include <QDialog>

#include "ui_gridsettingsdialog.h"


class GridSettingsDialog : public QDialog, private Ui::GridSettingsDialog
{
    Q_OBJECT

public:
    GridSettingsDialog( int xDimension = 100, int zDimension = 100, double xSpacing = 1, double zSpacing = 1, QWidget* parent = 0 );
    ~GridSettingsDialog();

    int GetXDimension();
    int GetZDimension();

    double GetXSpacing();
    double GetZSpacing();

    bool IsSizeDefined();
};
