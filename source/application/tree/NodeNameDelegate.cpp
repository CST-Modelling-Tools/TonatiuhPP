#include <QLineEdit>
#include <QMessageBox>
#include <QModelIndex>
#include <QObject>

#include <Inventor/SbName.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include "kernel/gui/InstanceNode.h"
#include "NodeNameDelegate.h"
#include "gui/SceneModel.h"

/**
 * Creates a new delegate to facilitate editing node names.
 */
NodeNameDelegate::NodeNameDelegate(QObject* parent)
    : QItemDelegate(parent)
{

}

/**
 * Destroys the NodeNameDelegate.
 */
NodeNameDelegate::~NodeNameDelegate()
{

}

/**
 * Returns a line edit editor with a validates the input names. The names must start with a alphabetic character and optionally followed with some
 * alphanumeric characters.
 */
QWidget* NodeNameDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    QLineEdit* editor = new QLineEdit (parent);
    QRegExp rx("[a-zA-Z]\\S*");
    QValidator* validator = new QRegExpValidator(rx, 0);
    editor->setValidator(validator);

    return editor;
}

/**
 * Takes the name of the selected node and sets to de editor to display it.
 */
void NodeNameDelegate::setEditorData(QWidget* editor,
                                     const QModelIndex& index) const
{
    const SceneModel* model = static_cast< const SceneModel* >(index.model() );

    QString value = model->data(index, Qt::DisplayRole).toString();

    QLineEdit* textEdit = static_cast<QLineEdit*>(editor);

    SoNode* coinNode = model->NodeFromIndex(index)->GetNode();

    QString nodeName;
    if (coinNode->getName() == SbName() )
        nodeName = QString(coinNode->getTypeId().getName().getString() );
    else
        nodeName = QString(coinNode->getName().getString() );

    textEdit->setText(nodeName);
}
