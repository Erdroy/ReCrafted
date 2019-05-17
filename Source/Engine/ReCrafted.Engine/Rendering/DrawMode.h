// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

enum class DrawMode : byte
{
    /// <summary>
    ///     The default draw mode. Renders solid triangles, to view 1 (GBuffer).
    /// </summary>
    Default = 0,

    /// <summary>
    ///     The UI draw mode. Renders solid triangles, to view 0 (backbuffer).
    /// </summary>
    DrawUI,

    /// <summary>
    ///     The WebUI draw mode. Renders solid triangles, to view 0 (backbuffer).
    /// </summary>
    DrawWebUI,

    /// <summary>
    ///     The debug lines draw mode. Renders lines, to view 0 (backbuffer).
    /// </summary>
    DebugDrawLines,

    /// <summary>
    ///     The debug triangles draw mode. Renders solid triangles, to view 0 (backbuffer).
    /// </summary>
    DebugDrawTriangles
};