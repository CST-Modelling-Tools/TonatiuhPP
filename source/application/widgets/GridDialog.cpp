#include "GridDialog.h"

GridDialog::GridDialog(  int xDimension, int zDimension, double xSpacing, double zSpacing, QWidget* parent )
:QDialog( parent )
{
    setupUi( this );
    xDimensionSpin->setValue( xDimension );
    zDimensionSpin->setValue( zDimension );
    xSpacingSpin->setValue( xSpacing );
    zSpacingSpin->setValue( zSpacing );

}

GridDialog::~GridDialog()
{

}

int GridDialog::GetXDimension()
{
    return xDimensionSpin->value();
}

int GridDialog::GetZDimension()
{
    return zDimensionSpin->value();
}

double GridDialog::GetXSpacing()
{
    return xSpacingSpin->value();
}

double GridDialog::GetZSpacing()
{
    return zSpacingSpin->value();
}


bool GridDialog::IsSizeDefined()
{
    return userDefinedRadio->isChecked();
}
