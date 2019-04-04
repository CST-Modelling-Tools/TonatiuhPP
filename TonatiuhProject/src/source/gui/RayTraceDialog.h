/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef RAYTRACEDIALOG_H_
#define RAYTRACEDIALOG_H_

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
	RayTraceDialog( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	RayTraceDialog( int numRays,
			QVector< RandomDeviateFactory* > randomFactoryList, int selectedRandomFactory = 0,
			int widthDivisions = 200,int heightDivisions = 200,
			bool drawRays = true, bool drawPhotons = false,
			int photonMapSize = 1000000, bool increasePhotonMap = false,
				QWidget * parent = 0, Qt::WindowFlags f = 0 );
    ~RayTraceDialog();

    bool DrawPhotons() const;
    bool DrawRays() const;
    int GetHeightDivisions() const;
    int GetNumRays() const;
    int GetPhotonMapBufferSize() const;
    int GetRandomDeviateFactoryIndex() const;
    int GetWidthDivisions() const;
    bool IncreasePhotonMap() const;;

public slots:
	void applyChanges( QAbstractButton* button );
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

#endif /*RAYTRACEDIALOG_H_*/
