// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef BOUNDINGFRUSTUM_H
#define BOUNDINGFRUSTUM_H

// includes
#include "Plane.h"
#include "BoundingBox.h"

struct BoundingFrustum
{
private:
	void getBoxToPlanePVertexNVertex(BoundingBox& box, Vector3& planeNormal, Vector3* p, Vector3* n) const
	{
		*p = box.minimum();
		if (planeNormal.x >= 0)
			p->x = box.maximum().x;
		if (planeNormal.y >= 0)
			p->y = box.maximum().y;
		if (planeNormal.z >= 0)
			p->z = box.maximum().z;

		*n = box.maximum();
		if (planeNormal.x >= 0)
			n->x = box.minimum().x;
		if (planeNormal.y >= 0)
			n->y = box.minimum().y;
		if (planeNormal.z >= 0)
			n->z = box.minimum().z;
	}

public:
	/// <summary>
	/// Returns one of the 6 planes related to this frustum.
	/// </summary>
	/// <param name="index">Plane index where 0 fro Left, 1 for Right, 2 for Top, 3 for Bottom, 4 for Near, 5 for Far</param>
	/// <returns>The plane.</returns>
	Plane& getPlane(int index)
	{
		switch(index)
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
		Vector3 p, n;
		Plane plane;

		for (auto i = 0; i < 6; i++)
		{
			plane = getPlane(i);

			getBoxToPlanePVertexNVertex(box, plane.normal, &p, &n);

			if (Plane::planeIntersectsPoint(plane, p) == PlaneIntersection::Back)
				return false;

			if (Plane::planeIntersectsPoint(plane, n) == PlaneIntersection::Back)
				return true;
		}

		return true;
	}

public:
	/// <summary>
	/// Creates a new frustum relaying on perspective camera parameters
	/// </summary>
	/// <param name="position">The camera pos.</param>
	/// <param name="direction">The look dir.</param>
	/// <param name="up">Up dir.</param>
	/// <param name="fov">The fov.</param>
	/// <param name="znear">The znear.</param>
	/// <param name="zfar">The zfar.</param>
	/// <param name="aspect">The aspect.</param>
	/// <returns>The bounding frustum calculated from perspective camera</returns>
	static BoundingFrustum FromCamera(Vector3& position, Vector3& direction, Vector3& up, float fov, float znear, float zfar, float aspect)
	{
		direction = Vector3(direction.x, direction.y, direction.z);
		direction.normalize();
		up.normalize();

		auto nearCenter = position + direction * znear;
		auto farCenter = position + direction * zfar;
		auto nearHalfHeight = znear * Math::tanf(fov  * 0.5f);
		auto farHalfHeight = zfar * Math::tanf(fov * 0.5f);
		auto nearHalfWidth = nearHalfHeight * aspect;
		auto farHalfWidth = farHalfHeight * aspect;

		auto rightDir = Vector3::normalize(Vector3::cross(up, direction));
		auto Near1 = nearCenter - up * nearHalfHeight + rightDir * nearHalfWidth;
		auto Near2 = nearCenter + up * nearHalfHeight + rightDir * nearHalfWidth;
		auto Near3 = nearCenter + up * nearHalfHeight - rightDir *  nearHalfWidth;
		auto Near4 = nearCenter - up * nearHalfHeight - rightDir *  nearHalfWidth;
		auto Far1 = farCenter - up * farHalfHeight + rightDir * farHalfWidth;
		auto Far2 = farCenter + up *  farHalfHeight + rightDir *  farHalfWidth;
		auto Far3 = farCenter + up * farHalfHeight - rightDir * farHalfWidth;
		auto Far4 = farCenter - up * farHalfHeight - rightDir *  farHalfWidth;

		auto result = BoundingFrustum();
		result.planeNear = Plane(Near1, Near2, Near3);
		result.planeFar = Plane(Far3, Far2, Far1);
		result.planeLeft = Plane(Near4, Near3, Far3);
		result.planeRight = Plane(Far1, Far2, Near2);
		result.planeTop = Plane(Near2, Far2, Far3);
		result.planeBottom = Plane(Far4, Far1, Near1);

		result.planeNear.normalize();
		result.planeFar.normalize();
		result.planeLeft.normalize();
		result.planeRight.normalize();
		result.planeTop.normalize();
		result.planeBottom.normalize();

		return result;
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
