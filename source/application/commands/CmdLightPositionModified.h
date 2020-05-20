#pragma once

#include <QDateTime>
#include <QUndoCommand>

class TLightKit;

class CmdLightPositionModified : public QUndoCommand
{

public:
    CmdLightPositionModified(TLightKit* light, double azimuth, double zenith, QUndoCommand* parent = 0 );
    ~CmdLightPositionModified();

    void undo();
    void redo();

private:
    TLightKit* lightKit;

    double m_newAzimuth;
    double m_newZenith;

    double m_oldAzimuth;
    double m_oldZenith;
};
