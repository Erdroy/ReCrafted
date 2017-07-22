// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Texture2D.h"
#include "Scripting/Mono.h"
#include "Scripting/Assembly.h"
#include "Scripting/Bindings.h"

namespace Internal
{
	Ptr<Texture2D> createNewTexture2D()
	{
		Ptr<Texture2D> texture(new Texture2D());
		return texture;
	}

	int getWidth(Texture2D* texture)
	{
		if (!texture) return 0;

		auto width = texture->getWidth();

		return width;
	}

	int getHeight(Texture2D* texture)
	{
		if (!texture) return 0;

		auto height = texture->getHeight();

		return height;
	}
}

void Texture2D::initRuntime()
{
	// create type binding
	BIND_OBJECT("ReCrafted.API.Graphics", "Texture2D", &Internal::createNewTexture2D);

	API_FILE("Graphics/Texture2D.Gen.cs");
	{
		API_USING("ReCrafted.API.Mathematics");

		API_COMMENT("Texture2D class.");
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Texture2D", "Object");
		{
			API_COMMENT("Returns texture's width");
			API_PROPERTY(PUBLIC, REGULAR, "int", "Width", GET)
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Width_Get", &Internal::getWidth);
			}
			API_PROPERTY_END();

			API_COMMENT("Returns texture's height");
			API_PROPERTY(PUBLIC, REGULAR, "int", "Height", GET)
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Height_Get", &Internal::getHeight);
			}
			API_PROPERTY_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}