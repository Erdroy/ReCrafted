// ReCrafted (c) 2016-2018 Always Too Late

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

    void apply(Texture2D* texture)
    {
        if (!texture) return;

        texture->apply();
    }

    void loadFile(Texture2D* texture, MonoString* string)
    {
        if (!texture) return;

        // convert monostring to ansi string
        MONO_ANSI_ERR();
        auto str = MONO_ANSI(string);

        // load from file
        texture->loadFile(str);

        // free ansi string
        MONO_ANSI_FREE(str);
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
        API_COMMENT("Texture2D class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Texture2D", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            API_METHOD(INTERNAL, STATIC, "InternalLoadFile", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.Texture2D::InternalLoadFile", &Internal::loadFile);

                API_PARAM("IntPtr", "nativePtr");
                API_PARAM("string", "fileName");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, STATIC, "InternalApply", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.Texture2D::InternalApply", &Internal::apply);

                API_PARAM("IntPtr", "nativePtr");
            }
            API_METHOD_END();

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
