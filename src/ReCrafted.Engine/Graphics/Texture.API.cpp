// ReCrafted (c) 2016-2018 Always Too Late

#include "Texture.h"
#include "Scripting/Mono.h"
#include "Scripting/Bindings.h"
#include "Content/ContentManager.h"

namespace Internal
{
    int getWidth(Texture* texture)
    {
        if (!texture) return 0;

        auto width = texture->GetWidth();

        return width;
    }

    int getHeight(Texture* texture)
    {
        if (!texture) return 0;

        auto height = texture->GetHeight();

        return height;
    }

    void apply(Texture* texture)
    {
        if (!texture) return;

        texture->Apply();
    }

    void loadFile(Texture* texture, MonoString* string)
    {
        if (!texture) return;

        // convert monostring to ansi string
        MONO_ANSI_ERR();
        cvar str = MONO_ANSI(string);

        // load from file
        texture->InitializeFromFile(str);

        // free ansi string
        MONO_ANSI_FREE(str);
    }

    MonoObject* createTexture()
    {
        cvar texture = Object::CreateAssetInstance<Texture>("ReCrafted.API.Graphics", "Texture");
        return texture->GetManagedPtr();
    }
}

void Texture::InitRuntime()
{
    // create type binding

    API_FILE("Graphics/Texture.Gen.cs");
    {
        API_COMMENT("Texture class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Texture", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            API_METHOD(INTERNAL, STATIC, "InternalLoadFile", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.Texture::InternalLoadFile", &Internal::loadFile);

                API_PARAM("IntPtr", "nativePtr");
                API_PARAM("string", "fileName");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, STATIC, "InternalApply", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.Texture::InternalApply", &Internal::apply);

                API_PARAM("IntPtr", "nativePtr");
            }
            API_METHOD_END();

            API_COMMENT("Creates new Texture");
            API_METHOD(PUBLIC, STATIC, "Create", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.Texture::Create", &Internal::createTexture);

                API_RETURN("Texture");
            }
            API_METHOD_END();

            API_COMMENT("Returns texture's width");
            API_PROPERTY(PUBLIC, REGULAR, "int", "Width", GET);
            {
                API_BIND("ReCrafted.API.Graphics.Texture::Internal_Width_Get", &Internal::getWidth);
            }
            API_PROPERTY_END();

            API_COMMENT("Returns texture's height");
            API_PROPERTY(PUBLIC, REGULAR, "int", "Height", GET);
            {
                API_BIND("ReCrafted.API.Graphics.Texture::Internal_Height_Get", &Internal::getHeight);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
