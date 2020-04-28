#pragma once

#include <QDialog>

#include "ui_griddialog.h"


class GridDialog : public QDialog, private Ui::GridDialog
{
    Q_OBJECT

public:
    GridDialog( int xDimension = 100, int zDimension = 100, double xSpacing = 1, double zSpacing = 1, QWidget* parent = 0 );
    ~GridDialog();

    int GetXDimension();
    int GetZDimension();

    double GetXSpacing();
    double GetZSpacing();

    bool IsSizeDefined();
};
