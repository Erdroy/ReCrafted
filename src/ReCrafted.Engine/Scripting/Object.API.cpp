// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Object.h"
#include "Scripting/Mono.h"

namespace Internal
{
	void destroy(void* ptr)
	{
		// TODO: queue to destroy
	}

	void onFinalized(void* ptr)
	{
		// call onFinalize
		//auto obj = static_cast<Object*>(ptr);
		//obj->onFinalize();
	}
}

void Object::initRuntime()
{
	API_FILE("Object.Gen.cs");
	{
		API_COMMENT("Base class for all mono object instances.");
		API_CLASS(PUBLIC, ABSTRACT, "ReCrafted.API", "Object", PARTIAL);
		{
			API_METHOD(INTERNAL, STATIC, "InternalDestroy", EXTERN);
			{
				API_BIND("ReCrafted.API.Object::InternalDestroy", &Internal::destroy);

				API_PARAM("IntPtr", "nativePtr");
			}
			API_METHOD(INTERNAL, STATIC, "InternalObjectFinalized", EXTERN);
			{
				API_BIND("ReCrafted.API.Object::InternalObjectFinalized", &Internal::onFinalized);

				API_PARAM("IntPtr", "nativePtr");
			}
		}
		API_CLASS_END();
	}
	API_FILE_END();
}
