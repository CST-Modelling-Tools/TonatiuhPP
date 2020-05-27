#include "InstanceNode.h"

#include <iostream>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "TonatiuhFunctions.h"
#include "kernel/material/MaterialRT.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/shape/ShapeRT.h"
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Transform.h"
#include "scene/TShapeKit.h"
#include "shape//DifferentialGeometry.h"
#include "sun/SunKit.h"
#include "trackers/Tracker.h"


InstanceNode::InstanceNode(SoNode* node):
    m_node(node), m_parent(0)
{

}

InstanceNode::~InstanceNode()
{
    qDeleteAll(children);
}

/**
 * Appends new \a child node to the end of the child list.
 **/
void InstanceNode::addChild(InstanceNode* child)
{
    children << child;
    child->m_parent = this;
}
/**
 * Inserts the \a instanceChild node as child number \a row.
 **/
void InstanceNode::insertChild(int row, InstanceNode* child)
{
    if (row > children.size())
        row = children.size();
    children.insert(row, child);
    child->m_parent = this;
}

void InstanceNode::replaceChild(int row, InstanceNode* child)
{
    children[row] = child;
    child->m_parent = this;
}

bool InstanceNode::operator==(const InstanceNode& other)
{
    return
        m_node == other.m_node &&
        m_parent->m_node == other.m_parent->m_node;
}

/**
 * Returns node URL.
 */
QString InstanceNode::getURL() const
{
    QString url;
    if (m_parent) url = m_parent->getURL();
    url += "/";
    url += m_node->getName().getString();
    return url;
}

void InstanceNode::Print(int level) const
{
    for (int i = 0; i < level; ++i)
        std::cout << " ";

    std::cout << m_node->getTypeId().getName().getString()
              << " has " << children.size()
              << " children " << std::endl;

    for (InstanceNode* child : children)
        child->Print(level++);
}

bool InstanceNode::intersect(const Ray& rayIn, Random& rand, bool& isShapeFront, InstanceNode*& shapeNode, Ray& rayOut)
{
    if (!m_box.intersect(rayIn)) return false;

    if (TShapeKit* kit = dynamic_cast<TShapeKit*>(m_node))
    {
        ShapeRT* shape = kit->getShape();
        if (!shape) return false;

        Ray rayLocal = m_transform.transformInverse(rayIn);
        double thit = 0.;
        DifferentialGeometry dg;
        if (!shape->intersect(rayLocal, &thit, &dg)) return false;
        rayIn.tMax = thit;
        isShapeFront = dg.isFront;
        shapeNode = this;

        MaterialRT* material = kit->getMaterial();
        if (!material) return false;
        Ray ray;
        if (material->OutputRay(rayLocal, dg, rand, ray))
        {
            rayOut = m_transform.transformDirect(ray);
            return true;
        }
    }
    else
    {
        bool hasRayOut = false;
        double t = rayIn.tMax;
        for (InstanceNode* child : children)
        {
            Ray childRayOut;
            bool childShapeFront = true;
            bool isChildOutputRay = child->intersect(rayIn, rand, childShapeFront, child, childRayOut);

            if (rayIn.tMax < t) // tMax mutable
            {
                t = rayIn.tMax;
                isShapeFront = childShapeFront;
                shapeNode = child;

                rayOut = childRayOut;
                hasRayOut = isChildOutputRay;
            }
        }
        return hasRayOut;
    }
    return false;
}

void InstanceNode::extendBoxForLight(SbBox3f* extendedBox)
{
    SoGetBoundingBoxAction* action = new SoGetBoundingBoxAction(SbViewportRegion());
    getNode()->getBoundingBox(action);
    SbBox3f box = action->getXfBoundingBox().project();
    delete action;
    extendedBox->extendBy(box);
}

/**
 * Set node world to object transform to \a nodeTransform .
 */
void InstanceNode::updateTree(const Transform& tParent)
{
    if (TSeparatorKit* separatorKit = dynamic_cast<TSeparatorKit*>(m_node))
    {
        SoTransform* t = (SoTransform*) separatorKit->getPart("transform", true);
        m_transform = tParent*tgf::makeTransform(t);

        BoundingBox box;
        for (InstanceNode* child : children)
        {
            child->updateTree(m_transform);
            box.expand(child->m_box);
        }
        m_box = box;
    }
    else if (TShapeKit* shapeKit = dynamic_cast<TShapeKit*>(m_node))
    {
//        shapeKit->m_shape = (ShapeRT*) shapeKit->getPart("shape", false);
//        shapeKit->m_material = (MaterialRT*) shapeKit->getPart("material", false);

        if (SoTransform* t = (SoTransform*) shapeKit->getPart("transform", false))
            m_transform = tParent*tgf::makeTransform(t);
        else
            m_transform = tParent;

        m_box = m_transform(shapeKit->getShape()->getBox());

    }
}

void InstanceNode::collectShapeTransforms(QStringList disabledNodes, QVector<QPair<TShapeKit*, Transform> >& shapes)
{
    if (disabledNodes.contains(getURL())) return;

    if (dynamic_cast<TSeparatorKit*>(m_node))
    {
        for (InstanceNode* child : children)
            child->collectShapeTransforms(disabledNodes, shapes);
    }
    else if (TShapeKit* shape = dynamic_cast<TShapeKit*>(m_node))
    {
        shapes << QPair<TShapeKit*, Transform>(shape, m_transform);
    }
}

QDataStream& operator<<(QDataStream& s, const InstanceNode& node)
{
    s << node.getNode();
    return s;
}

QDataStream& operator>>(QDataStream& s, const InstanceNode& node)
{
    s >> node;
    return s;
}
