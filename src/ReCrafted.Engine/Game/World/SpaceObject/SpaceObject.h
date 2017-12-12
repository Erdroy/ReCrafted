// ReCrafted © 2016-2017 Always Too Late

#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

// includes
#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectSettings.h"

class SpaceObject
{
private:
	Ptr<SpaceObjectOctree> m_octree = nullptr;
	Array<Vector3> m_views = {};
	SpaceObjectSettings m_settings = {};
	
public:
	void init(SpaceObjectSettings& settings);
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

	SpaceObjectSettings& getSettings();

public:
	static Ptr<SpaceObject> createSpaceObject();

public:
	PROPERTY(Vector3, position) = {};
};

#endif // SPACEOBJECT_H
