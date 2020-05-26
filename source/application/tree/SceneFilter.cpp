#include "SceneFilter.h"

#include "kernel/run/InstanceNode.h"
#include "tree/SceneModel.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"


/*!
 * Creates model object
 */
SceneFilter::SceneFilter(QObject* parent):
    QSortFilterProxyModel(parent)
{

}

/*!
 * Adds new node type to de filter.
 */
void SceneFilter::AddShapeTypeFilter(QString shapeType)
{
    if (!m_shapeTypeList.contains(shapeType) )
        m_shapeTypeList << shapeType;
}

/*!
 * Sets the shape filters to \a  shapeTypeFilters. Previously defined shape filters will be removed.
 */
void SceneFilter::SetShapeTypeFilters(QVector<QString> shapeTypeFilters)
{
    m_shapeTypeList.clear();
    m_shapeTypeList = shapeTypeFilters;
}

/*!
 * Returns true if the \a m_nodetypeList contains the type of the element defined with \a sourceRow and \a sourceParent
 */
bool SceneFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    SceneModel* sceneModel = dynamic_cast<SceneModel*> (sourceModel() );
    QModelIndex index = sceneModel->index(sourceRow, 0, sourceParent);

    InstanceNode* nodeInstance = sceneModel->getInstance(index);
    if (!nodeInstance) return false;

    SoNode* node = nodeInstance->getNode();
    if (!node) return false;

    if (node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) ) return true;
    if (node->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) )
    {
        if (m_shapeTypeList.count() < 1) return true;

        TShapeKit* shapeKit = static_cast<TShapeKit*>(node);
        if (!shapeKit) return false;
        ShapeRT* shape = static_cast<ShapeRT*>(shapeKit->getPart("shape", false) );

        if (shape && m_shapeTypeList.contains(shape->getTypeName()))
            return true;
    }

    return false;
}
