#include <QString>

#include <Inventor/SbString.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "ParametersItem.h"

void updateItem(void *data, SoSensor* )
{
    ParametersItem* parametersItem = (ParametersItem *) data;
    parametersItem->setData(parametersItem->data(Qt::DisplayRole), Qt::DisplayRole );
}

ParametersItem::ParametersItem( QString text,  bool editable, SoField* field  )
:QStandardItem( text ),
 m_pField( field ),
 m_pFieldSensor( 0 ),
 m_text( text )
{
     setEditable (editable );
     m_pFieldSensor = new SoFieldSensor(updateItem, this);
     m_pFieldSensor->attach( m_pField );

}

ParametersItem::~ParametersItem()
{
    delete m_pFieldSensor;
}

QVariant ParametersItem::data ( int role ) const
{
    if( role == Qt::DisplayRole )
    {
        if ( column() == 0 ) return m_text;

        SbString fieldValue = "null";
        m_pField->get( fieldValue );
        return QString( fieldValue.getString() );
    }
    else
        return QStandardItem::data( role );
}

void ParametersItem::setData ( const QVariant& value, int role )
{
    if( role == Qt::EditRole ){
        if ( column() == 1 )
            m_text = value.toString();
    }
    else if( role == Qt::UserRole )
    {
        if( m_pField ) m_pField->set( value.toString().toStdString().c_str() );
    }
    else
        return QStandardItem::setData( value, role );
}

QString ParametersItem::text () const
{
    return m_text;
}
