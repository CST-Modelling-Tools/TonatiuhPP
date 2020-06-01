#include "SceneFilter.h"

#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "tree/SceneModel.h"


SceneFilter::SceneFilter(QObject* parent):
    QSortFilterProxyModel(parent)
{

}

/*!
 * Adds new node type to de filter.
 */
void SceneFilter::AddShapeTypeFilter(QString shape)
{
    if (!m_shapes.contains(shape))
        m_shapes << shape;
}

/*!
 * Sets the shape filters to \a  shapeTypeFilters. Previously defined shape filters will be removed.
 */
void SceneFilter::SetShapeTypeFilters(QVector<QString> shapes)
{
    m_shapes = shapes;
}

/*!
 * Returns true if the \a m_nodetypeList contains the type of the element defined with \a sourceRow and \a sourceParent
 */
bool SceneFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    if (m_shapes.isEmpty()) return true;

    SceneModel* sceneModel = dynamic_cast<SceneModel*>(sourceModel());
    QModelIndex index = sceneModel->index(sourceRow, 0, sourceParent);
    InstanceNode* instance = sceneModel->getInstance(index);
    if (!instance) return false;
    SoNode* node = instance->getNode();
    if (!node) return false;

    if (node->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId()))
        return true;

    if (node->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId()))
    {
        TShapeKit* shapeKit = static_cast<TShapeKit*>(node);
        ShapeRT* shape = static_cast<ShapeRT*>(shapeKit->shapeRT.getValue());
        if (shape && m_shapes.contains(shape->getTypeName()))
            return true;
    }

    return false;
}
