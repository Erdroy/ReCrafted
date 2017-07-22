// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Font.h"
#include "Scripting/Mono.h"
#include "Scripting/Assembly.h"
#include "Scripting/Bindings.h"

namespace Internal
{
	Ptr<Font> createFont(int glyphCount)
	{
		Ptr<Font>font(new Font(glyphCount));

		return font;
	}
}

void Font::initRuntime()
{
	// create type binding
	BIND_OBJECT("ReCrafted.API.Graphics", "Font", &Internal::createFont);

	API_FILE("Graphics/Font.Gen.cs");
	{
		API_USING("ReCrafted.API.Mathematics");

		API_COMMENT("Font class.");
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Font", "Object");
		{

		}
		API_CLASS_END();
	}
	API_FILE_END();
}