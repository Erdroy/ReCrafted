// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "UI.h"
#include "../../Scripting/ScriptingEngine.h"

void UI::initRuntime()
{
	API_FILE("UI/UI.Gen.cs");
	{
		API_USING("ReCrafted.API.Core");

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
