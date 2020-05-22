#pragma once

#include <QDateTime>
#include <QUndoCommand>

class SunKit;

class CmdLightPositionModified : public QUndoCommand
{

public:
    CmdLightPositionModified(SunKit* light, double azimuth, double zenith, QUndoCommand* parent = 0 );
    ~CmdLightPositionModified();

    void undo();
    void redo();

private:
    SunKit* lightKit;

    double m_newAzimuth;
    double m_newZenith;

    double m_oldAzimuth;
    double m_oldZenith;
};
