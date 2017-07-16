// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Object.h"
#include "Scripting/Mono.h"
#include "Core/Logger.h"
#include "Domain.h"
#include "Graphics/Camera.h"
#include "Bindings.h"

namespace Internal
{
	MonoObject* createObject(MonoReflectionType* typeRef)
	{
		auto type = mono_reflection_type_get_type(typeRef);

		// create new using bindings
		auto object = Bindings::instantiate(mono_type_get_type(type));

		// create object
		Object::create(object, Domain::Root->getMono(), mono_type_get_class(type), true);

		return object->getManagedPtr();
	}
}

void Object::initRuntime()
{
	API_FILE("Object.Gen.cs");
	{
		API_COMMENT("Base class for all mono object instances.");
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API", "Object", PARTIAL);
		{
			API_METHOD(INTERNAL, STATIC, "InternalDestroy", EXTERN);
			{
				API_BIND("ReCrafted.API.Object::InternalDestroy", &Object::destroy);

				API_PARAM("IntPtr", "nativePtr");
			}
			API_METHOD(INTERNAL, STATIC, "InternalCreate", EXTERN);
			{
				API_BIND("ReCrafted.API.Object::InternalCreate", &Internal::createObject);

				API_PARAM("Type", "type");

				API_RETURN("Object");
			}
			API_METHOD(INTERNAL, STATIC, "InternalObjectFinalized", EXTERN);
			{
				API_BIND("ReCrafted.API.Object::InternalObjectFinalized", &Object::finalize);

				API_PARAM("IntPtr", "nativePtr");
			}
		}
		API_CLASS_END();
	}
	API_FILE_END();
}
