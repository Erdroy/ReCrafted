// ReCrafted (c) 2016-2018 Always Too Late

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

float LoDTable[];

#endif // SPACEOBJECTTABLES_H
