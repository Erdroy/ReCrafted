#pragma once

#ifndef MESHINGHELPERS_H
#define MESHINGHELPERS_H

// includes
#include "Core/Math/Math.h"

/**
* \brief Calculates surface intersection on edge based on two data samples.
* \param positionA The first point.
* \param positionB The second point.
* \param densityA The first point value.
* \param densityB The second point value.
* \return The intersection of the given points.
*/
inline Vector3 GetIntersection(Vector3& positionA, Vector3& positionB, const float densityA, const float densityB)
{
    return positionA + (positionB - positionA) * -densityA  / (densityB - densityA);
}

#endif // MESHINGHELPERS_H
