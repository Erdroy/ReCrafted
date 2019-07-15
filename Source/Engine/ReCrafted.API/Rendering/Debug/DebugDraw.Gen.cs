// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Rendering.Debug 
{
    /// <summary>
    ///     The DebugDraw class. Provides drawing functions for debugging.
    /// </summary>
    public static class DebugDraw
    {
        
        /// <summary>
        ///     Draws arrow at the end point with line.
        /// </summary>
        /// <param name="start">The line start.</param>
        /// <param name="end">The line end and arrow position.</param>
        /// <param name="arrowSize">The arrow length.</param>
        public static void DrawArrow(Vector3 start, Vector3 end, float arrowSize = 0.25f)
        {
            InternalDrawArrow(start, end, arrowSize);
        }
        
        /// <summary>
        ///     Draws line.
        /// </summary>
        /// <param name="start">The line start.</param>
        /// <param name="end">The line end.</param>
        public static void DrawLine(Vector3 start, Vector3 end)
        {
            InternalDrawLine(start, end);
        }
        
        /// <summary>
        ///     Draws box using given bounds.
        /// </summary>
        /// <param name="bounds">The box bounds.</param>
        public static void DrawBox(BoundingBox bounds)
        {
            InternalDrawBox(bounds);
        }
        
        /// <summary>
        ///     Draws box using given position (center) and size.
        /// </summary>
        /// <param name="center">The center position.</param>
        /// <param name="size">The size of the box.</param>
        public static void DrawBox(Vector3 center, Vector3 size)
        {
            InternalDrawBox(center, size);
        }
        
        /// <summary>
        ///     Draws wire box using given position (center) and size.
        /// </summary>
        /// <param name="center">The center position.</param>
        /// <param name="size">The size of the box.</param>
        public static void DrawWireBox(Vector3 center, Vector3 size)
        {
            InternalDrawWireBox(center, size);
        }
        
        /// <summary>
        ///     Draws wire box using given bounds.
        /// </summary>
        /// <param name="bounds">The box bounds.</param>
        public static void DrawWireBox(BoundingBox bounds)
        {
            InternalDrawWireBox(bounds);
        }
        
        /// <summary>
        ///     Draws given frustum as lines between every corner.
        /// </summary>
        /// <param name="frustum">The frustum.</param>
        public static void DrawWireFrustum(BoundingFrustum frustum)
        {
            InternalDrawWireFrustum(frustum);
        }
        
        /// <summary>
        ///     Draws sphere at given position.
        /// </summary>
        /// <param name="center">The sphere draw position.</param>
        /// <param name="radius">The sphere radius.</param>
        public static void DrawSphere(Vector3 center, float radius)
        {
            InternalDrawSphere(center, radius);
        }
        
        /// <summary>
        ///     Draws wire sphere at given position.
        /// </summary>
        /// <param name="center">The sphere draw position.</param>
        /// <param name="radius">The sphere radius.</param>
        public static void DrawWireSphere(Vector3 center, float radius)
        {
            InternalDrawWireSphere(center, radius);
        }
        
        /// <summary>
        ///     Draws wire circle.
        /// </summary>
        /// <param name="center">The circle center point.</param>
        /// <param name="majorAxis">The major axis point.</param>
        /// <param name="minorAxis">The minor axis point.</param>
        public static void DrawWireCircle(Vector3 center, Vector3 majorAxis, Vector3 minorAxis)
        {
            InternalDrawWireCircle(center, majorAxis, minorAxis);
        }

        /// <summary>
        ///     Sets current debug draw render color.
        /// </summary>
        public static Color4 Color
        {
            get
            {
                Get_InternalColor(out var data);
                return data;
            }
            set => Set_InternalColor(ref value);
        }

        /// <summary>
        ///     Sets matrix that will be used for vertex transformation.
        /// </summary>
        public static Matrix Matrix
        {
            get
            {
                Get_InternalMatrix(out var data);
                return data;
            }
            set => Set_InternalMatrix(ref value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDrawArrow(Vector3 start, Vector3 end, float arrowSize);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDrawLine(Vector3 start, Vector3 end);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDrawBox(BoundingBox bounds);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDrawBox(Vector3 center, Vector3 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDrawWireBox(Vector3 center, Vector3 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDrawWireBox(BoundingBox bounds);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDrawWireFrustum(BoundingFrustum frustum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDrawSphere(Vector3 center, float radius);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDrawWireSphere(Vector3 center, float radius);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDrawWireCircle(Vector3 center, Vector3 majorAxis, Vector3 minorAxis);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalColor(out Color4 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalColor(ref Color4 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalMatrix(out Matrix data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalMatrix(ref Matrix data);
    }
}