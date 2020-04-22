#include <QComboBox>
#include <QLineEdit>
#include <QModelIndex>
#include <QString>

#include <Inventor/fields/SoSFEnum.h> 

#include "ParametersItem.h"
#include "ParametersDelegate.h"
#include "ParametersModel.h"
#include "libraries/fields/FieldEditor.h"
#include "libraries/fields/UserSField.h"
#include "libraries/fields/UserMField.h"

ParametersDelegate::ParametersDelegate(  QObject* parent)
 : QItemDelegate(parent)
{
    
} 

ParametersDelegate::~ParametersDelegate()
{
    
}

QWidget* ParametersDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex &index) const
{
    const ParametersModel* model = static_cast< const ParametersModel* >( index.model() );
    SoField* field = model->ModelItem( index )->GetField();

    if( field->getTypeId().isDerivedFrom( SoSFEnum::getClassTypeId() ) )
    {
        QComboBox* editor = new QComboBox( parent );
        SoSFEnum* enumField = static_cast< SoSFEnum* >( field );
        for( int i = 0; i < enumField->getNumEnums() ; ++i )
        {
            SbName enumName;
            enumField->getEnum( i , enumName );
            editor->addItem( enumName.getString() );
        }
        return editor;
    }
    else if( field->getTypeId().isDerivedFrom( UserSField::getClassTypeId() ) )
    {
        UserSField* mField = static_cast< UserSField* >(field);

        FieldEditor* fieldEdit = mField->GetEditor();
        fieldEdit->setGeometry(option.rect);
        fieldEdit->setParent(parent);

        connect( fieldEdit, SIGNAL( editingFinished( )  ), this, SLOT( CloseEditor() ));
        return fieldEdit;
    }
    else if( field->getTypeId().isDerivedFrom( UserMField::getClassTypeId() ) )
    {
        UserMField* mField = static_cast< UserMField* >(field);

        FieldEditor* fieldEdit = mField->GetEditor();
        fieldEdit->setGeometry(option.rect);
        fieldEdit->setParent(parent);

        connect( fieldEdit, SIGNAL( editingFinished( )  ), this, SLOT( CloseEditor() ));
        return fieldEdit;
    }
    else
    {
        QLineEdit* editor = new QLineEdit(parent);
        return editor;
    }
}

void ParametersDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    const ParametersModel* model = static_cast< const ParametersModel* >( index.model() );
    SoField* field = model->ModelItem( index )->GetField();
    
    if( field->getTypeId().isDerivedFrom( SoSFEnum::getClassTypeId() ) )
    {

        SoSFEnum* enumField = static_cast< SoSFEnum* >( field );
        int selectedIndex = enumField->getValue();
        
        QComboBox* combo = static_cast<QComboBox *>(editor);
        combo->setCurrentIndex( selectedIndex );

    }
    else if( field->getTypeId().isDerivedFrom( UserSField::getClassTypeId() ) )
    {

        FieldEditor* fieldEdit = static_cast< FieldEditor *>(editor);

        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        fieldEdit->SetData( value );

    }
    else if( field->getTypeId().isDerivedFrom( UserMField::getClassTypeId() ) )
    {

        FieldEditor* fieldEdit = static_cast< FieldEditor *>(editor);

        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        fieldEdit->SetData( value );
    }
    else
    {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
            
        QLineEdit  *textEdit = static_cast<QLineEdit *>(editor);
        textEdit->setText(value);
    }

}

void ParametersDelegate::CloseEditor()
{
    QWidget *editor = qobject_cast<QWidget *>(sender());
    emit closeEditor(editor);
}
