// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "UI.h"
#include "Scripting/Mono.h"

namespace Internal
{
	void drawTex2d(Texture2D* texture, Rectf* rect, Rectf* uvs)
	{
		UI::drawTexture(texture, *rect, *uvs);
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

			API_COMMENT("Draws Texture2D.");
			API_METHOD(INTERNAL, STATIC, "DrawTexture2D", EXTERN);
			{
				API_BIND("ReCrafted.API.UI.UIInternal::DrawTexture2D", &Internal::drawTex2d);

				API_PARAM("IntPtr", "texturePtr");
				API_PARAM("ref RectangleF", "rect");
				API_PARAM("ref RectangleF", "uvs");
			}
			API_METHOD_END();

			API_COMMENT("The current drawing color");
			API_PROPERTY(INTERNAL, STATIC, "Color", "Color", GETSET);
			{
				API_BIND("ReCrafted.API.UI.UIInternal::Internal_Color_Get", &UI::getColor);
				API_BIND("ReCrafted.API.UI.UIInternal::Internal_Color_Set", &UI::setColor);
			}
			API_PROPERTY_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}
