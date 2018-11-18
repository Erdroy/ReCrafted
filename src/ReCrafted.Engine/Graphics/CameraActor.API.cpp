// ReCrafted (c) 2016-2018 Always Too Late

#include "CameraActor.h"

namespace Internal
{
    MonoObject* GetManagedCamera(CameraActor* actor)
    {
        ASSERT(actor);
        ASSERT(actor->GetCamera());
        return actor->GetCamera()->GetManagedPtr();
    }

    Object* CreateCameraActor(bool createManagedInstance)
    {
        return CameraActor::Create();
    }
}

const char* Actor<CameraActor>::ManagedName = "CameraActor";
const char* Actor<CameraActor>::ManagedNamespace = "ReCrafted.API.Graphics";

void CameraActor::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Graphics.CameraActor", &Internal::CreateCameraActor);

    API_FILE("Graphics/CameraActor.Gen.cs");
    {
        API_USING("ReCrafted.API.Common.Actors");
        API_USING("ReCrafted.API.Graphics");

        API_COMMENT("CameraActor actor.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "CameraActor", "ActorBase", PARTIAL, NOCONSTRUCTOR);
        {
            API_COMMENT("The camera instance used by this actor.");
            API_PROPERTY(PUBLIC, REGULAR, "Camera", "Camera", GET);
            {
                API_BIND("ReCrafted.API.Graphics.CameraActor::Internal_Camera_Get", &Internal::GetManagedCamera);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
