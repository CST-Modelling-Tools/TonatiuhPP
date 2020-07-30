#pragma once

#include <QDateTime>
#include <QUndoCommand>

class SunKit;

class CmdLightPositionModified : public QUndoCommand
{

public:
    CmdLightPositionModified(SunKit* light, double azimuth, double zenith, QUndoCommand* parent = 0);
    ~CmdLightPositionModified();

    void undo();
    void redo();

private:
    SunKit* sunKit;

    double m_azimuth;
    double m_elevation;

    double m_azimuthOld;
    double m_elevationOld;
};
