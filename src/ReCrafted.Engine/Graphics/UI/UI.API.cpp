// ReCrafted © 2016-2017 Always Too Late

#include "UI.h"
#include "Scripting/Mono.h"

namespace Internal
{
	void drawTex2d(Texture2D* texture, Rectf* rect, Rectf* uvs)
	{
		if (texture == nullptr)
			return;

		UI::drawTexture(texture, *rect, *uvs);
	}

	void drawString(Font* font, MonoString* string, Vector2* position)
	{
		if (font == nullptr)
			return;

		auto str = MONO_TEXT(string);
		auto strlen = mono_string_length(string);

		UI::drawText(font, static_cast<Char*>(str), strlen, *position);
	}
}

void UI::initRuntime()
{
	API_FILE("UI/UI.Gen.cs");
	{
		API_USING("ReCrafted.API.Mathematics");
		API_USING("ReCrafted.API.Graphics");

		API_COMMENT("UI internal class.");
		API_CLASS(INTERNAL, STATIC, "ReCrafted.API.UI", "UIInternal");
		{
			API_COMMENT("Draws box.");
			API_METHOD(INTERNAL, STATIC, "DrawBox", EXTERN);
			{
				API_BIND("ReCrafted.API.UI.UIInternal::DrawBox", &UI::drawBox);
				
				API_COMMENT("The rectangle which will be used to draw box");
				API_PARAM("RectangleF", "rect");
			}
			API_METHOD_END();

			API_COMMENT("Draws string.");
			API_METHOD(INTERNAL, STATIC, "DrawString", EXTERN);
			{
				API_BIND("ReCrafted.API.UI.UIInternal::DrawString", &Internal::drawString);

				API_PARAM("IntPtr", "font");
				API_PARAM("string", "text");
				API_PARAM("ref Vector2", "position");
			}
			API_METHOD_END();

            API_COMMENT("Sets view rect.");
            API_METHOD(INTERNAL, STATIC, "UpdateViewRect", EXTERN);
            {
                API_BIND("ReCrafted.API.UI.UIInternal::UpdateViewRect", &UI::setViewRect);

                API_PARAM("ref RectangleF", "viewRect");
                API_PARAM("bool", "use");
            }
            API_METHOD_END();

			API_COMMENT("Draws Texture2D.");
			API_METHOD(INTERNAL, STATIC, "DrawTexture2D", EXTERN);
			{
				API_BIND("ReCrafted.API.UI.UIInternal::DrawTexture2D", &Internal::drawTex2d);

				API_PARAM("IntPtr", "texturePtr");
				API_PARAM("ref RectangleF", "rect");
				API_PARAM("ref RectangleF", "uvs");
			}
			API_METHOD_END();

			API_COMMENT("The current drawing color.");
			API_PROPERTY(INTERNAL, STATIC, "Color", "Color", GETSET);
			{
				API_BIND("ReCrafted.API.UI.UIInternal::Internal_Color_Get", &UI::getColor);
				API_BIND("ReCrafted.API.UI.UIInternal::Internal_Color_Set", &UI::setColor);
			}
			API_PROPERTY_END();

			API_COMMENT("Depth of current drawing ui.");
			API_PROPERTY(INTERNAL, STATIC, "float", "Depth", GETSET);
			{
				API_BIND("ReCrafted.API.UI.UIInternal::Internal_Depth_Get", &UI::getDepth);
				API_BIND("ReCrafted.API.UI.UIInternal::Internal_Depth_Set", &UI::setDepth);
			}
			API_PROPERTY_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}
