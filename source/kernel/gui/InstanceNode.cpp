#include <iostream>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "libraries/geometry/BBox.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/Ray.h"
#include "raytracing/DifferentialGeometry.h"
#include "InstanceNode.h"
#include "geometry/tgf.h"
#include "kernel/material/TMaterial.h"
#include "kernel/shape/TShape.h"
#include "raytracing/TShapeKit.h"
#include "raytracing/TLightKit.h"
#include "raytracing/TTracker.h"
#include "raytracing/TTrackerForAiming.h"


InstanceNode::InstanceNode(SoNode* node)
    : m_coinNode(node), m_parent(0)
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
    if (GetParent() ) url = GetParent()->GetNodeURL();
    url.append(QLatin1String("/") );
    const char* nodeName = m_coinNode->getName().getString();
    url.append(QLatin1String(nodeName) );
    return url;
}

void InstanceNode::Print(int level) const
{
    for (int i = 0; i < level; ++i) std::cout << " ";
    std::cout << m_coinNode->getTypeId().getName().getString()
              << " has " << children.size()
              << " children " << std::endl;
    for (int index = 0; index < children.count(); ++index)
        children[index]->Print(level++);
}

/**
 * Appends new \a child node to the end of the child list.
 **/
void InstanceNode::AddChild(InstanceNode* child)
{
    children.push_back(child);
    child->SetParent(this);
}
/**
 * Inserts the \a instanceChild node as child number \a row.
 **/
void InstanceNode::InsertChild(int row, InstanceNode* instanceChild)
{
    if (row > children.size() ) row = children.size();
    children.insert(row, instanceChild);
    instanceChild->SetParent(this);
}


//bool InstanceNode::Intersect( const Ray& ray, RandomDeviate& rand, InstanceNode** modelNode, Ray* outputRay )
bool InstanceNode::Intersect(const Ray& ray, RandomDeviate& rand, bool* isShapeFront, InstanceNode** modelNode, Ray* outputRay)
{
    //Check if the ray intersects with the BoundingBox
    if (!m_bbox.IntersectP(ray) ) return false;
    if (!GetNode()->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) ) // nodekit
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
        Ray childCoordinatesRay(m_transformWTO(ray));

        TShape* tshape = 0;
        TMaterial* tmaterial = 0;
        if (children[0]->GetNode()->getTypeId().isDerivedFrom(TShape::getClassTypeId() ) )
        {
            tshape = static_cast<TShape*>(children[0]->GetNode() );
            if (children.size() > 1) tmaterial = static_cast<TMaterial*> (children[1]->GetNode() );
        }
        else if (children.count() > 1)
        {
            tmaterial = static_cast<TMaterial*>(children[0]->GetNode() );
            tshape = static_cast<TShape*>(children[1]->GetNode() );
        }

        if (tshape)
        {
            double thit = 0.;
            DifferentialGeometry dg;
            if (!tshape->Intersect(childCoordinatesRay, &thit, &dg) ) return false;
            ray.tMax = thit;
            *modelNode = this;

            *isShapeFront = dg.shapeFrontSide;

            if (tmaterial)
            {
                Ray surfaceOutputRay;
                if (tmaterial->OutputRay(childCoordinatesRay, &dg, rand, &surfaceOutputRay) )
                {
                    *outputRay = m_transformOTW(surfaceOutputRay);
                    return true;
                }
            }
        }
    }
    return false;
}

void InstanceNode::DisconnectAllTrackers()
{
    //RecursivlyApply<TTracker>(&TTracker::Disconnect);
}

void InstanceNode::ReconnectAllTrackers(TLightKit* /*coinLight*/)
{
    //RecursivlyApply<TTracker,TLightKit *>(&TTracker::SetLightAngles,coinLight);
}

void InstanceNode::SetAimingPointRelativity(bool /*relative*/)
{
    //RecursivlyApply<TTrackerForAiming,bool>(&TTrackerForAiming::SetAimingPointRelativity, relative);
}

void InstanceNode::extendBoxForLight(SbBox3f* extendedBox)
{
    SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction(SbViewportRegion() );
    GetNode()->getBoundingBox(bbAction);

    SbBox3f box = bbAction->getXfBoundingBox().project();
    delete bbAction;
    extendedBox->extendBy(box);
}

BBox InstanceNode::GetIntersectionBBox()
{
    return m_bbox;
}

Transform InstanceNode::GetIntersectionTransform()
{
    return m_transformWTO;
}

void InstanceNode::SetIntersectionBBox(BBox nodeBBox)
{
    m_bbox = nodeBBox;
}
/**
 * Set node world to object transform to \a nodeTransform .
 */
void InstanceNode::SetIntersectionTransform(Transform nodeTransform)
{
    m_transformWTO = nodeTransform;
    m_transformOTW = m_transformWTO.GetInverse();
}

QDataStream& operator<< (QDataStream& s, const InstanceNode& node)
{
    s << node.GetNode();
    return s;
}

QDataStream& operator>> (QDataStream& s, const InstanceNode& node)
{
    s >> node;
    return s;
}

bool operator==(const InstanceNode& thisNode,const InstanceNode& otherNode)
{
    return ( (thisNode.GetNode() == otherNode.GetNode()) &&
             (thisNode.GetParent()->GetNode() == otherNode.GetParent()->GetNode()) );
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
