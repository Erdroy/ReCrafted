// ReCrafted (c) 2016-2018 Always Too Late

#include "CharacterActor.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    int MoveCharacter(CharacterActor* actor, Vector3* displacement)
    {
        ASSERT(actor);
        return static_cast<int>(actor->Move(*displacement));
    }

    float GetContactOffset(CharacterActor* actor)
    {
        ASSERT(actor);
        return actor->GetContactOffset();
    }

    void SetContactOffset(CharacterActor* actor, const float contactOffset)
    {
        ASSERT(actor);
        actor->SetContactOffset(contactOffset);
    }

    float GetSlopeLimit(CharacterActor* actor)
    {
        ASSERT(actor);
        return actor->GetSlopeLimit();
    }

    void SetSlopeLimit(CharacterActor* actor, const float slopeLimit)
    {
        ASSERT(actor);
        actor->SetSlopeLimit(slopeLimit);
    }

    float GetStepOffset(CharacterActor* actor)
    {
        ASSERT(actor);
        return actor->GetStepOffset();
    }

    void SetStepOffset(CharacterActor* actor, const float stepOffset)
    {
        ASSERT(actor);
        actor->SetStepOffset(stepOffset);
    }

    float GetRadius(CharacterActor* actor)
    {
        ASSERT(actor);
        return actor->GetRadius();
    }

    void SetRadius(CharacterActor* actor, const float radius)
    {
        ASSERT(actor);
        actor->SetRadius(radius);
    }

    float GetHeight(CharacterActor* actor)
    {
        ASSERT(actor);
        return actor->GetHeight();
    }

    void SetHeight(CharacterActor* actor, const float height)
    {
        ASSERT(actor);
        actor->SetHeight(height);
    }

    void GetUpDirection(CharacterActor* actor, Vector3* direction)
    {
        ASSERT(actor);
        *direction = actor->GetUpDirection();
    }

    void SetUpDirection(CharacterActor* actor, Vector3* direction)
    {
        ASSERT(actor);
        actor->SetUpDirection(*direction);
    }

    void GetCharacterVelocity(CharacterActor* actor, Vector3* velocity)
    {
        ASSERT(actor);
        *velocity = actor->GetVelocity();
    }

    bool GetIsGrounded(CharacterActor* actor)
    {
        ASSERT(actor);
        return actor->IsGrounded();
    }

    Object* CreateCharacter(bool createManagedInstance)
    {
        return CharacterActor::Create();
    }
}

const char* Actor<CharacterActor>::ManagedName = "CharacterActor";
const char* Actor<CharacterActor>::ManagedNamespace = "ReCrafted.API.Physics";

void CharacterActor::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Physics.CharacterActor", &Internal::CreateCharacter);

    API_FILE("Physics/CharacterActor.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");
        API_USING("ReCrafted.API.Common.Actors");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("CharacterActor actor");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Physics", "CharacterActor", "ActorBase", PARTIAL, NOCONSTRUCTOR);
        {
            API_METHOD(INTERNAL, REGULAR, "MoveCharacter", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_MoveCharacter", &Internal::MoveCharacter);

                API_PARAM("ref Vector3", "displacement");
                API_RETURN("CharacterCollisionFlags");
            }
            API_METHOD_END();

            API_COMMENT("Gets or sets controller's contact offset.");
            API_PROPERTY(PUBLIC, REGULAR, "float", "ContactOffset", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_ContactOffset_Get", &Internal::GetContactOffset);
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_ContactOffset_Set", &Internal::SetContactOffset);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets the slope limit.");
            API_PROPERTY(PUBLIC, REGULAR, "float", "SlopeLimit", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_SlopeLimit_Get", &Internal::GetSlopeLimit);
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_SlopeLimit_Set", &Internal::SetSlopeLimit);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets the step offset.");
            API_PROPERTY(PUBLIC, REGULAR, "float", "StepOffset", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_StepOffset_Get", &Internal::GetStepOffset);
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_StepOffset_Set", &Internal::SetStepOffset);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets controller's height.");
            API_PROPERTY(PUBLIC, REGULAR, "float", "Height", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_Height_Get", &Internal::GetHeight);
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_Height_Set", &Internal::SetHeight);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets controller's radius.");
            API_PROPERTY(PUBLIC, REGULAR, "float", "Radius", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_Radius_Get", &Internal::GetRadius);
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_Radius_Set", &Internal::SetRadius);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets controller's up direction.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "UpDirection", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_UpDirection_Get", &Internal::GetUpDirection);
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_UpDirection_Set", &Internal::SetUpDirection);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets controller's current velocity.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Velocity", GET, BY_REF);
            {
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_Velocity_Get", &Internal::GetCharacterVelocity);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets controller's grounded flag from last Move call.");
            API_PROPERTY(PUBLIC, REGULAR, "bool", "IsGrounded", GET);
            {
                API_BIND("ReCrafted.API.Physics.CharacterActor::Internal_IsGrounded_Get", &Internal::GetIsGrounded);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
