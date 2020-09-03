#include "SceneFilter.h"

#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "tree/SceneTreeModel.h"


SceneFilter::SceneFilter(QObject* parent):
    QSortFilterProxyModel(parent)
{

}

void SceneFilter::addShapeType(QString shape)
{
    if (!m_shapes.contains(shape))
        m_shapes << shape;
}

bool SceneFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    if (m_shapes.isEmpty()) return true;

    SceneTreeModel* model = dynamic_cast<SceneTreeModel*>(sourceModel());
    QModelIndex index = model->index(sourceRow, 0, sourceParent);
    InstanceNode* instance = model->getInstance(index);
    if (!instance) return false;
    SoNode* node = instance->getNode();
    if (!node) return false;

    if (node->getTypeId() == TShapeKit::getClassTypeId())
    {
        TShapeKit* kit = static_cast<TShapeKit*>(node);
        ShapeRT* shape = static_cast<ShapeRT*>(kit->shapeRT.getValue());
        if (!shape) return false;
        return m_shapes.contains(shape->getTypeName());
    }

    return true;
}
