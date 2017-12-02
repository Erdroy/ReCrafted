// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SPACEOBJECTTABLES_H
#define SPACEOBJECTTABLES_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Vector3.h"

byte LocalNeighTable[];
byte NeighDirTable[][3];
byte DirIndex[];
byte NodeDirIds[];
Vector3 DirectionOffset[];
Vector3 ChildrenNodeOffsets[];

#endif // SPACEOBJECTTABLES_H
