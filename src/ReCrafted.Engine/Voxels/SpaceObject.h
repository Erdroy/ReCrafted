// ReCrafted (c) 2016-2018 Always Too Late

#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

// includes
#include "Core/Math/Vector3.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectSettings.h"
#include "Storage/VoxelEditMode.h"

class VoxelStorage;

class SpaceObject
{
    friend class VoxelStorage;

private:
    SpaceObjectSettings m_settings = {};

    Array<Vector3> m_views = {};

    Ref<SpaceObjectOctree> m_octree = nullptr;
    Ref<VoxelStorage> m_voxelStorage = nullptr;

private:
    void Init(SpaceObjectSettings& settings);

public:
    void Update();
    void Draw();
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
     * \param mode The edit mode.
     * \param position The edit position.
     * \param size The edit size.
     */
    void Modify(VoxelEditMode::_enum mode, Vector3& position, float size);

    /**
	 * \brief Gets space object settings reference.
	 * \return Space object settings reference.
	 */
    SpaceObjectSettings& GetSettings();

    /**
     * \brief Gets the VoxelStorage instance of this SpaceObject.
     * \return VoxelStorage instance of this SpaceObject.
     */
    VoxelStorage* GetStorage() const
    {
        return m_voxelStorage.get();
    }

public:
    /**
	 * \brief Creates space object from settings.
	 * \param settings The space object settings.
	 * \return The new created space object.
	 */
    static Ref<SpaceObject> CreateSpaceObject(SpaceObjectSettings& settings);

public:
PROPERTY(Vector3, Position) = {};
};

#endif // SPACEOBJECT_H
