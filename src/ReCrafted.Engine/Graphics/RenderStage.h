// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef RENDERSTAGE_H
#define RENDERSTAGE_H

/**
 * \brief RenderStage enum. Contains all valid render stages,
 */
struct RenderStage
{
    enum _enum : byte
    {
        /**
         * \brief The default render stage. Renders solid triangles, to view 1 (GBuffer).
         */
        Default = 0,

        /**
        * \brief The UI render stage. Renders solid triangles, to view 0 (backbuffer).
        */
        DrawUI,

        /**
        * \brief The WebUI render stage. Renders solid triangles, to view 0 (backbuffer).
        */
        DrawWebUI,

        /**
        * \brief The debug lines render stage. Renders lines, to view 0 (backbuffer).
        */
        DebugDrawLines,

        /**
        * \brief The debug triangles render stage. Renders solid triangles, to view 0 (backbuffer).
        */
        DebugDrawTriangles
    };
};

#endif // RENDERSTAGE_H
