// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTCHUNK_H
#define SPACEOBJECTCHUNK_H

// includes
#include "ReCrafted.h"
#include "Core/Lock.h"
#include "Graphics/Rendering/RenderableBase.h"

#include "Storage/VoxelChunkData.h"
#include "SpaceObject.h"
#include "VoxelChunkMesh.h"
#include "VoxelChunkCollision.h"

#include <atomic>
#include <concurrentqueue.h>

struct IVoxelMesher;
class Mesh;
class SpaceObject;
class SpaceObjectOctreeNode;

class SpaceObjectChunk : public RenderableBase
{
    friend class SpaceObjectManager;

private:
    typedef enum
    {
        None,
        SwapMesh,
        UploadMesh,
        ClearMesh
    } UploadType;

private:
    SpaceObject* spaceObject = nullptr;
    SpaceObjectOctreeNode* owner = nullptr;

    int m_lod = 0;
    uint64_t m_id = 0u;
    Vector3 m_position = {};

    RefPtr<VoxelChunkData> m_chunkData = nullptr;

    RefPtr<VoxelChunkMesh> m_mesh = nullptr;
    RefPtr<VoxelChunkMesh> m_newMesh = nullptr;

    RefPtr<VoxelChunkCollsion> m_collision = nullptr;
    RefPtr<VoxelChunkCollsion> m_newCollision = nullptr;

    moodycamel::ConcurrentQueue<std::pair<RefPtr<VoxelChunkMesh>, RefPtr<VoxelChunkCollsion>>> m_disposeQueue;

    Lock m_uploadLock = {};
    std::atomic<UploadType> m_uploadType = None;

    IPhysicsActor* m_physicsActor = nullptr;

private:
    void SetUpload(const RefPtr<VoxelChunkMesh>& mesh, const RefPtr<VoxelChunkCollsion>& collision, UploadType uploadType);

public:
    void Init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject);
    void Upload();
    void Dispose();

public:
    /**
     * \brief Called by owner node when this chunk should be attached for render.
     */
    void OnRenderAttach();

    /**
     * \brief Called by owner node when this chunk should be detached from render.
     */
    void OnRenderDetach();

public: /* Worker */
    void Generate(IVoxelMesher* mesher);
    void Rebuild(IVoxelMesher* mesher);

public: /* Physics */
    void InitializePhysics();
    void ShutdownPhysics();

    void AttachCollision();
    void DetachCollision();

    /*bool HasCollision() const
    {
        return !m_physicsShapes.Empty();
    }*/

public: /* RenderableBase */
    void Render(RenderableRenderMode renderMode) override;

    Shader* GetShader() const override
    {
        return spaceObject->m_terrainShader;
    }

    Vector3& GetPosition() override
    {
        return m_position;
    }

    BoundingBox& GetBounds() override
    {
        return owner->GetBounds();
    }

    RenderableRenderMode GetRenderMode() override
    {
        return RenderableRenderMode::RenderGeometry /*| RenderableRenderMode::RenderShadows*/;
    }

public:
    bool NeedsUpload() const
    {
        return m_uploadType != None;
    }

    RefPtr<VoxelChunkData> GetChunkData() const
    {
        return m_chunkData;
    }

    bool HasMesh() const
    {
        return m_mesh != nullptr && m_mesh->IsValid();
    }

    bool HasCollision() const
    {
        return m_collision != nullptr && m_collision->IsValid();
    }

public:
    static uint64_t CalculateChunkId(const Vector3& position);
};

#endif // SPACEOBJECTCHUNK_H

