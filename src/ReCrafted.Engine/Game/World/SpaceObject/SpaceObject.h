// ReCrafted © 2016-2017 Always Too Late

#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

// includes
#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectSettings.h"

class VoxelStorage;

class SpaceObject
{
private:
    SpaceObjectSettings m_settings = {};

    Array<Vector3> m_views = {};

	Ptr<SpaceObjectOctree> m_octree = nullptr;
    Ptr<VoxelStorage> m_voxelStorage = nullptr;
	
private:
    void init(SpaceObjectSettings& settings);

public:
	void update();
	void draw();
	void dispose();

public:
	/**
	 * \brief Updates the SpaceObject LoD view points using this view point for the next frame.
	 * Can be used on client-side. To use this, call this in early update, before the octree updates.
	 * \param view The view position.
	 */
	void updateViewPoint(Vector3& view);

    /**
	 * \brief Gets space object settings reference.
	 * \return Space object settings reference.
	 */
	SpaceObjectSettings& getSettings();

public:
    /**
	 * \brief Creates space object from settings.
	 * \param settings The space object settings.
	 * \return The new created space object.
	 */
	static Ptr<SpaceObject> createSpaceObject(SpaceObjectSettings& settings);

public:
	PROPERTY(Vector3, position) = {};
};

#endif // SPACEOBJECT_H
