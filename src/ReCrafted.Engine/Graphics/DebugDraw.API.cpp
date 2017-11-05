// ReCrafted © 2016-2017 Always Too Late

#include "DebugDraw.h"
#include "Core/Math/Color.h"
#include "Core/Math/Vector3.h"

namespace Internal
{
	void getColor(Color* color)
	{
		auto ddColor = DebugDraw::getColor();
		*color = *ddColor;
	}

    void setColor(Color* color)
	{
		auto ddColor = DebugDraw::getColor();
		*ddColor = *color;
	}

    void drawLine(Vector3 start, Vector3 end)
	{
		DebugDraw::drawLine(start, end);
	}

	void drawCube(Vector3 center, Vector3 size)
	{
		DebugDraw::drawCube(center, size);
	}

	void drawWireCube(Vector3 center, Vector3 size)
	{
		DebugDraw::drawWireCube(center, size);
	}
}

void DebugDraw::initRuntime()
{
	API_FILE("Graphics/DebugDraw.Gen.cs");
	{
		API_USING("ReCrafted.API.Mathematics");

		API_COMMENT("DebugDraw class.");
		API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Graphics", "DebugDraw");
		{
			API_COMMENT("Draws cube");
			API_METHOD(PUBLIC, STATIC, "DrawCube", EXTERN);
			{
				API_BIND("ReCrafted.API.Graphics.DebugDraw::DrawCube", &Internal::drawCube);

				API_PARAM("Vector3", "center");
				API_PARAM("Vector3", "size");
			}
			API_METHOD_END();

			API_COMMENT("Draws wireframe cube");
			API_METHOD(PUBLIC, STATIC, "DrawWireCube", EXTERN);
			{
				API_BIND("ReCrafted.API.Graphics.DebugDraw::DrawWireCube", &Internal::drawWireCube);

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
