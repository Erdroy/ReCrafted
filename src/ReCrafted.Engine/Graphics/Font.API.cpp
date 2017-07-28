// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Font.h"
#include "Scripting/Bindings.h"

namespace Internal
{

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

		}
		API_CLASS_END();
	}
	API_FILE_END();
}