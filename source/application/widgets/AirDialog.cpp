#include "AirDialog.h"
#include "ui_airdialog.h"

#include <Inventor/fields/SoField.h>
#include "kernel/air/AirAbstract.h"
#include "kernel/air/AirFactory.h"


AirDialog::AirDialog(QMap<QString, AirFactory*> airMap, QWidget* parent, Qt::WindowFlags f):
    QDialog(parent, f),
    ui(new Ui::AirDialog),
    m_airMap(airMap),
    m_index(-1),
    m_air(0)
{
    ui->setupUi(this);

    for (AirFactory* f : airMap)
        ui->comboBox->addItem(f->icon(), f->name());
    ui->comboBox->setCurrentIndex(-1);

    connect(
        ui->comboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(changeModel(int))
    );
    connect(
        ui->airParameters, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(setValue(SoNode*, QString, QString))
    );
}

AirDialog::~AirDialog()
{
    delete ui;
}

void AirDialog::setModel(AirAbstract* air)
{    
    if (!air) return;
    m_airOld = static_cast<AirAbstract*>(air->copy(true));
    m_index = 0;
    if (m_airOld) {
        m_index = ui->comboBox->findText(m_airOld->getTypeName());
        ui->comboBox->setCurrentIndex(m_index);
    }
}

void AirDialog::changeModel(int index)
{
    while (m_air && m_air->getRefCount() > 0)
        m_air->unref();

    if (index == m_index)
        m_air = static_cast<AirAbstract*>(m_airOld->copy(true));
    else {
        AirFactory* f = m_airMap[ui->comboBox->itemText(index)];
        m_air = f->create();
    }

    ui->airParameters->SetContainer(m_air, QString());
}

void AirDialog::setValue(SoNode* node, QString parameter, QString value)
{
    SoField* field = node->getField(SbName(parameter.toStdString().c_str()));
    if (field)
        field->set(value.toStdString().c_str());
}
