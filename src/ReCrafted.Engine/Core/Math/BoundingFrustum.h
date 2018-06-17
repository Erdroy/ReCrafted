// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef BOUNDINGFRUSTUM_H
#define BOUNDINGFRUSTUM_H

// includes
#include "Plane.h"
#include "BoundingBox.h"

#include "Core/Containers/Array.h"

struct BoundingFrustum
{
private:
    static void GetBoxToPlanePVertexNVertex(BoundingBox& box, Vector3& planeNormal, Vector3* p, Vector3* n)
    {
        cvar bmin = box.Minimum();
        cvar bmax = box.Maximum();

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

    Vector3 Get3PlanesInterPoint(const Plane& p1, const Plane& p2, const Plane& p3) const
    {
        var v =
            Vector3::Cross(p2.normal, p3.normal) / Vector3::Dot(p1.normal, Vector3::Cross(p2.normal, p3.normal)) * -p1.distance
            -Vector3::Cross(p3.normal, p1.normal) / Vector3::Dot(p2.normal, Vector3::Cross(p3.normal, p1.normal)) * p2.distance
            -Vector3::Cross(p1.normal, p2.normal) / Vector3::Dot(p3.normal, Vector3::Cross(p1.normal, p2.normal)) * p3.distance;

        return v;
    }

public:
    /// <summary>
    /// Returns one of the 6 planes related to this frustum.
    /// </summary>
    /// <param name="index">Plane index where 0 fro Left, 1 for Right, 2 for Top, 3 for Bottom, 4 for Near, 5 for Far</param>
    /// <returns>The plane.</returns>
    Plane& GetPlane(const int index)
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
    bool Contains(BoundingBox& box)
    {
        Vector3 p;
        Vector3 n;

        for (auto i = 0; i < 6; i++)
        {
            var plane = GetPlane(i);

            GetBoxToPlanePVertexNVertex(box, plane.normal, &p, &n);

            if (Plane::PlaneIntersectsPoint(plane, p) == Back)
                return false;

            if (Plane::PlaneIntersectsPoint(plane, n) == Back)
                return true;
        }

        return true;
    }

    bool ContainsSphere(Vector3& position, const float radius)
    {
        for (var i = 0; i < 6; i++)
        {
            var plane = GetPlane(i);

            if (plane.Dot(position) + radius < 0.0f)
                return false;
        }

        return true;
    }

    void SetPlanes(const Matrix& matrix)
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

    Array<Vector3> GetCorners() const
    {
        Array<Vector3> corners = {};
        corners.Reserve(8);

        corners[0] = Get3PlanesInterPoint(planeNear, planeBottom, planeRight);    //Near1
        corners[1] = Get3PlanesInterPoint(planeNear, planeTop, planeRight);       //Near2
        corners[2] = Get3PlanesInterPoint(planeNear, planeTop, planeLeft);        //Near3
        corners[3] = Get3PlanesInterPoint(planeNear, planeBottom, planeLeft);     //Near3
        corners[4] = Get3PlanesInterPoint(planeFar, planeBottom, planeRight);    //Far1
        corners[5] = Get3PlanesInterPoint(planeFar, planeTop, planeRight);       //Far2
        corners[6] = Get3PlanesInterPoint(planeFar, planeTop, planeLeft);        //Far3
        corners[7] = Get3PlanesInterPoint(planeFar, planeBottom, planeLeft);     //Far3

        return corners;
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
