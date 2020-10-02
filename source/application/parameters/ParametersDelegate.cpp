#include "ParametersDelegate.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include <QMouseEvent>
#include <QApplication>

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include "libraries/Coin3D/FieldEditor.h"
#include "libraries/Coin3D/UserSField.h"
#include "libraries/Coin3D/UserMField.h"
#include "main/MainWindow.h"
#include "main/PluginManager.h"
#include "kernel/node/TFactory.h"
#include "kernel/node/TNode.h"

#include "ParametersModel.h"
#include "ParametersItemField.h"
#include "ParametersItemNode.h"
#include "ParametersEditor.h"
#include "ComboBoxDelegate.h"

ParametersDelegate::ParametersDelegate(QObject* parent):
    QStyledItemDelegate(parent)
{

}

QWidget* ParametersDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const ParametersModel* model = static_cast<const ParametersModel*>(index.model());
    ParametersItemField* item = static_cast<ParametersItemField*>(model->itemFromIndex(index));
    SoField* field = item->field();

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
            editor, SLOT(close())
        );
        editor->setItemDelegate(new ComboBoxDelegate);
        return editor;
    }
    else if (SoSFBool* f = dynamic_cast<SoSFBool*>(field))
    {
        QComboBox* editor = new QComboBox(parent);
        editor->addItems({"false", "true"});
        editor->setCurrentIndex(f->getValue() ? 1 : 0);
        connect(
            editor, SIGNAL(activated(int)),
            this, SLOT(onCloseEditor())
        );
        editor->setItemDelegate(new ComboBoxDelegate);
        return editor;
    }
    else if (SoSFNode* f = dynamic_cast<SoSFNode*>(field))
    {
        SoNode* node = f->getValue();
        TNode* tnode = dynamic_cast<TNode*>(node);
        if (!tnode) return 0;

        QComboBox* editor = new QComboBox(parent);
        MainWindow* main = model->getMain();
        QVector<TFactory*> factories = main->getPlugins()->getFactories(node);
        for (TFactory* tf : factories) {
            if (!tf)
                editor->insertSeparator(editor->count());
            else
                editor->addItem(tf->icon(), tf->name());
        }
        editor->setCurrentText(tnode->getTypeName());

        connect(
            editor, SIGNAL(activated(int)),
            this, SLOT(onCloseEditor())
        );
        editor->setItemDelegate(new ComboBoxDelegate);
        editor->setMaxVisibleItems(16);
//        editor->setFont(((QWidget*)this->parent())->font());
//        editor->setFont(QFont("Segoe UI", 9));
//        editor->setFont(QFont("MS Shell Dlg 2", 8));
        return editor;
    }
    else if (SoSFInt32* f = dynamic_cast<SoSFInt32*>(field))
    {
        QSpinBox* editor = new QSpinBox(parent);
        editor->setMinimum(std::numeric_limits<int>::min());
        editor->setMaximum(std::numeric_limits<int>::max());
        editor->setValue(f->getValue());
        return editor;
    }
//    else if (SoSFDouble* f = dynamic_cast<SoSFDouble*>(field))
//    {
//        QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
//        editor->setValue(f->getValue());
//        return editor;
//    }
    else if (UserSField* f = dynamic_cast<UserSField*>(field))
    {
        FieldEditor* editor = f->getEditor();
        editor->setParent(parent);
        editor->setGeometry(option.rect);
        QString s = model->data(index).toString();
        editor->setData(s);
        connect(
            editor, SIGNAL(editingFinished()),
            this, SLOT(onCloseEditor())
        );
        return editor;
    }
    else if (UserMField* f = dynamic_cast<UserMField*>(field))
    {
        FieldEditor* editor = f->getEditor();
        editor->setParent(parent);
        editor->setGeometry(option.rect);
        QString s = model->data(index).toString();
        editor->setData(s);
        connect(
            editor, SIGNAL(editingFinished()),
            this, SLOT(onCloseEditor())
        );
        return editor;
    }
    else
    {
        QString text = model->data(index, Qt::DisplayRole).toString();

        if (text == "...") {
            text = model->data(index, Qt::EditRole).toString();
            ParametersEditor* editor = new ParametersEditor;
            editor->setText(text);
            return editor;
        } else {
            QLineEdit* editor = new QLineEdit(parent);
            editor->setText(text);
            return editor;
        }
    }
}

void ParametersDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(index)
    // keep
}

void ParametersDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (!dynamic_cast<ParametersEditor*>(editor))
        QStyledItemDelegate::updateEditorGeometry(editor, option, index);

    if (QComboBox* cb = dynamic_cast<QComboBox*>(editor))
    {
        cb->setGeometry(option.rect); // fit icons
//        cb->showPopup();
        QMouseEvent event(QEvent::MouseButtonPress, QPointF(0, 0), Qt::LeftButton, 0, 0);
        QApplication::sendEvent(cb, &event);
    }
}

#include <QListView>
void ParametersDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    ParametersModel* modelP = static_cast<ParametersModel*>(model);
    ParametersItemField* item = static_cast<ParametersItemField*>(modelP->itemFromIndex(index));
    SoField* field = item->field();

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
    else if (SoSFNode* f = dynamic_cast<SoSFNode*>(field))
    {
        QComboBox* w = qobject_cast<QComboBox*>(editor);
        value = w->currentText();
        QString text = model->data(index, Qt::DisplayRole).toString();
        if (value == text) return;

        ParametersItemNode* pitem = (ParametersItemNode*) item->parent();
        SoBaseKit* kit = (SoBaseKit*) pitem->node();

        SbName sname;
        kit->getFieldName(field, sname);
        QString name = sname.getString();

        SoNode* node = f->getValue();
        MainWindow* main = modelP->getMain();
        TFactory* tf = main->getPlugins()->getFactories(node)[w->currentIndex()];
        modelP->setData(kit, name, tf->create());

        return;
    }
    else if (dynamic_cast<SoSFInt32*>(field))
    {
        QSpinBox* w = qobject_cast<QSpinBox*>(editor);
        value = QString::number(w->value());
    }
//    else if (dynamic_cast<SoSFDouble*>(field))
//    {
//        QDoubleSpinBox* w = qobject_cast<QDoubleSpinBox*>(editor);
//        value = QString::number(w->value());
//    }
    else if (dynamic_cast<UserSField*>(field))
    {
        FieldEditor* w = static_cast<FieldEditor*>(editor);
        value = w->getData();
    }
    else if (dynamic_cast<UserMField*>(field))
    {
        FieldEditor* w = static_cast<FieldEditor*>(editor);
        value = w->getData();
    }
    else
    {
        if (QLineEdit* w = dynamic_cast<QLineEdit*>(editor) )
            value = w->text();
        else if (ParametersEditor* w = dynamic_cast<ParametersEditor*>(editor) )
            value = w->text();
    }

    QString text = model->data(index, Qt::DisplayRole).toString();
    if (text == "...") text = model->data(index, Qt::EditRole).toString();
    if (value == text) return;
    modelP->setData(index, value);
}

void ParametersDelegate::onCloseEditor()
{
    QWidget* editor = qobject_cast<QWidget*>(sender());
//    emit commitData(editor);
//    emit closeEditor(editor);
    editor->close();
}

void ParametersDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    ParametersModel* modelP = (ParametersModel*) index.model();
    ParametersItemField* item = dynamic_cast<ParametersItemField*>(modelP->itemFromIndex(index));
    if (item)
    {
        SoField* field = item->field();
        SoSFNode* f = dynamic_cast<SoSFNode*>(field);
        if (f) {
            QStyleOptionButton optionButton;
            QRect r = option.rect;
            r.setLeft(r.right() - 1.5*r.height());
            optionButton.rect = r;
            optionButton.text = "...";
            optionButton.state = QStyle::State_Enabled;
            QApplication::style()->drawControl(QStyle::CE_PushButton, &optionButton, painter);
        }
    }
}

#include <QMessageBox>
bool ParametersDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        ParametersModel* modelP = (ParametersModel*) index.model();
        ParametersItemField* item = dynamic_cast<ParametersItemField*>(modelP->itemFromIndex(index));
        if (item)
        {
            SoField* field = item->field();
            SoSFNode* f = dynamic_cast<SoSFNode*>(field);
            if (f) {
                QRect r = option.rect;
                r.setLeft(r.right() - 1.5*r.height());
                QMouseEvent * e = (QMouseEvent*) event;
                if (r.contains(e->pos()))
                {
                        QMessageBox::information(0, "reserved", "");
                        return true;
                }
            }
        }

    }
    QStyledItemDelegate::editorEvent(event, model, option, index);
}
