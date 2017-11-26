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
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Font", "Object");
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
			API_METHOD(PUBLIC, STATIC, "GetSize", EXTERN);
			{
				API_BIND("ReCrafted.API.Graphics.Font::GetSize", &Internal::getFontSize);

				API_PARAM("IntPtr", "nativePtr");

				API_RETURN("uint");
			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}