#include "ParametersView.h"

#include <QComboBox>
#include <QLineEdit>
#include <QTreeView>
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


/**
 * Creates a new ParametersView for the parameters in the \a fieldContainer with parent \a parent.
 *
 * The container name is \a containerName.
 */
ParametersView::ParametersView(QWidget* parent):
    QTreeView(parent)
{
    m_delegate = new ParametersDelegate;
    setItemDelegate(m_delegate);

    m_model = new ParametersModel;
    m_model->SetEditable(true);
    setModel(m_model);

    m_node = 0;
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
void ParametersView::SetContainer(SoNode* node)
{
    m_model->clear();
    m_model->setHorizontalHeaderLabels({"Parameter", "Value"});

    m_node = node;
    if (!m_node) return;

    SoFieldList fields;
    int nMax = m_node->getFields(fields);
    for (int n = 0; n < nMax; ++n)
    {
        SoField* field = fields.get(n);
        if (!field) continue;
        SbString value;
        field->get(value);
        SbName name;
        if (!m_node->getFieldName(field, name)) continue;
        ParametersItem* itemName = new ParametersItem(name.getString(), false, field);
        m_model->setItem(n, 0, itemName);
        ParametersItem* itemValue = new ParametersItem(value.getString(), true, field);
        m_model->setItem(n, 1, itemValue);
    }

//    resizeColumnToContents(1);
}

/**
 * Sets if the parameters values can be modified.
 */
void ParametersView::SetEditable(bool editable)
{
    m_model->SetEditable(editable);
}

void ParametersView::closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint)
{
    SoField* field = m_model->getData(currentIndex())->getField();
    QString value;

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

    if (!value.isEmpty())
    {
        SbName name;
        m_node->getFieldName(field, name);
        emit valueModified(m_node, name.getString(), value);
    }

    QTreeView::closeEditor(editor, hint);
}
