// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELCHM_H
#define VOXELCHM_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Graphics/Bitmap.h"

class VoxelCHM : IDisposable
{
private:
    byte* m_faces[6] = {};

    int m_bitmapWidth = 0;
    int m_bitmapHeight = 0;

private:
    VoxelCHM() {}

private:
    void loadFace(int face, const char* name, const char* directoryName);

    float sampleFace(const int face, const Vector2 texcoord) const
    {
#ifdef DEBUG
        if (texcoord.x < 0.0f || texcoord.y < 0.0f || texcoord.x >= 1.0f || texcoord.y >= 1.0f)
        {
            throw;
        }
#endif

        cvar bitmap = m_faces[face];

        cvar posX = static_cast<int>(texcoord.x * m_bitmapWidth);
        cvar posY = static_cast<int>(texcoord.y * m_bitmapHeight);

        cvar pixel = bitmap[posY * m_bitmapWidth + posX];

        return pixel / 255.0f;
    }

public:
    FORCEINLINE float sample(const Vector3& point, const float radius, const int lod) const
    {
        if (point.length() == 0)
            return 0.0f;

        cvar spherePoint = mapSphere(point, radius);
        cvar sphereFace = getFace(point);
        cvar texcoord = getTexcoord(sphereFace, spherePoint);

        return sampleFace(sphereFace, texcoord); // TODO: sample proper LOD level
    }

    void dispose() override
    {
        // free all 6 faces
        for(var i = 0; i < 6; i ++)
            Bitmap::free(m_faces[i]);
    }

public:
    FORCEINLINE static Vector2 getTexcoord(const int face, const Vector3& point)
    {
        Vector2 texcoord;
        
        switch(face)
        {
        case 0:
        {
            cvar localPoint = point * (1.0f / fabs(point.x));
            texcoord.y = localPoint.y;
            texcoord.x = -localPoint.z;
            break;
        }
        case 1:
        {
            cvar localPoint = point * (1.0f / fabs(point.x));
            texcoord.y = localPoint.y;
            texcoord.x = localPoint.z;
            break;
        }
        case 2:
        {
            cvar localPoint = point * (1.0f / fabs(point.y));
            texcoord.y = localPoint.z;
            texcoord.x = localPoint.x;
            break;
        }
        case 3:
        {
            cvar localPoint = point * (1.0f / fabs(point.y));
            texcoord.y = localPoint.z;
            texcoord.x = -localPoint.x;
            break;
        }
        case 4:
        {
            cvar localPoint = point * (1.0f / fabs(point.z));
            texcoord.y = localPoint.y;
            texcoord.x = -localPoint.x;
            break;
        }
        case 5:
        {
            cvar localPoint = point * (1.0f / fabs(point.z));
            texcoord.y = localPoint.y;
            texcoord.x = localPoint.x;
            break;
        }
        default: throw;
        }

        // clamp
        texcoord.x = (texcoord.x + 1.0f) * 0.5f;
        texcoord.y = (texcoord.y + 1.0f) * 0.5f;

        if (texcoord.x >= 1.0f)
            texcoord.x = 0.999999f;

        if (texcoord.y >= 1.0f)
            texcoord.y = 0.999999f;

        return texcoord;
    }

    /**
     * \brief Selects the sphere face is used by the point.
     * \param point The point which will be check.
     * \return The result.
     */
    FORCEINLINE static int getFace(const Vector3& point)
    {
        cvar absPoint = Vector3::abs(const_cast<Vector3&>(point));

        if(absPoint.x > absPoint.y)
        {
            if(absPoint.x > absPoint.z)
            {
                if (point.x > 0.0f)
                {
                    return 1; // right
                }
                return 0; // left
            }

            if (point.z > 0.0f)
            {
                return 4; // forward
            }
            return 5; // back
        }

        if (absPoint.y > absPoint.z)
        {
            if (point.y > 0.0f)
            {
                return 2; // up
            }

            return 3; // down
        }

        if (point.z > 0.0f)
        {
            return 4; // front
        }
        return 5; // back
    }

    /**
     * \brief Transforms given point (cube-space not world space!) to a sphere.
     * \param point This point will be transformed into sphere-space coord.
     * \param radius The radius of the sphere.
     * \return The transformed point to the sphere.
     */
    FORCEINLINE static Vector3 mapSphere(Vector3 point, const float radius)
    {
        point.normalize();
        return point * radius;
    }

    /**
     * \brief Loads CHM from given directory.
     * \param directoryName The directory name which contains the PNG files (left, back etc.).
     * \return The created VoxelCHM.
     */
    static Ptr<VoxelCHM> loadFromDirectory(const char* directoryName);
};

#endif // VOXELCHM_H
