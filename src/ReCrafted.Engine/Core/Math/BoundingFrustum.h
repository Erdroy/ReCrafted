// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef BOUNDINGFRUSTUM_H
#define BOUNDINGFRUSTUM_H

// includes
#include "Plane.h"
#include "BoundingBox.h"

struct BoundingFrustum
{
private:
    static void getBoxToPlanePVertexNVertex(BoundingBox& box, Vector3& planeNormal, Vector3* p, Vector3* n)
    {
        cvar bmin = box.minimum();
        cvar bmax = box.maximum();

        *p = bmin;
        *n = bmax;

        if (planeNormal.x >= 0)
        {
            p->x = bmax.x;
            n->x = bmin.x;
        }
        if (planeNormal.y >= 0)
        {
            p->y = bmax.y;
            n->y = bmin.y;
        }
        if (planeNormal.z >= 0)
        {
            p->z = bmax.z;
            n->z = bmin.z;
        }
    }

public:
    /// <summary>
    /// Returns one of the 6 planes related to this frustum.
    /// </summary>
    /// <param name="index">Plane index where 0 fro Left, 1 for Right, 2 for Top, 3 for Bottom, 4 for Near, 5 for Far</param>
    /// <returns>The plane.</returns>
    Plane& getPlane(const int index)
    {
        switch (index)
        {
        case 0:
            return planeLeft;
        case 1:
            return planeRight;
        case 2:
            return planeTop;
        case 3:
            return planeBottom;
        case 4:
            return planeNear;
        case 5:
            return planeFar;
        default:
            return planeLeft;
        }
    }

    /// <summary>
    /// Checks if the bounding frustum contains the bounding box.
    /// </summary>
    /// <param name="box">The bounding box.</param>
    /// <returns>The result, true when contains.</returns>
    bool contains(BoundingBox& box)
    {
        Vector3 p;
        Vector3 n;

        for (auto i = 0; i < 6; i++)
        {
            var plane = getPlane(i);

            getBoxToPlanePVertexNVertex(box, plane.normal, &p, &n);

            if (Plane::planeIntersectsPoint(plane, p) == Back)
                return false;

            if (Plane::planeIntersectsPoint(plane, n) == Back)
                return true;
        }

        return true;
    }

    bool containsSphere(Vector3& position, const float radius)
    {
        for (var i = 0; i < 6; i++)
        {
            var plane = getPlane(i);

            if (plane.dot(position) + radius < 0.0f)
                return false;
        }

        return true;
    }

    void setPlanes(Matrix& matrix)
    {
        // Left plane
        planeLeft.normal.x = matrix.M03 + matrix.M00;
        planeLeft.normal.y = matrix.M13 + matrix.M10;
        planeLeft.normal.z = matrix.M23 + matrix.M20;
        planeLeft.distance = matrix.M33 + matrix.M30;

        // Right plane
        planeRight.normal.x = matrix.M03 - matrix.M00;
        planeRight.normal.y = matrix.M13 - matrix.M10;
        planeRight.normal.z = matrix.M23 - matrix.M20;
        planeRight.distance = matrix.M33 - matrix.M30;

        // Top plane
        planeTop.normal.x = matrix.M03 - matrix.M01;
        planeTop.normal.y = matrix.M13 - matrix.M11;
        planeTop.normal.z = matrix.M23 - matrix.M21;
        planeTop.distance = matrix.M33 - matrix.M31;

        // Bottom plane
        planeBottom.normal.x = matrix.M03 + matrix.M01;
        planeBottom.normal.y = matrix.M13 + matrix.M11;
        planeBottom.normal.z = matrix.M23 + matrix.M21;
        planeBottom.distance = matrix.M33 + matrix.M31;

        // Near plane
        planeNear.normal.x = matrix.M02;
        planeNear.normal.y = matrix.M12;
        planeNear.normal.z = matrix.M22;
        planeNear.distance = matrix.M32;

        // Far plane
        planeFar.normal.x = matrix.M03 - matrix.M02;
        planeFar.normal.y = matrix.M13 - matrix.M12;
        planeFar.normal.z = matrix.M23 - matrix.M22;
        planeFar.distance = matrix.M33 - matrix.M32;
    }

public:
    Plane planeNear = {};
    Plane planeFar = {};
    Plane planeLeft = {};
    Plane planeRight = {};
    Plane planeTop = {};
    Plane planeBottom = {};
};

#endif // BOUNDINGFRUSTUM_H
