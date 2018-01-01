// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MATRIX_H
#define MATRIX_H

// includes
#include "ReCrafted.h"
#include "Vector3.h"

struct Matrix
{
public:
	/// <summary>
	/// Default constructor of matrix structure, 
	/// sets 0 for all the components, better to use Matrix::identity().
	/// </summary>
	Matrix() : M00(0), M01(0), M02(0), M03(0),
		M10(0), M11(0), M12(0), M13(0),
		M20(0), M21(0), M22(0), M23(0),
		M30(0), M31(0), M32(0), M33(0) { }

	/// <summary>
	/// Transpose the matrix.
	/// </summary>
	FORCEINLINE void transpose()
	{
		// make temporary copy
		auto temp = *this;

		M00 = temp.M00;
		M01 = temp.M10;
		M02 = temp.M20;
		M03 = temp.M30;

		M10 = temp.M01;
		M11 = temp.M11;
		M12 = temp.M21;
		M13 = temp.M31;

		M20 = temp.M02;
		M21 = temp.M12;
		M22 = temp.M22;
		M23 = temp.M32;

		M30 = temp.M03;
		M31 = temp.M13;
		M32 = temp.M23;
		M33 = temp.M33;
	}

public:
	/// <summary>
	/// Returns matrix identity.
	/// Sets 1 to [0,0], [1,1], [2,2], [3,3].
	/// </summary>
	/// <returns>The identity matrix.</returns>
	FORCEINLINE static Matrix identity()
	{
		auto matrix = Matrix();

		matrix.M00 = 1.0f;
		matrix.M11 = 1.0f;
		matrix.M22 = 1.0f;
		matrix.M33 = 1.0f;

		return matrix;
	}

	/// <summary>
	/// Returns default empty-zeroed matrix.
	/// </summary>
	/// <returns>The empty matrix.</returns>
	FORCEINLINE static Matrix zero()
	{
		return Matrix();
	}

	/// <summary>
	/// Transpose the matrix.
	/// </summary>
	/// <param name="matrix">Matrix to transpose.</param>
	/// <returns>The result.</returns>
	FORCEINLINE static Matrix transpose(Matrix matrix)
	{
		matrix.transpose();
		return matrix;
	}

	/// <summary>
	/// Transpose the matrix.
	/// </summary>
	/// <param name="matrix">Matrix to transpose.</param>
	/// <param name="result">The result.</param>
	FORCEINLINE static void transpose(Matrix matrix, Matrix* result)
	{
		matrix.transpose();
		*result = matrix;
	}

	/// <summary>
	/// Creates perspective matrix based on xfov - left handed.
	/// </summary>
	/// <param name="xfov">The horizontal fov.</param>
	/// <param name="aspect">The size aspect.</param>
	/// <param name="znear">The Z-Near plane distance.</param>
	/// <param name="zfar">The Z-Far plane distance.</param>
	/// <returns>The result.</returns>
	FORCEINLINE static Matrix createPerspectiveFovLH(float xfov, float aspect, float znear, float zfar)
	{
		auto m = zero();
		auto yScale = 1.0f / tan(xfov * 0.5f);
		auto q = zfar / (zfar - znear);

		m.M00 = yScale / aspect;
		m.M11 = yScale;
		m.M22 = q;
		m.M23 = 1.0f;
		m.M32 = -q * znear;

		return m;
	}

	FORCEINLINE static void createPerspectiveFovLH(float xfov, float aspect, float znear, float zfar, Matrix* result)
	{
		*result = createPerspectiveFovLH(xfov, aspect, znear, zfar);
	}


	FORCEINLINE static Matrix createViewLH(Vector3 position, Vector3 target, Vector3 up)
	{
		auto m = identity();
		
		auto zAxis = target - position;
		zAxis.normalize();

		auto xAxis = Vector3::cross(up, zAxis);
		xAxis.normalize();

		auto yAxis = Vector3::cross(zAxis, xAxis);

		m.M00 = xAxis.x;
		m.M01 = yAxis.x;
		m.M02 = zAxis.x;

		m.M10 = xAxis.y;
		m.M11 = yAxis.y;
		m.M12 = zAxis.y;

		m.M20 = xAxis.z;
		m.M21 = yAxis.z;
		m.M22 = zAxis.z;

		m.M30 = -Vector3::dot(xAxis, position);
		m.M31 = -Vector3::dot(yAxis, position);
		m.M32 = -Vector3::dot(zAxis, position);

		return m;
	}

	FORCEINLINE static void createViewLH(Vector3 position, Vector3 target, Vector3 up, Matrix* result)
	{
		*result = createViewLH(position, target, up);
	}

public:
	void Matrix::operator *= (Matrix r)
	{
		auto resultM00 = M00 * r.M00 + M01 * r.M10 + M02 * r.M20 + M03 * r.M30;
		auto resultM01 = M00 * r.M01 + M01 * r.M11 + M02 * r.M21 + M03 * r.M31;
		auto resultM02 = M00 * r.M02 + M01 * r.M12 + M02 * r.M22 + M03 * r.M32;
		auto resultM03 = M00 * r.M03 + M01 * r.M13 + M02 * r.M23 + M03 * r.M31;

		auto resultM10 = M10 * r.M00 + M11 * r.M10 + M12 * r.M20 + M13 * r.M30;
		auto resultM11 = M10 * r.M01 + M11 * r.M11 + M12 * r.M21 + M13 * r.M31;
		auto resultM12 = M10 * r.M02 + M11 * r.M12 + M12 * r.M22 + M13 * r.M32;
		auto resultM13 = M10 * r.M03 + M11 * r.M13 + M12 * r.M23 + M13 * r.M33;

		auto resultM20 = M20 * r.M00 + M21 * r.M10 + M22 * r.M20 + M23 * r.M30;
		auto resultM21 = M20 * r.M01 + M21 * r.M11 + M22 * r.M21 + M23 * r.M31;
		auto resultM22 = M20 * r.M02 + M21 * r.M12 + M22 * r.M22 + M23 * r.M32;
		auto resultM23 = M20 * r.M03 + M21 * r.M13 + M22 * r.M23 + M23 * r.M33;

		auto resultM30 = M30 * r.M00 + M31 * r.M10 + M32 * r.M20 + M33 * r.M30;
		auto resultM31 = M30 * r.M01 + M31 * r.M11 + M32 * r.M21 + M33 * r.M31;
		auto resultM32 = M30 * r.M02 + M31 * r.M12 + M32 * r.M22 + M33 * r.M32;
		auto resultM33 = M30 * r.M03 + M31 * r.M13 + M32 * r.M23 + M33 * r.M33;

		M00 = resultM00;
		M01 = resultM01;
		M02 = resultM02;
		M03 = resultM03;

		M10 = resultM10;
		M11 = resultM11;
		M12 = resultM12;
		M13 = resultM13;

		M20 = resultM20;
		M21 = resultM21;
		M22 = resultM22;
		M23 = resultM23;

		M30 = resultM30;
		M31 = resultM31;
		M32 = resultM32;
		M33 = resultM33;
	}

	Matrix Matrix::operator*(Matrix r) const
	{
		auto resultM00 = M00 * r.M00 + M01 * r.M10 + M02 * r.M20 + M03 * r.M30;
		auto resultM01 = M00 * r.M01 + M01 * r.M11 + M02 * r.M21 + M03 * r.M31;
		auto resultM02 = M00 * r.M02 + M01 * r.M12 + M02 * r.M22 + M03 * r.M32;
		auto resultM03 = M00 * r.M03 + M01 * r.M13 + M02 * r.M23 + M03 * r.M31;

		auto resultM10 = M10 * r.M00 + M11 * r.M10 + M12 * r.M20 + M13 * r.M30;
		auto resultM11 = M10 * r.M01 + M11 * r.M11 + M12 * r.M21 + M13 * r.M31;
		auto resultM12 = M10 * r.M02 + M11 * r.M12 + M12 * r.M22 + M13 * r.M32;
		auto resultM13 = M10 * r.M03 + M11 * r.M13 + M12 * r.M23 + M13 * r.M33;

		auto resultM20 = M20 * r.M00 + M21 * r.M10 + M22 * r.M20 + M23 * r.M30;
		auto resultM21 = M20 * r.M01 + M21 * r.M11 + M22 * r.M21 + M23 * r.M31;
		auto resultM22 = M20 * r.M02 + M21 * r.M12 + M22 * r.M22 + M23 * r.M32;
		auto resultM23 = M20 * r.M03 + M21 * r.M13 + M22 * r.M23 + M23 * r.M33;

		auto resultM30 = M30 * r.M00 + M31 * r.M10 + M32 * r.M20 + M33 * r.M30;
		auto resultM31 = M30 * r.M01 + M31 * r.M11 + M32 * r.M21 + M33 * r.M31;
		auto resultM32 = M30 * r.M02 + M31 * r.M12 + M32 * r.M22 + M33 * r.M32;
		auto resultM33 = M30 * r.M03 + M31 * r.M13 + M32 * r.M23 + M33 * r.M33;


		auto matrix = Matrix();
		matrix.M00 = resultM00;
		matrix.M01 = resultM01;
		matrix.M02 = resultM02;
		matrix.M03 = resultM03;

		matrix.M10 = resultM10;
		matrix.M11 = resultM11;
		matrix.M12 = resultM12;
		matrix.M13 = resultM13;

		matrix.M20 = resultM20;
		matrix.M21 = resultM21;
		matrix.M22 = resultM22;
		matrix.M23 = resultM23;

		matrix.M30 = resultM30;
		matrix.M31 = resultM31;
		matrix.M32 = resultM32;
		matrix.M33 = resultM33;

		return matrix;
	}
public:
	/// <summary>
	/// Matrix component, row 0 col 0
	/// </summary>
	float M00;

	/// <summary>
	/// Matrix component, row 0 col 1
	/// </summary>
	float M01;

	/// <summary>
	/// Matrix component, row 0 col 2
	/// </summary>
	float M02;

	/// <summary>
	/// Matrix component, row 0 col 3
	/// </summary>
	float M03;

	/// <summary>
	/// Matrix component, row 1 col 0
	/// </summary>
	float M10;

	/// <summary>
	/// Matrix component, row 1 col 1
	/// </summary>
	float M11;

	/// <summary>
	/// Matrix component, row 1 col 2
	/// </summary>
	float M12;

	/// <summary>
	/// Matrix component, row 1 col 3
	/// </summary>
	float M13;

	/// <summary>
	/// Matrix component, row 2 col 0
	/// </summary>
	float M20;

	/// <summary>
	/// Matrix component, row 2 col 1
	/// </summary>
	float M21;

	/// <summary>
	/// Matrix component, row 2 col 2
	/// </summary>
	float M22;

	/// <summary>
	/// Matrix component, row 2 col 3
	/// </summary>
	float M23;

	/// <summary>
	/// Matrix component, row 3 col 0
	/// </summary>
	float M30;

	/// <summary>
	/// Matrix component, row 3 col 1
	/// </summary>
	float M31;

	/// <summary>
	/// Matrix component, row 3 col 2
	/// </summary>
	float M32;

	/// <summary>
	/// Matrix component, row 3 col 3
	/// </summary>
	float M33;
};

#endif // MATRIX_H