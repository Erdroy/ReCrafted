// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef BOUNDINGFRUSTUM_H
#define BOUNDINGFRUSTUM_H

// includes
#include "ReCrafted.h"
#include "Plane.h"
#include "BoundingBox.h"

#include "Core/Containers/Array.h"

struct BoundingFrustum
{
private:
    static void GetBoxToPlanePVertexNVertex(const BoundingBox& box, const Vector3& planeNormal, Vector3* p, Vector3* n)
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
    bool Contains(const BoundingBox& box)
    {
        Vector3 p;
        Vector3 n;

        for (auto i = 0; i < 6; i++)
        {
            var plane = GetPlane(i);

            GetBoxToPlanePVertexNVertex(box, plane.normal, &p, &n);

            if (Plane::PlaneIntersectsPoint(plane, p) == PlaneIntersection::Back)
                return false;

            if (Plane::PlaneIntersectsPoint(plane, n) == PlaneIntersection::Back)
                return true;
        }

        return true;
    }

    bool ContainsSphere(const Vector3& position, const float radius)
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
        planeLeft.normal.x = matrix.m14 + matrix.m11;
        planeLeft.normal.y = matrix.m24 + matrix.m21;
        planeLeft.normal.z = matrix.m34 + matrix.m31;
        planeLeft.distance = matrix.m44 + matrix.m41;

        // Right plane
        planeRight.normal.x = matrix.m14 - matrix.m11;
        planeRight.normal.y = matrix.m24 - matrix.m21;
        planeRight.normal.z = matrix.m34 - matrix.m31;
        planeRight.distance = matrix.m44 - matrix.m41;

        // Top plane
        planeTop.normal.x = matrix.m14 - matrix.m12;
        planeTop.normal.y = matrix.m24 - matrix.m22;
        planeTop.normal.z = matrix.m34 - matrix.m32;
        planeTop.distance = matrix.m44 - matrix.m42;

        // Bottom plane
        planeBottom.normal.x = matrix.m14 + matrix.m12;
        planeBottom.normal.y = matrix.m24 + matrix.m22;
        planeBottom.normal.z = matrix.m34 + matrix.m32;
        planeBottom.distance = matrix.m44 + matrix.m42;

        // Near plane
        planeNear.normal.x = matrix.m13;
        planeNear.normal.y = matrix.m23;
        planeNear.normal.z = matrix.m33;
        planeNear.distance = matrix.m43;

        // Far plane
        planeFar.normal.x = matrix.m14 - matrix.m13;
        planeFar.normal.y = matrix.m24 - matrix.m23;
        planeFar.normal.z = matrix.m34 - matrix.m33;
        planeFar.distance = matrix.m44 - matrix.m43;
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
