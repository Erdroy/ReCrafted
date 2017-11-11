// ReCrafted © 2016-2017 Always Too Late

#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

// includes
#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"
#include "SpaceObjectOctree.h"

class SpaceObject
{
private:
	Ptr<SpaceObjectOctree> m_octree = nullptr;
	
public:
	void init();
	void update();
	void draw();
	void dispose();

public:
	// TODO: void updateViewPoints(Vector3* views, uint16_t viewCounts);
	
	/**
	 * \brief Updates the SpaceObject LoD view points using only one view point.
	 * Can be used on client-side.
	 * \param view The view position.
	 */
	void updateViewPoint(Vector3& view);

public:
	static Ptr<SpaceObject> createSpaceObject();

public:
	PROPERTY(float, radius) = 128.0f;
	PROPERTY(Vector3, position) = {};
};

#endif // SPACEOBJECT_H
