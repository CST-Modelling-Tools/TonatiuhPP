#include "CmdAirModified.h"

#include "kernel/air/AirKit.h"
#include "application/tree/SceneModel.h"
#include "kernel/scene/TSceneKit.h"


CmdAirModified::CmdAirModified(AirKit* air, SceneModel* model, QUndoCommand* parent):
    QUndoCommand("Air modified", parent),
    m_model(model)
{
    m_airOld = (AirKit*) m_model->getSceneKit()->getPart("world.air", false);
    m_airOld->ref();

    m_air = air;
    m_air->ref();
}

CmdAirModified::~CmdAirModified()
{
    m_airOld->unref();
    m_air->unref();
}

void CmdAirModified::undo()
{
    m_model->replaceAir(m_airOld);
}

void CmdAirModified::redo()
{
    m_model->replaceAir(m_air);
}
