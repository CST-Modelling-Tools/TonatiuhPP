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
/*! In a scene, a node can be shared by more than one parent. Each of these shared instances is represented in a scene as a InstanceNode object.
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
    void insertChild(int row, InstanceNode* child);

    QString GetNodeURL() const;
    void Print(int level) const;

    bool intersect(const Ray& rayIn, RandomAbstract& rand, bool* isShapeFront, InstanceNode** modelNode, Ray* rayOut);

    void extendBoxForLight(SbBox3f* extendedBox);

    const BoundingBox& getBox() const {return m_box;}
    void setBox(const BoundingBox& box) {m_box = box;}

    Transform getTransform() {return m_transformWtO;}
    void setTransform(Transform t);

    QVector<InstanceNode*> children;

    void updateTree(Transform parentOtW);

private:
    SoNode* m_node;
    InstanceNode* m_parent;
    BoundingBox m_box; // world
    Transform m_transformWtO;
    Transform m_transformOtW;
};

TONATIUH_KERNEL bool operator==(const InstanceNode& thisNode,const InstanceNode& otherNode);

TONATIUH_KERNEL QDataStream& operator<<(QDataStream& s, const InstanceNode& node);
TONATIUH_KERNEL QDataStream& operator>>(QDataStream& s, const InstanceNode& node);
