#include "ActionInsertShape.h"
#include "kernel/raytracing/TShapeFactory.h"


ActionInsertShape::ActionInsertShape (const QString& text, QObject* parent, TShapeFactory* pTShapeFactory)
    : QAction(text,parent), m_pTShapeFactory(pTShapeFactory)
{
}

/**
 * Emits a create shape signal.
 */
void ActionInsertShape::OnActionInsertShapeTriggered()
{
    emit CreateShape(m_pTShapeFactory);
}
