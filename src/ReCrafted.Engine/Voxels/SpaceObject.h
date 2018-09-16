// ReCrafted (c) 2016-2018 Always Too Late

#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

// includes
#include "Core/Math/Vector3.h"
#include "SpaceObject.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectSettings.h"
#include "Storage/VoxelEditMode.h"
#include "Graphics/Shader.h"

class SpaceObject
{
    friend class VoxelStorage;
    friend class SpaceObjectChunk;
    friend class VoxelGenerator;

public:
    static SpaceObject* current;
    Array<RefPtr<Texture2D>> m_textures = {};

private:
    SpaceObjectSettings m_settings = {};

    Array<Vector3> m_views = {};

    RefPtr<SpaceObjectOctree> m_octree = nullptr;
    RefPtr<VoxelGenerator> m_generator = nullptr;
    RefPtr<VoxelStorage> m_voxelStorage = nullptr;

    RefPtr<Shader> m_terrainShader = nullptr;

    std::string m_debugName;
    std::string m_updateProfileName;

private:
    void Init(SpaceObjectSettings& settings);

public:
    void GeneratePrimary();
    void Update();
    void Dispose();

public:
    /**
     * \brief Updates the SpaceObject LoD view points using this view point for the next frame.
     * Can be used on client-side. To use this, call this in early Update, before the octree updates.
     * \param view The view position.
     */
    void UpdateViewPoint(Vector3& view);

    /**
     * \brief Modifies this voxel space object, using given method.
     * \param material The voxel material that will be applied on every voxel.
     * \param mode The edit mode.
     * \param position The edit position.
     * \param size The edit size.
     */
    void Modify(VoxelMaterial_t material, VoxelEditMode::_enum mode, const Vector3& position, float size);

    /**
	 * \brief Gets space object settings reference.
	 * \return Space object settings reference.
	 */
    SpaceObjectSettings& GetSettings()
    {
        return m_settings;
    }

    /**
    * \brief Gets the SpaceObjectOctree instance of this SpaceObject.
    * \return SpaceObjectOctree instance owned by this SpaceObject.
    */
    SpaceObjectOctree* GetOctree() const
    {
        return m_octree.get();
    }

    /**
     * \brief Gets the VoxelStorage instance of this SpaceObject.
     * \return VoxelStorage instance owned by this SpaceObject.
     */
    VoxelStorage* GetStorage() const
    {
        return m_voxelStorage.get();
    }

    /**
     * \brief Gets the VoxelGenerator instance of this SpaceObject.
     * \return VoxelGenerator instance owned by this SpaceObject.
     */
    VoxelGenerator* GetGenerator() const
    {
        return m_generator.get();
    }

public:
    /**
	 * \brief Creates space object from settings.
	 * \param settings The space object settings.
	 * \return The new created space object.
	 */
    static RefPtr<SpaceObject> CreateSpaceObject(SpaceObjectSettings& settings);

public:
PROPERTY(Vector3, Position) = {};
};

#endif // SPACEOBJECT_H
