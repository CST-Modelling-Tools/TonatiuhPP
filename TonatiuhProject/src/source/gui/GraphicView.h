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

#ifndef GRAPHICVIEW_H_
#define GRAPHICVIEW_H_

#include <QAbstractItemView>

class GraphicRoot;
class QModelIndex;
class SbViewportRegion;
class SoCamera;
class SoQtExaminerViewer;
class TSeparatorKit;

//!  GraphicView class implements a 3D representation of items from a model.
/*!
  GraphicView provides a model/view implementation of a 3D view. Implements the interfaces defined by
  the QAbstractItemView class to allow it to display data provided by models derived from the QAbstractItemModel class.
*/

class GraphicView : public QAbstractItemView
{
    Q_OBJECT

public:
    GraphicView( QWidget* parent = 0 );
    ~GraphicView();

    void SetSceneGraph( GraphicRoot* sceneGraphRoot );

    SbViewportRegion GetViewportRegion() const;
    SoCamera* GetCamera() const;
    virtual QModelIndex indexAt ( const QPoint & point ) const;
    virtual void scrollTo ( const QModelIndex & index, ScrollHint hint = EnsureVisible );
    virtual QRect visualRect ( const QModelIndex & index ) const;

    void ViewDecoration( bool view );
    void ViewCoordinateSystem( bool view );

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void currentChanged ( const QModelIndex & current, const QModelIndex & previous );

protected:
    virtual void setSelection ( const QRect & rect, QItemSelectionModel::SelectionFlags flags );
    virtual int horizontalOffset () const;
    virtual int verticalOffset() const;
    virtual bool isIndexHidden ( const QModelIndex & index ) const;
    virtual QModelIndex moveCursor ( CursorAction cursorAction, Qt::KeyboardModifiers modifiers );
    virtual QRegion visualRegionForSelection ( const QItemSelection & selection ) const;

private:
    GraphicRoot* m_sceneGraphRoot;
    SoQtExaminerViewer* m_myRenderArea;
};

#endif /*GRAPHICVIEW_H_*/
