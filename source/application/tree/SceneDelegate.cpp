#include "SceneDelegate.h"

#include <QLineEdit>
#include <QModelIndex>

#include <Inventor/SbName.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include "kernel/run/InstanceNode.h"
#include "SceneTreeModel.h"
#include "kernel/sun/SunKit.h"
#include "kernel/air/AirKit.h"
#include "kernel/scene/TerrainKit.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"


SceneDelegate::SceneDelegate(QObject* parent):
    QStyledItemDelegate(parent)
{

}

QWidget* SceneDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const
{
    SceneTreeModel* model = (SceneTreeModel*) (index.model());
    SoNode* node = model->getInstance(index)->getNode();

    SoType type = node->getTypeId();
    if (type == TSeparatorKit::getClassTypeId() ||
               type == TShapeKit::getClassTypeId()) {
        QLineEdit* editor = new QLineEdit(parent);
        QRegExp rx("[a-zA-Z]\\S*"); // \\S matches a non-whitespace character
        QValidator* validator = new QRegExpValidator(rx);
        editor->setValidator(validator);
        return editor;
    }
    return 0;
}

void SceneDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (!editor) return;

    const SceneTreeModel* model = static_cast<const SceneTreeModel*>(index.model());
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
