// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Camera.h"
#include "Scripting/Mono.h"
#include "Scripting/Assembly.h"
#include "Scripting/Bindings.h"

namespace Internal
{
	void setAsCurrent(Camera* camera)
	{
		if (!camera)
			return;

		camera->setAsCurrent();
	}

	MonoObject* getCurrent()
	{
		auto camera = Camera::getMainCamera();

		if (!camera)
			return nullptr;

		return camera->getManagedPtr();
	}

	void setFov(Camera* camera, float value)
	{
		if (!camera)
			return;

		camera->set_fov(value);
	}

	float getFov(Camera* camera)
	{
		if (!camera)
			return 0.0f;

		return camera->get_fov();
	}

	Ptr<Camera> createNewCamera()
	{
		Ptr<Camera> camera(new Camera());
		return camera;
	}
}

void Camera::initRuntime()
{
	// create type binding
	BIND_OBJECT("ReCrafted.API.Graphics", "Camera", &Internal::createNewCamera);

	API_FILE("Graphics/Camera.Gen.cs");
	{
		API_COMMENT("Camera class.");
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Camera", "Object");
		{
			API_COMMENT("Sets the camera as current.");
			API_METHOD(PUBLIC, REGULAR, "SetAsCurrent", EXTERN);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_SetAsCurrent", &Internal::setAsCurrent);
			}
			API_METHOD_END();

			API_COMMENT("Camera's Field of view.");
			API_PROPERTY(PUBLIC, REGULAR, "float", "Fov", GETSET);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Fov_Get", &Internal::getFov);
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Fov_Set", &Internal::setFov);
			}
			API_PROPERTY_END();

			API_COMMENT("The current camera.");
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