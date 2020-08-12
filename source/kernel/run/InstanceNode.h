#pragma once

#include "kernel/TonatiuhKernel.h"

#include <vector>

#include <QVector>
#include <QMutex>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbMatrix.h>

#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Transform.h"

class Random;
class Ray;
class SoNode;
class TShapeKit;
class SceneModel;


//!  InstanceNode class represents a instance of a node in the scene.
/*! In a scene, a node can be shared by more than one parent. Each of these shared instances is represented in a scene as a InstanceNode object.
 * Any change made whitin a shared node is reflected in all node's InstanceNode.
 */

class TONATIUH_KERNEL InstanceNode
{
public:
    enum IndicesTShapeKit {
        IndexShapeRT = 0,
        IndexProfileRT,
        IndexMaterialRT,
        IndexMaterial
    };

    InstanceNode(SoNode* node);
    ~InstanceNode();

    SoNode* getNode() const {return m_node;}
    void setNode(SoNode* node) {m_node = node;}

    InstanceNode* getParent() const {return m_parent;}
    void setParent(InstanceNode* parent) {m_parent = parent;}

    const Box3D& getBox() const {return m_box;}
    void setBox(const Box3D& box) {m_box = box;}

    const Transform& getTransform() {return m_transform;}
    void setTransform(const Transform& t) {m_transform = t;}

    void addChild(InstanceNode* child);
    void insertChild(int row, InstanceNode* child);
    void replaceChild(int row, InstanceNode* child);

    bool operator==(const InstanceNode& other);
    QString getURL() const;
    void Print(int level) const;

    bool intersect(const Ray& rayIn, Random& rand, bool& isFront, InstanceNode*& instance, Ray& rayOut);

    void extendBoxForLight(SbBox3f* extendedBox);

    void updateTree(const Transform& tParent);
    void collectShapeTransforms(QStringList disabledNodes, QVector<QPair<TShapeKit*, Transform> >& shapes);

    QVector<InstanceNode*> children;

private:
    SoNode* m_node;
    InstanceNode* m_parent;
    Box3D m_box; // in world frame
    Transform m_transform; // from object to world
};


TONATIUH_KERNEL QDataStream& operator<<(QDataStream& s, const InstanceNode& node);
TONATIUH_KERNEL QDataStream& operator>>(QDataStream& s, const InstanceNode& node);
