// ReCrafted (c) 2016-2018 Always Too Late

#include "DebugDraw.h"
#include "Scripting/Mono.h"
#include "Scripting/Bindings.h"

namespace Internal
{
    void getColor(Color* color)
    {
        *color = DebugDraw::GetColor();
    }

    void setColor(Color* color)
    {
        DebugDraw::SetColor(*color);
    }

    void drawLine(Vector3 start, Vector3 end)
    {
        DebugDraw::DrawLine(start, end);
    }

    void drawBox(Vector3 center, Vector3 size)
    {
        DebugDraw::DrawBox(center, size);
    }

    void drawWireBox(Vector3 center, Vector3 size)
    {
        DebugDraw::DrawWireBox(center, size);
    }

    void drawArrow(Vector3 start, Vector3 end, float arrowSize)
    {
        DebugDraw::DrawArrow(start, end, arrowSize);
    }
}

void DebugDraw::InitRuntime()
{
    API_FILE("Graphics/DebugDraw.Gen.cs");
    {
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("DebugDraw class.");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Graphics", "DebugDraw");
        {
            API_COMMENT("Draws box using given position (center) and size.");
            API_METHOD(PUBLIC, STATIC, "DrawBox", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.DebugDraw::DrawBox", &Internal::drawBox);

                API_PARAM("Vector3", "center");
                API_PARAM("Vector3", "size");
            }
            API_METHOD_END();

            API_COMMENT("Draws wire box using given position (center) and size.");
            API_METHOD(PUBLIC, STATIC, "DrawWireBox", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.DebugDraw::DrawWireBox", &Internal::drawWireBox);

                API_PARAM("Vector3", "center");
                API_PARAM("Vector3", "size");
            }
            API_METHOD_END();

            API_COMMENT("Draws line");
            API_METHOD(PUBLIC, STATIC, "DrawLine", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.DebugDraw::DrawLine", &Internal::drawLine);

                API_PARAM("Vector3", "start");
                API_PARAM("Vector3", "end");
            }
            API_METHOD_END();

            API_COMMENT("Draws sphere at given position.");
            API_METHOD(PUBLIC, STATIC, "DrawSphere", EXTERN);
            {
                API_PARAM("Vector3", "center");
                API_PARAM("float", "radius");
            }
            API_METHOD_END();

            API_COMMENT("Draws wire sphere at given position.");
            API_METHOD(PUBLIC, STATIC, "DrawWireSphere", EXTERN);
            {
                API_PARAM("Vector3", "center");
                API_PARAM("float", "radius");
            }
            API_METHOD_END();

            API_COMMENT("Draws arrow at the end point with line.");
            API_METHOD(PUBLIC, STATIC, "DrawArrow", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.DebugDraw::DrawArrow", &Internal::drawArrow);

                API_PARAM("Vector3", "start");
                API_PARAM("Vector3", "end");
                API_PARAM("float", "arrowSize");
            }
            API_METHOD_END();

            API_COMMENT("Gets/Sets drawing color.");
            API_PROPERTY(PUBLIC, STATIC, "Color", "Color", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Graphics.DebugDraw::Internal_Color_Get", &Internal::getColor);
                API_BIND("ReCrafted.API.Graphics.DebugDraw::Internal_Color_Set", &Internal::setColor);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
