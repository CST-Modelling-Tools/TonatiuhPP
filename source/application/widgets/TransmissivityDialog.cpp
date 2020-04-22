#include <iostream>

#include <QVector>


#include <Inventor/fields/SoSFBool.h>


#include "TransmissivityDialog.h"
#include "kernel/raytracing/TTransmissivity.h"
#include "kernel/raytracing/TTransmissivityFactory.h"

TransmissivityDialog::TransmissivityDialog(QVector< TTransmissivityFactory* > transmissivityFactoryList, QWidget* parent, Qt::WindowFlags f)
    : QDialog(parent, f),
      m_currentTransmissivityIndex(-1),
      m_newTransmissivity(0)
{
    setupUi(this);
    transmissivityCombo->addItem ("---");
    for (int t = 0; t < (int) transmissivityFactoryList.size(); ++t)
    {
        QString transmissivityTypeName(transmissivityFactoryList[t]->CreateTTransmissivity()->getTypeId().getName().getString() );
        m_transmissivityFactoryList.insert(transmissivityTypeName, transmissivityFactoryList[t]);
        transmissivityCombo->addItem(transmissivityFactoryList[t]->TTransmissivityIcon(), transmissivityFactoryList[t]->TTransmissivityName(), transmissivityTypeName);
    }

    connect(transmissivityCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeTransmissivityType(int)) );
    //change parameter signal
    connect(transmissivityParameters, SIGNAL(valueModificated(SoNode*,QString,QString)), this, SLOT(SetValue(SoNode*,QString,QString)) );

}

/*!
 * Returns defined transmissivity node.
 */
TTransmissivity* TransmissivityDialog::GetTransmissivity() const
{
    return m_newTransmissivity;
}
/*
 * Set new new value into the parameter field
 */
void TransmissivityDialog::SetValue(SoNode* node, QString paramenterName, QString newValue)
{
    double value = newValue.toDouble();
    if (value>=0) {

        SoField* parameterField = node->getField(SbName(paramenterName.toStdString().c_str() ) );
        if (parameterField)
            parameterField->set(newValue.toStdString().c_str() );
    }
}
void TransmissivityDialog::SetCurrentTransmissivity(TTransmissivity* transmissivity)
{
    if (!transmissivity) return;
    m_currentTransmissivity = static_cast< TTransmissivity* >(transmissivity->copy(true) );

    //Select current Transmissivity
    m_currentTransmissivityIndex = 0;
    if (m_currentTransmissivity)
    {
        QString name(m_currentTransmissivity->getTypeId().getName() );
        m_currentTransmissivityIndex = transmissivityCombo->findData(name);
        transmissivityCombo->setCurrentIndex(m_currentTransmissivityIndex);
    }
}

void TransmissivityDialog::ChangeTransmissivityType(int index)
{
    while ( (m_newTransmissivity!= 0) && m_newTransmissivity->getRefCount() > 0) m_newTransmissivity->unref();

    if (index == 0) m_newTransmissivity = 0;
    else if (index == m_currentTransmissivityIndex)
        m_newTransmissivity = static_cast< TTransmissivity* >(m_currentTransmissivity->copy(true) );
    else
    {
        TTransmissivityFactory* transmissivityFactory = m_transmissivityFactoryList.value(transmissivityCombo->itemData(index).toString() );
        m_newTransmissivity = transmissivityFactory->CreateTTransmissivity();

    }

    transmissivityParameters->SetContainer(m_newTransmissivity, QString() );

}
