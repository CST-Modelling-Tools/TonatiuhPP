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
***************************************************************************/

#ifndef CMDLIGHTPOSITIONMODIFIED_H_
#define CMDLIGHTPOSITIONMODIFIED_H_

#include <QDateTime>
#include <QUndoCommand>

class TLightKit;

class CmdLightPositionModified : public QUndoCommand
{

public:
	CmdLightPositionModified( TLightKit* light, double azimuth, double zenith, QUndoCommand* parent = 0 );
	~CmdLightPositionModified();

	virtual void undo();
    virtual void redo();

private:
	TLightKit* lightKit;

	double m_newAzimuth;
	double m_newZenith;

	double m_oldAzimuth;
	double m_oldZenith;
};

#endif /*CMDLIGHTPOSITIONMODIFIED_H_*/
