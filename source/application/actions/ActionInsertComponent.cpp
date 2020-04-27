/*!
* \class ActionInsertComponent
* \brief The ActionInsertComponent class is the action to insert a component in the scene.
*
* The ActionInsertComponent class is the action added to components menu for each component plugin.
*
*/

#include "ActionInsertComponent.h"

/*!
 * Creates an action object with \a text and \a parent as parent object.
 */
ActionInsertComponent::ActionInsertComponent( const QString& text, QObject* parent, ComponentFactory* pComponentFactory )
:QAction( text, parent ),
 m_pComponentFactory( pComponentFactory )

{

}

/*!
 * Destroys the action object.
 */
ActionInsertComponent::~ActionInsertComponent()
{

}

/*!
 * Emits create component signal.
 */
void ActionInsertComponent::OnActionInsertComponentTriggered()
{
    emit CreateComponent( m_pComponentFactory );
}
