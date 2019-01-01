// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef VOXELUTILS_H
#define VOXELUTILS_H

#define INDEX_3D(x, y, z, size) ((x) * (size) * (size) + (y) * (size) + (z))

#define VOXEL_FROM_FLOAT(x) static_cast<sbyte>(Math::Clamp(x * 127.0f, -127.0f, 127.0f))
#define VOXEL_TO_FLOAT(x) (x / 127.0f)

#endif // VOXELUTILS_H
