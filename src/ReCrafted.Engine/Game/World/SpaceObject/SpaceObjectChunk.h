// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SPACEOBJECTCHUNK_H
#define SPACEOBJECTCHUNK_H

// includes
#include "Core/Math/Vector3.h"

class SpaceObject;
class SpaceObjectOctreeNode;

class SpaceObjectChunk
{
private:
	SpaceObject* spaceObject = nullptr;
	SpaceObjectOctreeNode* node = nullptr;

	Vector3 m_objectNormal = {};

public:
	void init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject);
	void generate();
	void update();
	void draw();
	void dispose();
};

#endif // SPACEOBJECTCHUNK_H
