// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

enum class DrawType : byte
{
    /// <summary>
    ///     The default render stage. Renders solid triangles, to view 1 (GBuffer).
    /// </summary>
    Default = 0,

    /// <summary>
    ///     The UI render stage. Renders solid triangles, to view 0 (backbuffer).
    /// </summary>
    DrawUI,

    /// <summary>
    ///     The WebUI render stage. Renders solid triangles, to view 0 (backbuffer).
    /// </summary>
    DrawWebUI,

    /// <summary>
    ///     The debug lines render stage. Renders lines, to view 0 (backbuffer).
    /// </summary>
    DebugDrawLines,

    /// <summary>
    ///     The debug triangles render stage. Renders solid triangles, to view 0 (backbuffer).
    /// </summary>
    DebugDrawTriangles
};