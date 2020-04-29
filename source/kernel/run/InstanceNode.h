#pragma once

#include "kernel/TonatiuhKernel.h"

#include <vector>

#include <QVector>
#include <QMutex>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbMatrix.h>

#include "libraries/geometry/BBox.h"
#include "libraries/geometry/Transform.h"

class RandomAbstract;
class Ray;
class SoNode;
class TLightKit;
class SceneModel;


//!  InstanceNode class represents a instance of a node in the scene.
/*! In a scene, a node can be shared by more that one parent. Each of these shared instances is represented in a scene as a InstanceNode object.
 * Any change made whitin a shared node is reflected in all node's InstanceNode.
 */

class TONATIUH_KERNEL InstanceNode
{
public:
    InstanceNode(SoNode* node);
    ~InstanceNode();

    void SetNode(SoNode* node)
    {
        m_coinNode = node;
    }
    void SetParent(InstanceNode* parent)
    {
        m_parent = parent;
    }
    void AddChild(InstanceNode* child);
    void InsertChild(int row, InstanceNode* instanceChild);

    SoNode* GetNode() const
    {
        return m_coinNode;
    }
    InstanceNode* GetParent() const
    {
        return m_parent;
    }
    QString GetNodeURL() const;
    void Print(int level) const;

    bool Intersect(const Ray& ray, RandomAbstract& rand, bool* isShapeFront, InstanceNode** modelNode, Ray* outputRay);

    //template<class T> void RecursivlyApply(void (T::*func)(void));
    //template<class T,class Param1> void RecursivlyApply(void (T::*func)(Param1),Param1 param1);
    //template<class T,class Param1> void RecursivlyApplyWithMto(void (T::*func)(Param1),Param1 param1);
    //template<class T,class Param1,class Param2> void RecursivlyApplyWithMto(void (T::*func)(Param1,Param2),Param1 param1,Param1 param2);

    void DisconnectAllTrackers();
    void ReconnectAllTrackers(TLightKit* coinLight);
    void SetAimingPointRelativity(bool relative);
    void extendBoxForLight(SbBox3f* extendedBox);
    BBox GetIntersectionBBox();
    Transform GetIntersectionTransform();
    void SetIntersectionBBox(BBox nodeBBox);
    void SetIntersectionTransform(Transform nodeTransform);

    QVector<InstanceNode*> children;

private:
    SoNode* m_coinNode;
    InstanceNode* m_parent;
    BBox m_bbox;
    Transform m_transformWTO;
    Transform m_transformOTW;
};

TONATIUH_KERNEL QDataStream& operator<<(QDataStream& s, const InstanceNode& node);
TONATIUH_KERNEL QDataStream& operator>>(QDataStream& s, const InstanceNode& node);
TONATIUH_KERNEL bool operator==(const InstanceNode& thisNode,const InstanceNode& otherNode);
