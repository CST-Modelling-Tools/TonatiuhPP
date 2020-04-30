#include <iostream>
#include <QComboBox>
#include <QLineEdit>
#include <QTreeView>
#include <QVBoxLayout>

#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/nodes/SoNode.h>

#include "FieldContainerWidget.h"
#include "libraries/fields/FieldEditor.h"
#include "ParametersDelegate.h"
#include "ParametersItem.h"
#include "ParametersModel.h"
#include "libraries/fields/UserMField.h"
#include "libraries/fields/UserSField.h"
#include <QHeaderView>

/*!
 * Creates an empty widget.
 */
FieldContainerWidget::FieldContainerWidget(QWidget* parent):
    QTreeView(parent),
    m_containerName(QString("") ),
    m_currentIndex(),
    m_delegate(0),
    m_pFieldContainer(0),
    m_pModel(0)
{
//    setAlternatingRowColors(true);

    m_delegate = new ParametersDelegate;
    setItemDelegate(m_delegate);

    m_pModel = new ParametersModel();
    m_pModel->SetEditable(true);
    setModel(m_pModel);
}

/**
 * Creates a new FieldContainerWidget for the parameters in the \a fieldContainer with parent \a parent.
 *
 * The container name is \a containerName.
 */
FieldContainerWidget::FieldContainerWidget(SoNode* fieldContainer, QString containerName, QWidget* parent):
    QTreeView(parent),
    m_containerName(containerName),
    m_currentIndex(),
    m_delegate(0),
    m_pFieldContainer(fieldContainer),
    m_pModel(0)
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

    m_pModel = new ParametersModel;
    m_pModel->SetEditable(true);
    m_pModel->setHorizontalHeaderLabels({"Parameter", "Value"});
    setModel(m_pModel);

    if (m_pFieldContainer) ReadFields();
    resizeColumnToContents(0);
}

/**
 * Destroys the FieldContainerWidget object.
 */
FieldContainerWidget::~FieldContainerWidget()
{
    delete m_delegate;
    delete m_pModel;
}

/*!
 * Sets \a fieldContainer as widget container and \a containerName as its containerName name.
 */
void FieldContainerWidget::SetContainer(SoNode* fieldContainer, QString containerName)
{
    m_pModel->clear();

    m_pFieldContainer = fieldContainer;
    m_containerName = containerName;

    if (m_pFieldContainer) ReadFields();
    resizeColumnToContents(1);
}

/**
 * Sets if the parameters values can be modified.
 */
void FieldContainerWidget::SetEditable(bool editable)
{
    m_pModel->SetEditable(editable);
}

/*!
 * Sets \a current as the view current element index.
 */
void FieldContainerWidget::currentChanged(const QModelIndex& current, const QModelIndex& /*previous*/)
{
    m_currentIndex = current;
}

void FieldContainerWidget::closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint)
{
    QString newValue;
    SoField* field = m_pModel->ModelItem(m_currentIndex)->GetField();
    if (field->getTypeId().isDerivedFrom(SoSFEnum::getClassTypeId() ) )
    {
        QComboBox* combo = qobject_cast<QComboBox*>(editor);
        newValue = combo->currentText();
    }

    else if (field->getTypeId().isDerivedFrom(UserSField::getClassTypeId() ) )
    {
        FieldEditor* fieldEdit = static_cast< FieldEditor*>(editor);
        newValue = fieldEdit->GetData();

    }

    else if (field->getTypeId().isDerivedFrom(UserMField::getClassTypeId() ) )
    {
        FieldEditor* fieldEdit = static_cast< FieldEditor*>(editor);
        newValue = fieldEdit->GetData();

    }
    else
    {
        QLineEdit* textEdit = qobject_cast<QLineEdit*>(editor);
        newValue = textEdit->text();
    }
    SbName fieldName;
    m_pFieldContainer->getFieldName(field, fieldName);
    QString parameterName(fieldName.getString() );

    if (!newValue.isEmpty() ) emit valueModificated(m_pFieldContainer, parameterName, newValue);
    QTreeView::closeEditor(editor, hint);

}

/**
 * Reads container parameters and for each parameters adds its name and value to de widget.
 */
void FieldContainerWidget::ReadFields()
{
    m_pModel->clear();
    m_pModel->setHorizontalHeaderLabels(QStringList() << tr("Parameter") << tr("Value") );

    SoFieldList fieldList;
    int totalFields = m_pFieldContainer->getFields(fieldList);

    SoField* pField = 0;
    SbName fieldName;
    SbString fieldValue = "null";

    for (int index = 0; index < totalFields; ++index)
    {
        pField = fieldList.get(index);
        if (pField)
        {
            pField->get(fieldValue);
            if (m_pFieldContainer->getFieldName(pField, fieldName) )
            {
                m_pModel->setItem(index, false, new ParametersItem (QString(fieldName.getString()), false, pField));
                ParametersItem* valueItem = new ParametersItem (QString(fieldValue.getString()), true, pField);
                m_pModel->setItem(index, true, valueItem);
            }
        }
    }
}
