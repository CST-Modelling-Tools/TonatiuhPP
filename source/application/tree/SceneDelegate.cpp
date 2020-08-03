#include "SceneDelegate.h"

#include <QLineEdit>
#include <QModelIndex>

#include <Inventor/SbName.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include "kernel/run/InstanceNode.h"
#include "SceneModel.h"
#include "kernel/sun/SunKit.h"
#include "kernel/air/AirKit.h"
#include "kernel/scene/TerrainKit.h"

SceneDelegate::SceneDelegate(QObject* parent):
    QStyledItemDelegate(parent)
{

}

/**
 * Returns a line edit editor with a validates the input names. The names must start with a alphabetic character and optionally followed with some
 * alphanumeric characters.
 */
QWidget* SceneDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const
{
    SceneModel* model = (SceneModel*) (index.model());
    SoNode* node = model->getInstance(index)->getNode();

    if (node->getTypeId().isDerivedFrom(SunKit::getClassTypeId())) {
        model->emitModifySun();
        return 0;
    } else if (node->getTypeId().isDerivedFrom(AirKit::getClassTypeId())) {
        model->emitModifyAir();
        return 0;
    }  else if (node->getTypeId().isDerivedFrom(TerrainKit::getClassTypeId())) {
        return 0;
    }

    QLineEdit* editor = new QLineEdit(parent);
    QRegExp rx("[a-zA-Z]\\S*"); // \\S matches a non-whitespace character
    QValidator* validator = new QRegExpValidator(rx);
    editor->setValidator(validator);
    return editor;

    // check Sun and air, run their dialog via main?
}

/**
 * Takes the name of the selected node and sets to de editor to display it.
 */
void SceneDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (!editor) return;

    const SceneModel* model = static_cast<const SceneModel*>(index.model());
    SoNode* node = model->getInstance(index)->getNode();

    QString name;
    if (node->getName() == SbName())
        name = node->getTypeId().getName().getString();
    else
        name = node->getName().getString();

//    if without [ref] number
//    QString name = model->data(index, Qt::DisplayRole).toString();

    QLineEdit* textEdit = static_cast<QLineEdit*>(editor);
    textEdit->setText(name);
}
