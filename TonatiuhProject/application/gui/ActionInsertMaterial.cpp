#include "ActionInsertMaterial.h"

/**
 * Constructor.
 */
ActionInsertMaterial::ActionInsertMaterial( const QString& text, QObject* parent, TMaterialFactory* pTMaterialFactory )
: QAction(text,parent), m_pTMaterialFactory( pTMaterialFactory )
{
}

/**
 * Destructor.
 */
ActionInsertMaterial::~ActionInsertMaterial()
{
}

/**
 * Emits a create material signal.
 */
void ActionInsertMaterial::OnActionInsertMaterialTriggered()
{
    emit CreateMaterial( m_pTMaterialFactory );
}
