#pragma once

#include "kernel/TonatiuhKernel.h"

#include <vector>

#include <QVector>
#include <QMutex>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbMatrix.h>

#include "libraries/geometry/BoundingBox.h"
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

    void setNode(SoNode* node) {m_node = node;}
    SoNode* getNode() const {return m_node;}

    InstanceNode* getParent() const {return m_parent;}
    void setParent(InstanceNode* parent) {m_parent = parent;}

    void addChild(InstanceNode* child);
    void InsertChild(int row, InstanceNode* instanceChild);

    QString GetNodeURL() const;
    void Print(int level) const;

    bool Intersect(const Ray& ray, RandomAbstract& rand, bool* isShapeFront, InstanceNode** modelNode, Ray* outputRay);

    //template<class T> void RecursivlyApply(void (T::*func)(void));
    //template<class T,class Param1> void RecursivlyApply(void (T::*func)(Param1),Param1 param1);
    //template<class T,class Param1> void RecursivlyApplyWithMto(void (T::*func)(Param1),Param1 param1);
    //template<class T,class Param1,class Param2> void RecursivlyApplyWithMto(void (T::*func)(Param1,Param2),Param1 param1,Param1 param2);

    void extendBoxForLight(SbBox3f* extendedBox);

    const BoundingBox& getBox() const {return m_box;}
    void setBox(const BoundingBox& box) {m_box = box;}

    Transform getTransform() {return m_transformWtO;}
    void setTransform(Transform nodeTransform);

    QVector<InstanceNode*> children;

    void updateTree(Transform parentOtW);

private:
    SoNode* m_node;
    InstanceNode* m_parent;
    BoundingBox m_box; // world
    Transform m_transformWtO;
    Transform m_transformOtW;
};

TONATIUH_KERNEL QDataStream& operator<<(QDataStream& s, const InstanceNode& node);
TONATIUH_KERNEL QDataStream& operator>>(QDataStream& s, const InstanceNode& node);
TONATIUH_KERNEL bool operator==(const InstanceNode& thisNode,const InstanceNode& otherNode);
