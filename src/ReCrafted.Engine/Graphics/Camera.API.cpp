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

	void setPos(Camera* camera, Vector3* pos)
	{
		if (!camera)
			return;

		camera->set_position(*pos);
	}

	void getPos(Camera* camera, Vector3* pos)
	{
		if (!camera)
			return;

		*pos = camera->get_position();
	}

	Ptr<Camera> createNewCamera()
	{
		Ptr<Camera> camera(new Camera());
		return camera;
	}

	void getBoundingFrustum(Camera* camera, BoundingFrustum* frustum)
	{
		if (!camera) return;

		*frustum = camera->getBoundingFrustum();
	}

	bool getFreeMov(Camera* camera)
	{
		if (!camera) return false;

		return camera->get_freeMovement();
	}

	void setFreeMov(Camera* camera, bool mov)
	{
		if (!camera) return;

		camera->set_freeMovement(mov);
	}

	float getFarPlane(Camera* camera)
	{
		if (!camera) return 0.0f;

		return camera->get_farPlane();
	}

	void setFarPlane(Camera* camera, float farplane)
	{
		if (!camera) return;

		camera->set_farPlane(farplane);
	}

	float getNearPlane(Camera* camera)
	{
		if (!camera) return 0.0f;

		return camera->get_nearPlane();
	}

	void setNearPlane(Camera* camera, float nearplane)
	{
		if (!camera) return;

		camera->set_nearPlane(nearplane);
	}

	void getForward(Camera* camera, Vector3* forward)
	{
		if (!camera) return;

		*forward = camera->get_forward();
	}

	void setForward(Camera* camera, Vector3* forward)
	{
		if (!camera) return;

		camera->set_forward(*forward);
	}

	void getUp(Camera* camera, Vector3* up)
	{
		if (!camera) return;

		*up = camera->get_up();
	}

	void setUp(Camera* camera, Vector3* up)
	{
		if (!camera) return;

		camera->set_up(*up);
	}

	void getRight(Camera* camera, Vector3* right)
	{
		if (!camera) return;

		*right = camera->get_right();
	}

	void setRight(Camera* camera, Vector3* right)
	{
		if (!camera) return;

		camera->set_right(*right);
	}
}

void Camera::initRuntime()
{
	// create type binding
	BIND_OBJECT("ReCrafted.API.Graphics", "Camera", &Internal::createNewCamera);

	API_FILE("Graphics/Camera.Gen.cs");
	{
		API_USING("ReCrafted.API.Core");

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

			API_COMMENT("Camera's position.");
			API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Position", GETSET, BY_REF);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Position_Get", &Internal::getPos);
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Position_Set", &Internal::setPos);
			}
			API_PROPERTY_END();

			API_COMMENT("Camera's bounding frustum");
			API_PROPERTY(PUBLIC, REGULAR, "BoundingFrustum", "GetBoundingFrustum", GET, BY_REF);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_GetBoundingFrustum_Get", &Internal::getBoundingFrustum);
			}
			API_PROPERTY_END();

			API_COMMENT("Is camera's free movement enabled?");
			API_PROPERTY(PUBLIC, REGULAR, "bool", "FreeMovement", GETSET);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_FreeMovement_Get", &Internal::getFreeMov);
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_FreeMovement_Set", &Internal::setFreeMov);
			}
			API_PROPERTY_END();

			API_COMMENT("Far camera plane");
			API_PROPERTY(PUBLIC, REGULAR, "float", "FarPlane", GETSET);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_FarPlane_Get", &Internal::getFarPlane);
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_FarPlane_Set", &Internal::setFarPlane);
			}
			API_PROPERTY_END();

			API_COMMENT("Near camera plane");
			API_PROPERTY(PUBLIC, REGULAR, "float", "NearPlane", GETSET);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_NearPlane_Get", &Internal::getNearPlane);
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_NearPlane_Set", &Internal::setNearPlane);
			}
			API_PROPERTY_END();

			API_COMMENT("Camera's Z positive axis");
			API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Forward", GETSET, BY_REF);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Forward_Get", &Internal::getForward);
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Forward_Set", &Internal::setForward);
			}
			API_PROPERTY_END();

			API_COMMENT("Camera's Y positive axis");
			API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Up", GETSET, BY_REF);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Up_Get", &Internal::getUp);
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Up_Set", &Internal::setUp);
			}
			API_PROPERTY_END();

			API_COMMENT("Camera's X positive axis");
			API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Right", GETSET, BY_REF);
			{
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Right_Get", &Internal::getRight);
				API_BIND("ReCrafted.API.Graphics.Camera::Internal_Right_Set", &Internal::setRight);
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