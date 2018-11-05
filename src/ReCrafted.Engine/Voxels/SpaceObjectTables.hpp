// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTTABLES_H
#define SPACEOBJECTTABLES_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Color.h"

byte LocalNeighTable[];
byte NeighDirTable[][3];
byte NodeDirIds[];
Vector3 DirectionOffset[];
Vector3 ChildrenNodeOffsets[];

Color NodeLoDDebugColors[];

int LoDTable[];

#endif // SPACEOBJECTTABLES_H
