// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef CHMHELPERS_H
#define CHMHELPERS_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Math.h"

class CHMHelpers
{
public:
    /**
     * \brief Maps given point to CHM UV coordinates in ranges [0.0 incl, 1.0 excl].
     * \param face The face which will get mapped point.
     * \param point The point which will be mapped.
     * \return The UV coordinates in ranges [0.0 incl, 1.0 excl].
     */
    FORCEINLINE static Vector2 GetTexcoord(const int face, const Vector3& point)
    {
        Vector2 texcoord;

        switch (face)
        {
        case 0:
        {
            cvar localPoint = point * (1.0f / fabs(point.z));
            texcoord.y = localPoint.y;
            texcoord.x = -localPoint.x;
            break;
        }
        case 1:
        {
            cvar localPoint = point * (1.0f / fabs(point.z));
            texcoord.y = localPoint.y;
            texcoord.x = localPoint.x;
            break;
        }
        case 2:
        {
            cvar localPoint = point * (1.0f / fabs(point.x));
            texcoord.y = localPoint.y;
            texcoord.x = -localPoint.z;
            break;
        }
        case 3:
        {
            cvar localPoint = point * (1.0f / fabs(point.x));
            texcoord.y = localPoint.y;
            texcoord.x = localPoint.z;
            break;
        }
        case 4:
        {
            cvar localPoint = point * (1.0f / fabs(point.y));
            texcoord.y = localPoint.z;
            texcoord.x = localPoint.x;
            break;
        }
        case 5:
        {
            cvar localPoint = point * (1.0f / fabs(point.y));
            texcoord.y = -localPoint.z;
            texcoord.x = localPoint.x;
            break;
        }
        default: throw;
        }

        // clamp
        texcoord.x = (texcoord.x + 1.0f) * 0.5f;
        texcoord.y = 1.0f - (texcoord.y + 1.0f) * 0.5f;

        if (texcoord.x >= 1.0f)
            texcoord.x = 0.99999f;

        if (texcoord.y >= 1.0f)
            texcoord.y = 0.99999f;

        return texcoord;
    }

    /**
    * \brief Selects the sphere face is used by the point.
    * \param point The point which will be check.
    * \return The selected face.
    */
    FORCEINLINE static int GetFace(const Vector3& point)
    {
        cvar absPoint = Vector3::Abs(const_cast<Vector3&>(point));

        if (absPoint.x > absPoint.y)
        {
            if (absPoint.x > absPoint.z)
            {
                if (point.x > 0.0f)
                {
                    return 3; // right
                }
                return 2; // left
            }

            if (point.z > 0.0f)
            {
                return 0; // front
            }
            return 1; // back
        }

        if (absPoint.y > absPoint.z)
        {
            if (point.y > 0.0f)
            {
                return 4; // up
            }

            return 5; // down
        }

        if (point.z > 0.0f)
        {
            return 0; // front
        }
        return 1; // back
    }
};

#endif // CHMHELPERS_H
