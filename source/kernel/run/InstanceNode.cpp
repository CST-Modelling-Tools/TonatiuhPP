#include "InstanceNode.h"

#include <iostream>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "TonatiuhFunctions.h"
#include "kernel/material/MaterialAbstract.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/shape/ShapeAbstract.h"
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Transform.h"
#include "scene/TShapeKit.h"
#include "shape//DifferentialGeometry.h"
#include "sun/TLightKit.h"
#include "trackers/TrackerAbstract.h"


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
    child->setParent(this);
}
/**
 * Inserts the \a instanceChild node as child number \a row.
 **/
void InstanceNode::insertChild(int row, InstanceNode* child)
{
    if (row > children.size())
        row = children.size();
    children.insert(row, child);
    child->setParent(this);
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
QString InstanceNode::GetNodeURL() const
{
    QString url;
    if (m_parent) url = m_parent->GetNodeURL();
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

bool InstanceNode::intersect(const Ray& rayIn, RandomAbstract& rand, bool& isShapeFront, InstanceNode*& shapeNode, Ray& rayOut)
{
    if (!m_box.intersect(rayIn)) return false;

    if (!getNode()->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) ) // nodekit
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
    else // shapekit
    {
        ShapeAbstract* shape = 0;
        MaterialAbstract* material = 0;
        if (children[0]->getNode()->getTypeId().isDerivedFrom(ShapeAbstract::getClassTypeId() ) )
        {
            shape = (ShapeAbstract*) children[0]->m_node;
            if (children.size() > 1)
                material = (MaterialAbstract*) children[1]->m_node;
        }
        else if (children.count() > 1)
        {
            shape = (ShapeAbstract*) children[1]->m_node;
            material = (MaterialAbstract*) children[0]->m_node;
        }

        if (!shape) return false;

        Ray rayLocal = m_transform.transformInverse(rayIn);
        double thit = 0.;
        DifferentialGeometry dg;
        if (!shape->intersect(rayLocal, &thit, &dg)) return false;
        rayIn.tMax = thit;
        isShapeFront = dg.isFront;
        shapeNode = this;

        if (!material) return false;
        Ray ray;
        if (material->OutputRay(rayLocal, dg, rand, ray))
        {
            rayOut = m_transform.transformDirect(ray);
            return true;
        }
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
    SoBaseKit* node = (SoBaseKit*) m_node;
    if (!node) return;

    if (dynamic_cast<TSeparatorKit*>(node))
    {
        SoTransform* t = (SoTransform*) node->getPart("transform", true);
        m_transform = tParent*tgf::makeTransform(t);

        BoundingBox box;
        for (InstanceNode* child : children)
        {
            child->updateTree(m_transform);
            box.expand(child->m_box);
        }
        m_box = box;
    }
    else if (dynamic_cast<TShapeKit*>(node))
    {
        for (InstanceNode* child : children)
        {
            if (!dynamic_cast<ShapeAbstract*>(child->m_node)) continue;

            if (SoTransform* t = (SoTransform*) node->getPart("transform", false))
                m_transform = tParent*tgf::makeTransform(t);
            else
                m_transform = tParent;

            ShapeAbstract* shape = (ShapeAbstract*) child->m_node;
            m_box = m_transform(shape->getBox());
            break;
        }
    }
}

void InstanceNode::collectShapeTransforms(QStringList disabledNodes, QVector<QPair<TShapeKit*, Transform> >& shapes)
{
    if (disabledNodes.contains(GetNodeURL())) return;

    if (dynamic_cast<TSeparatorKit*>(m_node))
    {
        for (InstanceNode* child : children)
            child->collectShapeTransforms(disabledNodes, shapes);
    }
    else if (TShapeKit* shape = dynamic_cast<TShapeKit*>(m_node))
    {
        shapes << QPair<TShapeKit*, Transform>(shape, getTransform());
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
