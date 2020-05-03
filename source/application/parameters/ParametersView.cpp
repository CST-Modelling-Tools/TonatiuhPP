#include "ParametersView.h"

#include <iostream>
#include <QComboBox>
#include <QLineEdit>
#include <QTreeView>
#include <QVBoxLayout>
#include <QHeaderView>

#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/nodes/SoNode.h>

#include "libraries/fields/FieldEditor.h"
#include "libraries/fields/UserMField.h"
#include "libraries/fields/UserSField.h"

#include "ParametersDelegate.h"
#include "ParametersItem.h"
#include "ParametersModel.h"

/*!
 * Creates an empty widget.
 */
ParametersView::ParametersView(QWidget* parent):
    QTreeView(parent),
    m_name(QString("") ),
    m_delegate(0),
    m_node(0),
    m_model(0),
    m_index()
{
//    setAlternatingRowColors(true);

    m_delegate = new ParametersDelegate;
    setItemDelegate(m_delegate);

    m_model = new ParametersModel();
    m_model->SetEditable(true);
    setModel(m_model);
}

/**
 * Creates a new ParametersView for the parameters in the \a fieldContainer with parent \a parent.
 *
 * The container name is \a containerName.
 */
ParametersView::ParametersView(SoNode* node, QString name, QWidget* parent):
    QTreeView(parent),
    m_node(node),
    m_name(name),
    m_delegate(0),
    m_model(0),
    m_index()
{
    setStyleSheet(R"(
QAbstractItemView {
outline: 0;
}

QAbstractItemView::item:selected {
color: black;
background-color: #c8dbe5;
}

QAbstractItemView::item:hover:selected {
background-color: #c8dbe5;
}

QAbstractItemView::item:hover:!selected {
background-color: #eeeeee;
}

QHeaderView::section
{
background-color: #d2dddb;
}
    )");

    m_delegate = new ParametersDelegate;
    setItemDelegate(m_delegate);

    m_model = new ParametersModel;
    m_model->SetEditable(true);
    m_model->setHorizontalHeaderLabels({"Parameter", "Value"});
    setModel(m_model);

    if (m_node) ReadFields();
    resizeColumnToContents(0);
}

/**
 * Destroys the ParametersView object.
 */
ParametersView::~ParametersView()
{
    delete m_delegate;
    delete m_model;
}

/*!
 * Sets \a fieldContainer as widget container and \a containerName as its containerName name.
 */
void ParametersView::SetContainer(SoNode* node, QString name)
{
    m_model->clear();

    m_node = node;
    m_name = name;

    if (m_node) ReadFields();
    resizeColumnToContents(1);
}

/**
 * Sets if the parameters values can be modified.
 */
void ParametersView::SetEditable(bool editable)
{
    m_model->SetEditable(editable);
}

/*!
 * Sets \a current as the view current element index.
 */
void ParametersView::currentChanged(const QModelIndex& current, const QModelIndex& /*previous*/)
{
    m_index = current;
}

void ParametersView::closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint)
{
    QString value;
    SoField* field = m_model->ModelItem(m_index)->getField();

    if (dynamic_cast<SoSFEnum*>(field))
    {
        QComboBox* w = qobject_cast<QComboBox*>(editor);
        value = w->currentText();
    }
    else if (dynamic_cast<SoSFBool*>(field))
    {
        QComboBox* w = qobject_cast<QComboBox*>(editor);
        value = w->currentText();
    }
    else if (dynamic_cast<UserSField*>(field))
    {
        FieldEditor* w = static_cast<FieldEditor*>(editor);
        value = w->GetData();
    }
    else if (dynamic_cast<UserMField*>(field))
    {
        FieldEditor* w = static_cast< FieldEditor*>(editor);
        value = w->GetData();
    }
    else
    {
        QLineEdit* w = static_cast<QLineEdit*>(editor);
        value = w->text();
    }

    SbName nameField;
    m_node->getFieldName(field, nameField);
    QString name = nameField.getString();

    if (!value.isEmpty())
        emit valueModified(m_node, name, value);

    QTreeView::closeEditor(editor, hint);
}

/**
 * Reads container parameters and for each parameters adds its name and value to the widget.
 */
void ParametersView::ReadFields()
{
    m_model->clear();
    m_model->setHorizontalHeaderLabels({"Parameter", "Value"});

    SoFieldList fields;
    int nMax = m_node->getFields(fields);

    SoField* field;
    SbName name;
    SbString value;

    for (int n = 0; n < nMax; ++n)
    {
        field = fields.get(n);
        if (!field) continue;
        field->get(value);
        if (m_node->getFieldName(field, name) )
        {
            ParametersItem* itemName = new ParametersItem(name.getString(), false, field);
            m_model->setItem(n, 0, itemName);
            ParametersItem* itemValue = new ParametersItem(value.getString(), true, field);
            m_model->setItem(n, 1, itemValue);
        }
    }
}
