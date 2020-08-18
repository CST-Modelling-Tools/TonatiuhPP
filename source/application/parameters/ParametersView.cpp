#include "ParametersView.h"

#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QTreeView>
#include <QHeaderView>
#include <QMouseEvent>

#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>

#include <Inventor/nodes/SoNode.h>

#include "libraries/Coin3D/FieldEditor.h"
#include "libraries/Coin3D/UserSField.h"
#include "libraries/Coin3D/UserMField.h"

#include "ParametersDelegate.h"
#include "ParametersItem.h"
#include "ParametersModel.h"
#include "ParametersEditor.h"


ParametersView::ParametersView(QWidget* parent):
    QTreeView(parent)
{
    ParametersDelegate* delegate = new ParametersDelegate(this);
    setItemDelegate(delegate);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setStyleSheet(R"(
QLineEdit {
padding-left: 1;
selection-background-color: #c8dbe5;
selection-color: black;
}
     )");
}

ParametersModel* ParametersView::getModel()
{
    return (ParametersModel*) model();
}

void ParametersView::reset()
{
    QTreeView::reset();
    double w = 2.5*fontMetrics().horizontalAdvance("Parameter");
    setColumnWidth(0, w);
}

void ParametersView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QModelIndex index = indexAt(event->pos());
        if (currentIndex() == index) {
            if (model()->flags(index) & Qt::ItemIsEditable) {
                edit(index);
                return;
            }
        }
    }
    QTreeView::mousePressEvent(event);
}
