#include <iostream>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "libraries/geometry/BBox.h"
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
#include "trackers/TTrackerForAiming.h"


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
        for (int index = 0; index < children.size(); ++index)
        {
            InstanceNode* intersectedChild = 0;
            Ray childOutputRay;
            bool childShapeFront = true;
            bool isChildOutputRay = children[index]->Intersect(ray, rand, &childShapeFront, &intersectedChild, &childOutputRay);

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
        Ray childCoordinatesRay(m_transformWtO(ray));

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
            if (!tshape->intersect(childCoordinatesRay, &thit, &dg) ) return false;
            ray.tMax = thit;
            *modelNode = this;

            *isShapeFront = dg.isFront;

            if (tmaterial)
            {
                Ray surfaceOutputRay;
                if (tmaterial->OutputRay(childCoordinatesRay, &dg, rand, &surfaceOutputRay) )
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
void InstanceNode::setTransform(Transform nodeTransform)
{
    m_transformWtO = nodeTransform;
    m_transformOtW = m_transformWtO.GetInverse();
}

QDataStream& operator<< (QDataStream& s, const InstanceNode& node)
{
    s << node.getNode();
    return s;
}

QDataStream& operator>> (QDataStream& s, const InstanceNode& node)
{
    s >> node;
    return s;
}

bool operator==(const InstanceNode& a, const InstanceNode& b)
{
    return a.getNode() == b.getNode() &&
        a.getParent()->getNode() == b.getParent()->getNode();
}

/*
   template<class T> void InstanceNode::RecursivlyApply(void (T::*func)(void))
   {
        if ( GetNode()->getTypeId().isDerivedFrom( T::getClassTypeId() ) )
        {
           T * elem = static_cast< T* > ( GetNode() );
           (elem->*func)();
        }
        else
        {
      for( int index = 0; index < children.size(); ++index )
      {
         children[index]->RecursivlyApply<T>(func);
      }
        }
   }
   template<class T,class Param1> void InstanceNode::RecursivlyApply(void (T::*func)(Param1),Param1 param1)
   {
        if (GetNode()->getTypeId().isDerivedFrom( T::getClassTypeId() ) )
        {
           T * elem = static_cast< T* > ( GetNode() );
           (elem->*func)(param1);
        }
        else
        {
      for( int index = 0; index < children.size(); ++index )
      {
         children[index]->RecursivlyApply<T,Param1>(func,param1);
      }
        }
   }
   template<class T,class Param1> void InstanceNode::RecursivlyApplyWithMto(void (T::*func)(Param1),Param1 param1)
   {
        if (GetNode()->getTypeId().isDerivedFrom( T::getClassTypeId() ) )
        {
           T * elem = static_cast< T* > ( GetNode() );
           (elem->*func)(&m_transformWTO, param1);
        }
        else
        {
      for( int index = 0; index < children.size(); ++index )
      {
         children[index]->RecursivlyApplyWithMto<T,Param1>(func,param1);
      }
        }
   }
   template<class T,class Param1,class Param2> void InstanceNode::RecursivlyApplyWithMto(void (T::*func)(Param1,Param2),Param1 param1,Param1 param2)
   {
        if (GetNode()->getTypeId().isDerivedFrom( T::getClassTypeId() ) )
        {
           T * elem = static_cast< T* > ( GetNode() );
           (elem->*func)(&m_transformWTO, param1,param2);
        }
        else
        {
      for( int index = 0; index < children.size(); ++index )
      {
         children[index]->RecursivlyApplyWithMto<T,Param1,Param2>(func,param1,param2);
      }
        }
   }
 */
