#include <iostream>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/Ray.h"
#include "shape//DifferentialGeometry.h"
#include "InstanceNode.h"
#include "TonatiuhFunctions.h"
#include "kernel/material/MaterialAbstract.h"
#include "kernel/shape/ShapeAbstract.h"
#include "scene/TShapeKit.h"
#include "sun/TLightKit.h"
#include "trackers/TrackerAbstract.h"
#include "kernel/scene/TSeparatorKit.h"


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

bool InstanceNode::intersect(const Ray& rayIn, RandomAbstract& rand, bool* isShapeFront, InstanceNode** modelNode, Ray* rayOut)
{
    if (!m_box.intersect(rayIn)) return false;

    if (!getNode()->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) ) // nodekit
    {
        bool isOutputRay = false;
        double t = rayIn.tMax;
        for (int n = 0; n < children.size(); ++n)
        {
            InstanceNode* child = 0;
            Ray childOutputRay;
            bool childShapeFront = true;
            bool isChildOutputRay = children[n]->intersect(rayIn, rand, &childShapeFront, &child, &childOutputRay);

            if (rayIn.tMax < t) // tMax mutable
            {
                t = rayIn.tMax;
                *modelNode = child;
                *isShapeFront = childShapeFront;

                *rayOut = childOutputRay;
                isOutputRay = isChildOutputRay;
            }
        }
        return isOutputRay;
    }
    else // shapekit
    {
        Ray rayLocal(m_transformWtO(rayIn));

        ShapeAbstract* shape = 0;
        MaterialAbstract* material = 0;
        if (children[0]->getNode()->getTypeId().isDerivedFrom(ShapeAbstract::getClassTypeId() ) )
        {
            shape = static_cast<ShapeAbstract*>(children[0]->getNode());
            if (children.size() > 1)
                material = static_cast<MaterialAbstract*>(children[1]->getNode());
        }
        else if (children.count() > 1)
        {
            material = static_cast<MaterialAbstract*>(children[0]->getNode() );
            shape = static_cast<ShapeAbstract*>(children[1]->getNode() );
        }

        if (shape)
        {
            double thit = 0.;
            DifferentialGeometry dg;
            if (!shape->intersect(rayLocal, &thit, &dg) ) return false;
            rayIn.tMax = thit;
            *modelNode = this;
            *isShapeFront = dg.isFront;

            if (material)
            {
                Ray ray;
                if (material->OutputRay(rayLocal, &dg, rand, &ray) )
                {
                    *rayOut = m_transformOtW(ray);
                    return true;
                }
            }
        }
    }
    return false;
}

void InstanceNode::extendBoxForLight(SbBox3f* extendedBox)
{
    SoGetBoundingBoxAction* action = new SoGetBoundingBoxAction(SbViewportRegion() );
    getNode()->getBoundingBox(action);

    SbBox3f box = action->getXfBoundingBox().project();
    delete action;
    extendedBox->extendBy(box);
}

/**
 * Set node world to object transform to \a nodeTransform .
 */
void InstanceNode::setTransform(Transform t)
{
    m_transformOtW = t;
    m_transformWtO = t.inversed();
}

void InstanceNode::updateTree(Transform parentOtW)
{
    SoBaseKit* nodeRoot = (SoBaseKit*) getNode();
    if (!nodeRoot) return;

    if (nodeRoot->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
    {
        SoTransform* nodeTransform = (SoTransform*) nodeRoot->getPart("transform", true);
        Transform transform = parentOtW*tgf::TransformFromSoTransform(nodeTransform);
        setTransform(transform);

        BoundingBox box;
        for (InstanceNode* child : children)
        {
            child->updateTree(transform);
            box.expand(child->getBox());
        }
        setBox(box);
    }
    else if (nodeRoot->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId()))
    {
        Transform transform = parentOtW;
        SoTransform* nodeTransform = (SoTransform*) nodeRoot->getPart("transform", false);
        if (nodeTransform)
            transform = transform*tgf::TransformFromSoTransform(nodeTransform);

        if (children.count() > 0)
        {
            InstanceNode* child = 0;
            if (children[0]->getNode()->getTypeId().isDerivedFrom(ShapeAbstract::getClassTypeId() ) )
                child = children[0];
            else if (children.count() > 1)
                child = children[1];

            if (child)
            {
                ShapeAbstract* shape = static_cast<ShapeAbstract*>(child->getNode() );
                BoundingBox box = transform(shape->getBox());
                setBox(box);
                setTransform(transform);
            }
        }
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

bool operator==(const InstanceNode& a, const InstanceNode& b)
{
    return a.getNode() == b.getNode() &&
        a.getParent()->getNode() == b.getParent()->getNode();
}
