// ReCrafted © 2016-2017 Always Too Late

#include "Font.h"
#include "Scripting/Bindings.h"

namespace Internal
{
	MonoObject* loadFont(MonoString* fileName, int fontSize)
	{
		auto text = Text::constant(MONO_TEXT(fileName));
		auto font = Object::createInstance<Font>("ReCrafted.API.Graphics", "Font");;
	
		font->loadFont(text, fontSize, true);
		
		return font->getManagedPtr();
	}

	void fontMeasure(Font* font, MonoString* str, Vector2* size)
	{
		auto text = Text::constant(MONO_TEXT(str));
		*size = font->measureText(text);
	}

	uint getFontSize(Font* font)
	{
		return font->getSize();
	}
}

void Font::initRuntime()
{
	// create type binding

	API_FILE("Graphics/Font.Gen.cs");
	{
		API_USING("ReCrafted.API.Mathematics");

		API_COMMENT("Font class.");
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Font", "Object", PARTIAL);
		{
			API_COMMENT("Loads font from file");
			API_METHOD(PUBLIC, STATIC, "Load", EXTERN);
			{
				API_BIND("ReCrafted.API.Graphics.Font::Load", &Internal::loadFont);

				API_PARAM("string", "fileName");
				API_PARAM("int", "fontSize");

				API_RETURN("Font");
			}
			API_METHOD_END();

			API_COMMENT("Get size of font.");
			API_METHOD(INTERNAL, STATIC, "Internal_GetSize", EXTERN);
			{
				API_BIND("ReCrafted.API.Graphics.Font::Internal_GetSize", &Internal::getFontSize);

				API_PARAM("IntPtr", "nativePtr");

				API_RETURN("uint");
			}
			API_METHOD_END();

			API_COMMENT("Measures size of given string.");
			API_METHOD(INTERNAL, STATIC, "Internal_MeasureString", EXTERN);
			{
				API_BIND("ReCrafted.API.Graphics.Font::Internal_MeasureString", &Internal::fontMeasure);

				API_PARAM("IntPtr", "nativePtr");
				API_PARAM("string", "text");
				API_PARAM("out Vector2", "size");

				API_RETURN("uint");
			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}