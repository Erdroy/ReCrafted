// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTOCTREE_H
#define SPACEOBJECTOCTREE_H

// includes
#include "SpaceObjectOctreeNode.h"

#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Containers/Array.h"

class SpaceObject;

/**
* \brief Octree structure of SpaceObject.
*/
class SpaceObjectOctree
{
    friend class SpaceObject;
    friend class SpaceObjectManager;
    friend class SpaceObjectOctreeNode;

private:
    SpaceObject* spaceObject = nullptr;

    int m_rootNodesCount = 1;
    SpaceObjectOctreeNode** m_rootNodes = nullptr;

public:
    void Update();
    void UpdateViews(Array<Vector3>& views);
    void Draw();
    void Dispose();

public:
    void GeneratePrimary();

public:
    SpaceObjectOctreeNode* FindNode(const Vector3& position, int size) const;
    SpaceObjectOctreeNode* FindNode(const Vector3& position) const;
    Array<SpaceObjectOctreeNode*> FindIntersecting(const BoundingBox& box, bool leafOnly) const;

public:
PROPERTY(int, MaxDepth) = 0;
PROPERTY(BoundingBox, Bounds) = {};
};

#endif // SPACEOBJECTOCTREE_H
