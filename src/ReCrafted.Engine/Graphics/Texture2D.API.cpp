// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Texture2D.h"
#include "Scripting/Mono.h"
#include "Scripting/Bindings.h"

namespace Internal
{
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

	MonoObject* createTexture2D()
	{
		return Object::createInstance<Texture2D>("ReCrafted.API.Graphics", "Texture2D")->getManagedPtr();
	}
}

void Texture2D::initRuntime()
{
	// create type binding

	API_FILE("Graphics/Texture2D.Gen.cs");
	{
		API_USING("ReCrafted.API.Mathematics");

		API_COMMENT("Texture2D class.");
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Texture2D", "Object");
		{
			API_COMMENT("Creates new Texture2D");
			API_METHOD(PUBLIC, STATIC, "Create", EXTERN);
			{
				API_BIND("ReCrafted.API.Graphics.Texture2D::Create", &Internal::createTexture2D);

				API_RETURN("Texture2D");
			}
			API_METHOD_END();

			API_COMMENT("Returns texture's width");
			API_PROPERTY(PUBLIC, REGULAR, "int", "Width", GET);
			{
				API_BIND("ReCrafted.API.Graphics.Texture2D::Internal_Width_Get", &Internal::getWidth);
			}
			API_PROPERTY_END();

			API_COMMENT("Returns texture's height");
			API_PROPERTY(PUBLIC, REGULAR, "int", "Height", GET);
			{
				API_BIND("ReCrafted.API.Graphics.Texture2D::Internal_Height_Get", &Internal::getHeight);
			}
			API_PROPERTY_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}