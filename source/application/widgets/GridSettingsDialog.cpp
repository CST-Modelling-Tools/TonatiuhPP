#include "GridSettingsDialog.h"

GridSettingsDialog::GridSettingsDialog(  int xDimension, int zDimension, double xSpacing, double zSpacing, QWidget* parent )
:QDialog( parent )
{
    setupUi( this );
    xDimensionSpin->setValue( xDimension );
    zDimensionSpin->setValue( zDimension );
    xSpacingSpin->setValue( xSpacing );
    zSpacingSpin->setValue( zSpacing );

}

GridSettingsDialog::~GridSettingsDialog()
{

}

int GridSettingsDialog::GetXDimension()
{
    return xDimensionSpin->value();
}

int GridSettingsDialog::GetZDimension()
{
    return zDimensionSpin->value();
}

double GridSettingsDialog::GetXSpacing()
{
    return xSpacingSpin->value();
}

double GridSettingsDialog::GetZSpacing()
{
    return zSpacingSpin->value();
}


bool GridSettingsDialog::IsSizeDefined()
{
    return userDefinedRadio->isChecked();
}
