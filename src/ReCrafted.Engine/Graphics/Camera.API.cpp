// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Camera.h"
#include "Scripting/Mono.h"

namespace Internal
{
	void setAsCurrent(Camera* camera)
	{
		throw;
	}

	MonoObject* getCurrent()
	{
		auto camera = Camera::getMainCamera();

		if (!camera)
			return nullptr;

		return camera->getManagedPtr();
	}
}

void Camera::initRuntime()
{
	API_FILE("Graphics/Camera.Gen.cs");
	{
		API_COMMENT("Camera class.");
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Camera", "Object");
		{
			API_COMMENT("Sets the camera as current.");
			API_METHOD(PUBLIC, REGULAR, "SetAsCurrent", EXTERN);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::SetAsCurrent", &Internal::setAsCurrent);
			}
			API_METHOD_END();

			API_COMMENT("Is game running state.");
			API_PROPERTY(PUBLIC, STATIC, "Camera", "Current", GET);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Current_Get", &Internal::getCurrent);
			}
			API_PROPERTY_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}
