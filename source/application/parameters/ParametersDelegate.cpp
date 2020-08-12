#include "ParametersDelegate.h"

#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QTimer>

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>

#include "ParametersModel.h"
#include "ParametersItem.h"
#include "ParametersEditor.h"

#include "libraries/Coin3D/FieldEditor.h"
#include "libraries/Coin3D/UserSField.h"
#include "libraries/Coin3D/UserMField.h"


ParametersDelegate::ParametersDelegate(QObject* parent):
    QStyledItemDelegate(parent)
{

}

QWidget* ParametersDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const ParametersModel* model = static_cast<const ParametersModel*>(index.model());
    SoField* field = model->getData(index)->getField();

    if (SoSFEnum* f = dynamic_cast<SoSFEnum*>(field))
    {
        QComboBox* editor = new QComboBox(parent);
        SbName name;
        for (int n = 0; n < f->getNumEnums(); ++n)
        {
            f->getEnum(n, name);
            editor->addItem(name.getString());
        }
        editor->setCurrentIndex(f->getValue());
        connect(editor,  SIGNAL(activated(int)),
                this, SLOT(onCloseEditor()));


        return editor;
    }
    else if (SoSFBool* f = dynamic_cast<SoSFBool*>(field))
    {
        return 0;
        QCheckBox* editor = new QCheckBox(parent);
        editor->setChecked(f->getValue());
        return editor;
    }
    else if (UserSField* f = dynamic_cast<UserSField*>(field))
    {
        FieldEditor* editor = f->GetEditor();
        editor->setGeometry(option.rect);
        editor->setParent(parent);
        connect(
            editor, SIGNAL(editingFinished()),
            this, SLOT(onCloseEditor())
            );
        QString s = model->data(index, Qt::DisplayRole).toString();
        editor->SetData(s);
        return editor;
    }
    else if (UserMField* f = dynamic_cast<UserMField*>(field))
    {
        FieldEditor* editor = f->getEditor();
        editor->setGeometry(option.rect);
        editor->setParent(parent);
        connect(
            editor, SIGNAL(editingFinished()),
            this, SLOT(onCloseEditor())
            );
        QString s = model->data(index, Qt::DisplayRole).toString();
        editor->SetData(s);
        return editor;
    }
    else
    {
        SbString fieldValue = "null";
        field->get(fieldValue);
        QString text = fieldValue.getString();
//        QString s = model->data(index, Qt::DisplayRole).toString();

        if (text.indexOf('\n') >= 0) {
            text = text.trimmed();
            ParametersEditor* editor = new ParametersEditor;
            editor->setText(text);
            return editor;
        } else {
            QLineEdit* editor = new QLineEdit(parent);
            editor->setText(text);
            editor->setFrame(false);
            return editor;
        }
    }
}

void ParametersDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (!dynamic_cast<ParametersEditor*>(editor))
        QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}

void ParametersDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (QComboBox* cb = dynamic_cast<QComboBox*>(editor))
    {
//        cb->addItem(QIcon(":/icons/select16.png"), "select");
//        cb->set();
//        cb->showPopup();

    }
}

void ParametersDelegate::onCloseEditor() // not used?
{
    QWidget* editor = qobject_cast<QWidget*>(sender());
    emit closeEditor(editor);
}

void ParametersDelegate::commitAndCloseEditor()
{
    QComboBox *editor = qobject_cast<QComboBox*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
