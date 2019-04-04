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

#ifndef PARAMETERSVIEW_H_
#define PARAMETERSVIEW_H_

#include <QTabWidget>

class SoBaseKit;
class SoNode;

//! The ParametersView class provides a tabbed view for nodes parameters.
/*!
 * ParametersView provides a page for each of the parts of a node. The tab bar shows the node part name and the tab area shows the part parameters list.
 *
 * The tab area containes a FieldContainerWidget.
 *
 * \sa FieldContainerWidget
 *
*/

class ParametersView : public QTabWidget
{
	Q_OBJECT

public:
	ParametersView( QWidget* parent = 0 );
	~ParametersView();

	void SelectionChangedToPart( SoNode* coinPart );
	void SelectionChangedToKit( SoBaseKit* coinNode );
	void UpdateView();

signals:
	 /*!
	 * Emmits a signal when a parameters of the view is modified.
	 *
	 * The modified paramenters is a \a coinNode paramenter named \a parameterName.
	 *
	 * The new value for the parameter is \a newValue.
	 * */
	 void valueModificated( SoNode* coinNode, QString parameterName, QString newValue );

public slots:
	void SetValue( SoNode* node, QString paramenterName, QString newValue );

private:
	void AddTab( SoNode* coinNode, QString partName );
	QStringList ContainerNodeParts( SoBaseKit* coinNode );

	SoNode* m_actualCoinNode;
	bool m_isPart;

};

#endif /*PARAMETERSVIEW_H_*/
