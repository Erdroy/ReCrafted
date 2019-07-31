// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/Lock.h"
#include "Voxels/VoxelChunk.h"

class VoxelObjectOctree
{
    DELETE_COPY_MOVE(VoxelObjectOctree)
    friend class VoxelObjectBase;

public:
    enum class NodeDirection
    {
        Front = 0,
        Back,
        Left,
        Right,
        Top,
        Bottom
    };

    class Node
    {
        DELETE_COPY_MOVE(Node)
        friend class VoxelObjectOctree;
        friend class VoxelObjectManager;

    public:
        /// <summary>
        ///     The minimal node size.
        /// </summary>
        static const int MinimumNodeSize = 16;

        /// <summary>
        ///     The count of neighboring nodes of a single octree node.
        /// </summary>
        static const int NeighborNodeCount = 6;

        /// <summary>
        ///     The count of possible children nodes of a single octree node.
        /// </summary>
        static const int ChildrenNodeCount = 8;

    private:
        Node* m_parent = nullptr;
        Node* m_root = nullptr;
        
        Node* m_neighborNodes[NeighborNodeCount] = { nullptr };
        Node* m_childrenNodes[ChildrenNodeCount] = { nullptr };

        VoxelObjectOctree* m_owner = nullptr;

        VoxelChunk* m_chunk = nullptr;

        int m_id = 0;
        int m_size = 0;
        int m_depth = 0;
        bool m_isRoot = false;
        bool m_isPopulated = false;

        std::atomic_bool m_isProcessing = false;

        BoundingBoxD m_bounds = {};

    public:
        Node() = default;
        ~Node() = default;

    private:
        void DestroyChildren();
        void UpdateNeighborNodes();
        void DebugDraw();

    public:
        /// <summary>
        ///     Populates this node.
        /// </summary>
        void Populate();

        /// <summary>
        ///     Depopulates this node.
        /// </summary>
        void Depopulate();

        /// <summary>
        ///     Rebuilds this node. To rebuilt this node, 
        ///     it has to be an leaf node, i.e. cannot be a parent.
        /// </summary>
        void Rebuild();

        /// <summary>
        ///     Called on main thread after this node has been created 
        ///     by populating of the parent.
        /// </summary>
        void OnCreate();

        /// <summary>
        ///     Called on main thread, when parent is being depopulated.
        /// </summary>
        void OnDestroy();

        /// <summary>
        ///     Called on main thread, when this node has been populated.
        /// </summary>
        void OnPopulate();

        /// <summary>
        ///     Called on main thread, when this node has been depopulated.
        /// </summary>
        void OnDepopulate();

        /// <summary>
        ///     Called on main thread, when this node has been rebuilt (i.e. got new mesh).
        /// </summary>
        void OnRebuild();

    private:
        void WorkerPopulate(IVoxelMesher* mesher);
        void WorkerRebuild(IVoxelMesher* mesher);

    public:
        /// <summary>
        ///     Looks for intersecting nodes.
        /// </summary>
        /// <param name="nodes">The intersecting nodes output list.</param>
        /// <param name="box">The bounding box to check node intersection against.</param>
        /// <param name="targetNodeSize">The target node size. Set to 0 when any.</param>
        void FindIntersecting(List<Node*>& nodes, const BoundingBoxD& box, int targetNodeSize = 16);

        /// <summary>
        ///     Looks for a neighboring node in given direction.
        /// </summary>
        /// <param name="direction">The direction.</param>
        /// <returns>The neighbor node or null when not found.</returns>
        Node* FindNeighbor(NodeDirection direction);

        /// <summary>
        ///     Looks for a node at given position with given size.
        /// </summary>
        /// <param name="position">The position.</param>
        /// <param name="size">
        ///     The size. To return the node, the node at the given position 
        ///     will need to have equal size to the 'size' parameter.
        /// </param>
        /// <returns>The node or null when not found.</returns>
        Node* Find(const Vector3d& position, int size);

        /// <summary>
        ///     Looks for a node at given position.
        /// </summary>
        /// <param name="position">The position.</param>
        /// <returns>The node or null when not found.</returns>
        Node* Find(const Vector3d& position);
    };

private:
    VoxelObjectBase* m_owner = nullptr;
    Node** m_rootNodes = nullptr;
    int m_rootNodesCount = 0;

    int m_maxDepth = 0;

    std::atomic_bool m_isLoading = true;
    BoundingSphereD m_bounds = {};

private:
    void CreateRootNodes();
    void RebuildRootNodes() const;

public:
    explicit VoxelObjectOctree(VoxelObjectBase* owner);
    ~VoxelObjectOctree();
    
private:
    void Initialize();
    void Update();

    void DebugDraw();

public:
    /// <summary>
    ///     Looks for a node at given position with given size.
    /// </summary>
    /// <param name="position">The position.</param>
    /// <param name="size">
    ///     The size. To return the node, the node at the given position 
    ///     will need to have equal size to the 'size' parameter.
    /// </param>
    /// <returns>The node or null when not found.</returns>
    Node* FindNode(const Vector3d& position, int size) const;

    /// <summary>
    ///     Looks for a node at given position.
    /// </summary>
    /// <param name="position">The position.</param>
    /// <returns>The node or null when not found.</returns>
    Node* FindNode(const Vector3d& position) const;
};
