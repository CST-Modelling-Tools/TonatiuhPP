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
    ParametersItem* item = static_cast<ParametersItem*>(model->itemFromIndex(index));
    SoField* field = item->getField();

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
        connect(
            editor,  SIGNAL(activated(int)),
            this, SLOT(commitAndCloseEditor())
        );
        return editor;
    }
    else if (SoSFBool* f = dynamic_cast<SoSFBool*>(field))
    {
        QComboBox* editor = new QComboBox(parent);
        editor->addItem("false");
        editor->addItem("true");
        editor->setCurrentIndex(f->getValue() ? 1 : 0);
        connect(
            editor,  SIGNAL(activated(int)),
            this, SLOT(commitAndCloseEditor())
        );
        return editor;
    }
    else if (UserSField* f = dynamic_cast<UserSField*>(field))
    {
        FieldEditor* editor = f->GetEditor();
        editor->setGeometry(option.rect);
        editor->setParent(parent);
        QString s = model->data(index, Qt::DisplayRole).toString();
        editor->SetData(s);
        connect(
            editor, SIGNAL(editingFinished()),
            this, SLOT(onCloseEditor())
        );
        return editor;
    }
    else if (UserMField* f = dynamic_cast<UserMField*>(field))
    {
        FieldEditor* editor = f->getEditor();
        editor->setGeometry(option.rect);
        editor->setParent(parent);
        QString s = model->data(index, Qt::DisplayRole).toString();
        editor->SetData(s);
        connect(
            editor, SIGNAL(editingFinished()),
            this, SLOT(onCloseEditor())
        );
        return editor;
    }
    else
    {
        QString text = model->data(index).toString();

        if (text.indexOf('\n') >= 0) {
//            text = text.trimmed();
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

void ParametersDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
// keep
}

#include <QMouseEvent>
#include <QApplication>
void ParametersDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (!dynamic_cast<ParametersEditor*>(editor))
        QStyledItemDelegate::updateEditorGeometry(editor, option, index);

    if (QComboBox* cb = dynamic_cast<QComboBox*>(editor))
    {
        QMouseEvent event(QEvent::MouseButtonPress, QPointF(0, 0), Qt::LeftButton, 0, 0);
        QApplication::sendEvent(cb, &event);
//        cb->showPopup();
    }
}

void ParametersDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    ParametersModel* modelP = static_cast<ParametersModel*>(model);
    ParametersItem* item = static_cast<ParametersItem*>(modelP->itemFromIndex(index));
    SoField* field = item->getField();

    QString value;

    if (dynamic_cast<SoSFEnum*>(field))
    {
        QComboBox* w = qobject_cast<QComboBox*>(editor);
        value = w->currentText();
    }
    else if (dynamic_cast<SoSFBool*>(field))
    {
        QComboBox* w = qobject_cast<QComboBox*>(editor);
        value = w->currentIndex() ? "TRUE" : "FALSE";
    }
    else if (dynamic_cast<UserSField*>(field))
    {
        FieldEditor* w = static_cast<FieldEditor*>(editor);
        value = w->GetData();
    }
    else if (dynamic_cast<UserMField*>(field))
    {
        FieldEditor* w = static_cast<FieldEditor*>(editor);
        value = w->GetData();
    }
    else
    {
        if (QLineEdit* w = dynamic_cast<QLineEdit*>(editor) )
            value = w->text();

        else if (ParametersEditor* w = dynamic_cast<ParametersEditor*>(editor) )
            value = w->text();

        QString text = model->data(index).toString();
        if (value == text) return;
    }

    modelP->setData(index, value);
}

void ParametersDelegate::onCloseEditor() // not used?
{
    QWidget* editor = qobject_cast<QWidget*>(sender());
    emit closeEditor(editor);
}

void ParametersDelegate::commitAndCloseEditor()
{
    QComboBox* editor = qobject_cast<QComboBox*>(sender());
//    emit commitData(editor);
//    emit closeEditor(editor);
    editor->close();
}
