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
    Matrix() : M11(0), M12(0), M13(0), M14(0),
               M21(0), M22(0), M23(0), M24(0),
               M31(0), M32(0), M33(0), M34(0),
               M41(0), M42(0), M43(0), M44(0)
    {
    }
    
    /**
     * \brief Inverts this matrix.
     */
    FORCEINLINE void Invert()
    {
        Invert(*this, this);
    }

    /// <summary>
    /// Transpose the matrix.
    /// </summary>
    FORCEINLINE void Transpose()
    {
        // make temporary copy
        auto temp = *this;

        M11 = temp.M11;
        M12 = temp.M21;
        M13 = temp.M31;
        M14 = temp.M41;

        M21 = temp.M12;
        M22 = temp.M22;
        M23 = temp.M32;
        M24 = temp.M42;

        M31 = temp.M13;
        M32 = temp.M23;
        M33 = temp.M33;
        M34 = temp.M43;

        M41 = temp.M14;
        M42 = temp.M24;
        M43 = temp.M34;
        M44 = temp.M44;
    }

public:
    /// <summary>
    /// Returns matrix identity.
    /// Sets 1 to [0,0], [1,1], [2,2], [3,3].
    /// </summary>
    /// <returns>The identity matrix.</returns>
    FORCEINLINE static Matrix Identity()
    {
        auto matrix = Matrix();

        matrix.M11 = 1.0f;
        matrix.M22 = 1.0f;
        matrix.M33 = 1.0f;
        matrix.M44 = 1.0f;

        return matrix;
    }

    /// <summary>
    /// Returns default empty-zeroed matrix.
    /// </summary>
    /// <returns>The empty matrix.</returns>
    FORCEINLINE static Matrix Zero()
    {
        return Matrix();
    }

    /// <summary>
    /// Transpose the matrix.
    /// </summary>
    /// <param name="matrix">Matrix to transpose.</param>
    /// <returns>The result.</returns>
    FORCEINLINE static Matrix Transpose(Matrix matrix)
    {
        matrix.Transpose();
        return matrix;
    }

    /// <summary>
    /// Transpose the matrix.
    /// </summary>
    /// <param name="matrix">Matrix to transpose.</param>
    /// <param name="result">The result.</param>
    FORCEINLINE static void Transpose(Matrix matrix, Matrix* result)
    {
        matrix.Transpose();
        *result = matrix;
    }

    /// <summary>
    /// Inverts given matrix.
    /// </summary>
    /// <param name="matrix">Matrix to invert.</param>
    /// <returns>The inverted matrix.</returns>
    FORCEINLINE static Matrix Invert(const Matrix& matrix)
    {
        Matrix result;
        Invert(matrix, &result);
        return result;
    }

    /// <summary>
    /// Inverts given matrix.
    /// </summary>
    /// <param name="value">Matrix to invert.</param>
    /// <param name="result">The result.</param>
    FORCEINLINE static void Invert(const Matrix& value, Matrix* result)
    {
        float b0 = (value.M31 * value.M42) - (value.M32 * value.M41);
        float b1 = (value.M31 * value.M43) - (value.M33 * value.M41);
        float b2 = (value.M34 * value.M41) - (value.M31 * value.M44);
        float b3 = (value.M32 * value.M43) - (value.M33 * value.M42);
        float b4 = (value.M34 * value.M42) - (value.M32 * value.M44);
        float b5 = (value.M33 * value.M44) - (value.M34 * value.M43);

        float d11 = value.M22 * b5 + value.M23 * b4 + value.M24 * b3;
        float d12 = value.M21 * b5 + value.M23 * b2 + value.M24 * b1;
        float d13 = value.M21 * -b4 + value.M22 * b2 + value.M24 * b0;
        float d14 = value.M21 * b3 + value.M22 * -b1 + value.M23 * b0;

        float det = value.M11 * d11 - value.M12 * d12 + value.M13 * d13 - value.M14 * d14;
        if (fabs(det) == 0.0f)
        {
            *result = Zero();
            return;
        }

        det = 1.0f / det;

        float a0 = (value.M11 * value.M22) - (value.M12 * value.M21);
        float a1 = (value.M11 * value.M23) - (value.M13 * value.M21);
        float a2 = (value.M14 * value.M21) - (value.M11 * value.M24);
        float a3 = (value.M12 * value.M23) - (value.M13 * value.M22);
        float a4 = (value.M14 * value.M22) - (value.M12 * value.M24);
        float a5 = (value.M13 * value.M24) - (value.M14 * value.M23);

        float d21 = value.M12 * b5 + value.M13 * b4 + value.M14 * b3;
        float d22 = value.M11 * b5 + value.M13 * b2 + value.M14 * b1;
        float d23 = value.M11 * -b4 + value.M12 * b2 + value.M14 * b0;
        float d24 = value.M11 * b3 + value.M12 * -b1 + value.M13 * b0;

        float d31 = value.M42 * a5 + value.M43 * a4 + value.M44 * a3;
        float d32 = value.M41 * a5 + value.M43 * a2 + value.M44 * a1;
        float d33 = value.M41 * -a4 + value.M42 * a2 + value.M44 * a0;
        float d34 = value.M41 * a3 + value.M42 * -a1 + value.M43 * a0;

        float d41 = value.M32 * a5 + value.M33 * a4 + value.M34 * a3;
        float d42 = value.M31 * a5 + value.M33 * a2 + value.M34 * a1;
        float d43 = value.M31 * -a4 + value.M32 * a2 + value.M34 * a0;
        float d44 = value.M31 * a3 + value.M32 * -a1 + value.M33 * a0;

        result->M11 = +d11 * det; result->M12 = -d21 * det; result->M13 = +d31 * det; result->M14 = -d41 * det;
        result->M21 = -d12 * det; result->M22 = +d22 * det; result->M23 = -d32 * det; result->M24 = +d42 * det;
        result->M31 = +d13 * det; result->M32 = -d23 * det; result->M33 = +d33 * det; result->M34 = -d43 * det;
        result->M41 = -d14 * det; result->M42 = +d24 * det; result->M43 = -d34 * det; result->M44 = +d44 * det;
    }

    /// <summary>
    /// Creates perspective matrix based on xfov - left handed.
    /// </summary>
    /// <param name="xfov">The horizontal fov.</param>
    /// <param name="aspect">The size aspect.</param>
    /// <param name="znear">The Z-Near plane distance.</param>
    /// <param name="zfar">The Z-Far plane distance.</param>
    /// <returns>The result.</returns>
    FORCEINLINE static Matrix CreatePerspectiveFovLH(float xfov, float aspect, float znear, float zfar)
    {
        auto m = Zero();
        auto yScale = 1.0f / tan(xfov * 0.5f);
        auto q = zfar / (zfar - znear);

        m.M11 = yScale / aspect;
        m.M22 = yScale;
        m.M33 = q;
        m.M34 = 1.0f;
        m.M43 = -q * znear;

        return m;
    }

    FORCEINLINE static void CreatePerspectiveFovLH(float xfov, float aspect, float znear, float zfar, Matrix* result)
    {
        *result = CreatePerspectiveFovLH(xfov, aspect, znear, zfar);
    }


    FORCEINLINE static Matrix CreateViewLH(Vector3 position, Vector3 target, Vector3 up)
    {
        auto m = Identity();

        auto zAxis = target - position;
        zAxis.Normalize();

        auto xAxis = Vector3::Cross(up, zAxis);
        xAxis.Normalize();

        auto yAxis = Vector3::Cross(zAxis, xAxis);

        m.M11 = xAxis.x;
        m.M12 = yAxis.x;
        m.M13 = zAxis.x;

        m.M21 = xAxis.y;
        m.M22 = yAxis.y;
        m.M23 = zAxis.y;

        m.M31 = xAxis.z;
        m.M32 = yAxis.z;
        m.M33 = zAxis.z;

        m.M41 = -Vector3::Dot(xAxis, position);
        m.M42 = -Vector3::Dot(yAxis, position);
        m.M43 = -Vector3::Dot(zAxis, position);

        return m;
    }

    FORCEINLINE static void CreateViewLH(Vector3 position, Vector3 target, Vector3 up, Matrix* result)
    {
        *result = CreateViewLH(position, target, up);
    }

public:
    void Matrix::operator *=(Matrix r)
    {
        auto resultM00 = M11 * r.M11 + M12 * r.M21 + M13 * r.M31 + M14 * r.M41;
        auto resultM01 = M11 * r.M12 + M12 * r.M22 + M13 * r.M32 + M14 * r.M42;
        auto resultM02 = M11 * r.M13 + M12 * r.M23 + M13 * r.M33 + M14 * r.M43;
        auto resultM03 = M11 * r.M14 + M12 * r.M24 + M13 * r.M34 + M14 * r.M42;

        auto resultM10 = M21 * r.M11 + M22 * r.M21 + M23 * r.M31 + M24 * r.M41;
        auto resultM11 = M21 * r.M12 + M22 * r.M22 + M23 * r.M32 + M24 * r.M42;
        auto resultM12 = M21 * r.M13 + M22 * r.M23 + M23 * r.M33 + M24 * r.M43;
        auto resultM13 = M21 * r.M14 + M22 * r.M24 + M23 * r.M34 + M24 * r.M44;

        auto resultM20 = M31 * r.M11 + M32 * r.M21 + M33 * r.M31 + M34 * r.M41;
        auto resultM21 = M31 * r.M12 + M32 * r.M22 + M33 * r.M32 + M34 * r.M42;
        auto resultM22 = M31 * r.M13 + M32 * r.M23 + M33 * r.M33 + M34 * r.M43;
        auto resultM23 = M31 * r.M14 + M32 * r.M24 + M33 * r.M34 + M34 * r.M44;

        auto resultM30 = M41 * r.M11 + M42 * r.M21 + M43 * r.M31 + M44 * r.M41;
        auto resultM31 = M41 * r.M12 + M42 * r.M22 + M43 * r.M32 + M44 * r.M42;
        auto resultM32 = M41 * r.M13 + M42 * r.M23 + M43 * r.M33 + M44 * r.M43;
        auto resultM33 = M41 * r.M14 + M42 * r.M24 + M43 * r.M34 + M44 * r.M44;

        M11 = resultM00;
        M12 = resultM01;
        M13 = resultM02;
        M14 = resultM03;

        M21 = resultM10;
        M22 = resultM11;
        M23 = resultM12;
        M24 = resultM13;

        M31 = resultM20;
        M32 = resultM21;
        M33 = resultM22;
        M34 = resultM23;

        M41 = resultM30;
        M42 = resultM31;
        M43 = resultM32;
        M44 = resultM33;
    }

    Matrix Matrix::operator*(Matrix r) const
    {
        auto resultM00 = M11 * r.M11 + M12 * r.M21 + M13 * r.M31 + M14 * r.M41;
        auto resultM01 = M11 * r.M12 + M12 * r.M22 + M13 * r.M32 + M14 * r.M42;
        auto resultM02 = M11 * r.M13 + M12 * r.M23 + M13 * r.M33 + M14 * r.M43;
        auto resultM03 = M11 * r.M14 + M12 * r.M24 + M13 * r.M34 + M14 * r.M42;

        auto resultM10 = M21 * r.M11 + M22 * r.M21 + M23 * r.M31 + M24 * r.M41;
        auto resultM11 = M21 * r.M12 + M22 * r.M22 + M23 * r.M32 + M24 * r.M42;
        auto resultM12 = M21 * r.M13 + M22 * r.M23 + M23 * r.M33 + M24 * r.M43;
        auto resultM13 = M21 * r.M14 + M22 * r.M24 + M23 * r.M34 + M24 * r.M44;

        auto resultM20 = M31 * r.M11 + M32 * r.M21 + M33 * r.M31 + M34 * r.M41;
        auto resultM21 = M31 * r.M12 + M32 * r.M22 + M33 * r.M32 + M34 * r.M42;
        auto resultM22 = M31 * r.M13 + M32 * r.M23 + M33 * r.M33 + M34 * r.M43;
        auto resultM23 = M31 * r.M14 + M32 * r.M24 + M33 * r.M34 + M34 * r.M44;

        auto resultM30 = M41 * r.M11 + M42 * r.M21 + M43 * r.M31 + M44 * r.M41;
        auto resultM31 = M41 * r.M12 + M42 * r.M22 + M43 * r.M32 + M44 * r.M42;
        auto resultM32 = M41 * r.M13 + M42 * r.M23 + M43 * r.M33 + M44 * r.M43;
        auto resultM33 = M41 * r.M14 + M42 * r.M24 + M43 * r.M34 + M44 * r.M44;


        auto matrix = Matrix();
        matrix.M11 = resultM00;
        matrix.M12 = resultM01;
        matrix.M13 = resultM02;
        matrix.M14 = resultM03;

        matrix.M21 = resultM10;
        matrix.M22 = resultM11;
        matrix.M23 = resultM12;
        matrix.M24 = resultM13;

        matrix.M31 = resultM20;
        matrix.M32 = resultM21;
        matrix.M33 = resultM22;
        matrix.M34 = resultM23;

        matrix.M41 = resultM30;
        matrix.M42 = resultM31;
        matrix.M43 = resultM32;
        matrix.M44 = resultM33;

        return matrix;
    }

public:
    /// <summary>
    /// Matrix component, row 0 col 0
    /// </summary>
    float M11;

    /// <summary>
    /// Matrix component, row 0 col 1
    /// </summary>
    float M12;

    /// <summary>
    /// Matrix component, row 0 col 2
    /// </summary>
    float M13;

    /// <summary>
    /// Matrix component, row 0 col 3
    /// </summary>
    float M14;

    /// <summary>
    /// Matrix component, row 1 col 0
    /// </summary>
    float M21;

    /// <summary>
    /// Matrix component, row 1 col 1
    /// </summary>
    float M22;

    /// <summary>
    /// Matrix component, row 1 col 2
    /// </summary>
    float M23;

    /// <summary>
    /// Matrix component, row 1 col 3
    /// </summary>
    float M24;

    /// <summary>
    /// Matrix component, row 2 col 0
    /// </summary>
    float M31;

    /// <summary>
    /// Matrix component, row 2 col 1
    /// </summary>
    float M32;

    /// <summary>
    /// Matrix component, row 2 col 2
    /// </summary>
    float M33;

    /// <summary>
    /// Matrix component, row 2 col 3
    /// </summary>
    float M34;

    /// <summary>
    /// Matrix component, row 3 col 0
    /// </summary>
    float M41;

    /// <summary>
    /// Matrix component, row 3 col 1
    /// </summary>
    float M42;

    /// <summary>
    /// Matrix component, row 3 col 2
    /// </summary>
    float M43;

    /// <summary>
    /// Matrix component, row 3 col 3
    /// </summary>
    float M44;
};

#endif // MATRIX_H
