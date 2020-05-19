#include "SceneDelegate.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QModelIndex>
#include <QObject>

#include <Inventor/SbName.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include "kernel/run/InstanceNode.h"
#include "tree/SceneModel.h"

/**
 * Creates a new delegate to facilitate editing node names.
 */
SceneDelegate::SceneDelegate(QObject* parent):
    QStyledItemDelegate(parent)
{

}

/**
 * Returns a line edit editor with a validates the input names. The names must start with a alphabetic character and optionally followed with some
 * alphanumeric characters.
 */
QWidget* SceneDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    QLineEdit* editor = new QLineEdit(parent);
    QRegExp rx("[a-zA-Z]\\S*"); // exclude white spaces
    QValidator* validator = new QRegExpValidator(rx);
    editor->setValidator(validator);
    return editor;
}

/**
 * Takes the name of the selected node and sets to de editor to display it.
 */
void SceneDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
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
