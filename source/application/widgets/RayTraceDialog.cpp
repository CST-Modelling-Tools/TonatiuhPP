#include <QAbstractButton>
#include <QFileDialog>
#include <QDir>

#include "kernel/random/RandomDeviateFactory.h"
#include "RayTraceDialog.h"

/**
 * Creates a dialog to ray tracer options with the given \a parent and \a f flags.
 *
 * The variables take the default values.
 */
RayTraceDialog::RayTraceDialog(QWidget* parent, Qt::WindowFlags f):
    QDialog(parent, f),
    m_numRays(0),
    m_selectedRandomFactory(-1),
    m_widthDivisions(200),
    m_heightDivisions(200),
    m_drawPhotons(false),
    m_drawRays(false),
    m_photonMapBufferSize(1000000),
    m_increasePhotonMap(false)
{
    setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(saveChanges()) );
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(applyChanges(QAbstractButton*)) );
}

/**
 * Creates a dialog to ray tracer options with the given \a parent and \a f flags.
 *
 * The variables take the values specified by \a numRats, \a faction, \a drawPhotons and \a increasePhotonMap.
 */
RayTraceDialog::RayTraceDialog(
    int numRays,
    QVector<RandomDeviateFactory*> randomFactoryList, int selectedRandomFactory,
    int widthDivisions, int heightDivisions,
    bool drawRays, bool drawPhotons,
    int photonMapSize, bool increasePhotonMap,
    QWidget* parent, Qt::WindowFlags f
):
    QDialog(parent, f),
    m_numRays(numRays),
    m_selectedRandomFactory(selectedRandomFactory),
    m_widthDivisions(widthDivisions),
    m_heightDivisions(heightDivisions),
    m_drawPhotons(drawPhotons),
    m_drawRays(drawRays),
    m_photonMapBufferSize(photonMapSize),
    m_increasePhotonMap(increasePhotonMap)
{
    setupUi(this);
    raysSpinBox->setValue(m_numRays);
    for (int index = 0; index < randomFactoryList.size(); ++index)
        randomCombo->addItem(randomFactoryList[index]->icon(), randomFactoryList[index]->name() );

    if (m_selectedRandomFactory < 0 && randomFactoryList.size() > 0)
        m_selectedRandomFactory = 0;
    randomCombo->setCurrentIndex(m_selectedRandomFactory);

    widthDivisionsSpinBox->setValue(m_widthDivisions);
    heightDivisionsSpinBox->setValue(m_heightDivisions);

    showRaysCheck->setChecked(m_drawRays);
    showPhotonsCheck->setChecked(m_drawPhotons);

    bufferSizeSpin->setValue(m_photonMapBufferSize);
    if (m_increasePhotonMap)
        increaseMapRadio->setChecked(true);
    else
        newMapRadio->setChecked(true);

    connect(this, SIGNAL(accepted()), this, SLOT(saveChanges()) );
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(applyChanges(QAbstractButton*)) );
}

/**
 * If the applyChanges button is clicked the dialog values are saved.
 */
void RayTraceDialog::applyChanges(QAbstractButton* button)
{
    if (buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
        saveChanges();
}

/**
 * Saves the values of the dialog.
 */
void RayTraceDialog::saveChanges()
{
    m_numRays = raysSpinBox->value();
    m_selectedRandomFactory = randomCombo->currentIndex();
    m_widthDivisions = widthDivisionsSpinBox->value();
    m_heightDivisions = heightDivisionsSpinBox->value();

    m_drawRays = showRaysCheck->isChecked();
    m_drawPhotons = showPhotonsCheck->isChecked();

    m_photonMapBufferSize = bufferSizeSpin->value();
    m_increasePhotonMap = !newMapRadio->isChecked();
}

