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
 * Returns node URL.
 */
QString InstanceNode::GetNodeURL() const
{
    QString url;
    if (getParent()) url = getParent()->GetNodeURL();
    url.append(QLatin1String("/") );
    const char* nodeName = m_node->getName().getString();
    url.append(QLatin1String(nodeName) );
    return url;
}

void InstanceNode::Print(int level) const
{
    for (int i = 0; i < level; ++i) std::cout << " ";
    std::cout << m_node->getTypeId().getName().getString()
              << " has " << children.size()
              << " children " << std::endl;
    for (int index = 0; index < children.count(); ++index)
        children[index]->Print(level++);
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
void InstanceNode::InsertChild(int row, InstanceNode* instanceChild)
{
    if (row > children.size() ) row = children.size();
    children.insert(row, instanceChild);
    instanceChild->setParent(this);
}

//bool InstanceNode::Intersect( const Ray& ray, RandomDeviate& rand, InstanceNode** modelNode, Ray* outputRay )
bool InstanceNode::Intersect(const Ray& ray, RandomAbstract& rand, bool* isShapeFront, InstanceNode** modelNode, Ray* outputRay)
{
    //Check if the ray intersects with the BoundingBox
    if (!m_box.IntersectP(ray) ) return false;
    if (!getNode()->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) ) // nodekit
    {
        bool isOutputRay = false;
        double t = ray.tMax;
        for (int n = 0; n < children.size(); ++n)
        {
            InstanceNode* intersectedChild = 0;
            Ray childOutputRay;
            bool childShapeFront = true;
            bool isChildOutputRay = children[n]->Intersect(ray, rand, &childShapeFront, &intersectedChild, &childOutputRay);

            if (ray.tMax < t) // tMax mutable
            {
                t = ray.tMax;
                *modelNode = intersectedChild;
                *isShapeFront = childShapeFront;

                *outputRay = childOutputRay;
                isOutputRay = isChildOutputRay;
            }
        }
        return isOutputRay;
    }
    else // shapekit
    {
        Ray rayLocal(m_transformWtO(ray));

        ShapeAbstract* tshape = 0;
        MaterialAbstract* tmaterial = 0;
        if (children[0]->getNode()->getTypeId().isDerivedFrom(ShapeAbstract::getClassTypeId() ) )
        {
            tshape = static_cast<ShapeAbstract*>(children[0]->getNode() );
            if (children.size() > 1) tmaterial = static_cast<MaterialAbstract*> (children[1]->getNode() );
        }
        else if (children.count() > 1)
        {
            tmaterial = static_cast<MaterialAbstract*>(children[0]->getNode() );
            tshape = static_cast<ShapeAbstract*>(children[1]->getNode() );
        }

        if (tshape)
        {
            double thit = 0.;
            DifferentialGeometry dg;
            if (!tshape->intersect(rayLocal, &thit, &dg) ) return false;
            ray.tMax = thit;
            *modelNode = this;

            *isShapeFront = dg.isFront;

            if (tmaterial)
            {
                Ray surfaceOutputRay;
                if (tmaterial->OutputRay(rayLocal, &dg, rand, &surfaceOutputRay) )
                {
                    *outputRay = m_transformOtW(surfaceOutputRay);
                    return true;
                }
            }
        }
    }
    return false;
}

void InstanceNode::extendBoxForLight(SbBox3f* extendedBox)
{
    SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction(SbViewportRegion() );
    getNode()->getBoundingBox(bbAction);

    SbBox3f box = bbAction->getXfBoundingBox().project();
    delete bbAction;
    extendedBox->extendBy(box);
}

/**
 * Set node world to object transform to \a nodeTransform .
 */
void InstanceNode::setTransform(Transform t)
{
    m_transformOtW = t;
    m_transformWtO = t.GetInverse();
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
