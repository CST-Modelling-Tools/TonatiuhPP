#pragma once

#include <ui_raytracedialog.h>

#include <QDialog>
#include <QString>
#include <QVector>

class RandomDeviateFactory;
class TPhotonMapFactory;

//!  RayTraceDialog class is the dialog to set ray trace options.
/*!
  RayTraceDialog allow to the user modify ray tracer default options.
  The user can modify the number of ray to trace, draw and photon map options.
*/

class RayTraceDialog: public QDialog, private Ui::RayTraceDialog
{
    Q_OBJECT


public:
    RayTraceDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
    RayTraceDialog(int numRays,
                   QVector< RandomDeviateFactory* > randomFactoryList, int selectedRandomFactory = 0,
                   int widthDivisions = 200,int heightDivisions = 200,
                   bool drawRays = true, bool drawPhotons = false,
                   int photonMapSize = 1'000'000, bool increasePhotonMap = false,
                   QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~RayTraceDialog() {}

    bool DrawPhotons() const {return m_drawPhotons;}
    bool DrawRays() const {return m_drawRays;}
    int GetNumRays() const {return m_numRays;}
    int GetWidthDivisions() const {return m_widthDivisions;}
    int GetHeightDivisions() const {return m_heightDivisions;}
    int GetPhotonMapBufferSize() const {return m_photonMapBufferSize;}
    int GetRandomDeviateFactoryIndex() const {return m_selectedRandomFactory;}
    bool IncreasePhotonMap() const;

public slots:
    void applyChanges(QAbstractButton* button);
    void saveChanges();

private:
    bool m_drawPhotons;  /*!<This property holds whether photons are going to be drawn. */
    bool m_drawRays;  /*!<This property holds whether rays are going to be drawn. */
    int m_heightDivisions; /*!<number of height divisions in the sun*/
    bool m_increasePhotonMap; /*!<This property holds whether traced phtons are going to added to the old photon map. */
    int m_numRays; /*!< Number of rays to trace. */
    int m_photonMapBufferSize; /*!< Maximum number of photons int the PhotonMap. */
    int m_selectedRandomFactory; /*!< The index of factory selected from TPhotonMapFactory list. */
    int m_widthDivisions; /*number of width divisions in the sun*/
};
