// ReCrafted (c) 2016-2019 Always Too Late

#include "Font.h"
#include "Scripting/Bindings.h"

namespace Internal
{
    MonoObject* loadFont(MonoString* fileName, int fontSize)
    {
        auto text = Text::Constant(MONO_TEXT(fileName));
        auto font = Object::CreateInstance<Font>("ReCrafted.API.Graphics", "Font");;

        font->LoadFont(text, fontSize, true);

        return font->GetManagedPtr();
    }

    void fontMeasure(Font* font, MonoString* str, Vector2* size)
    {
        auto text = Text::Constant(MONO_TEXT(str));

        if (text.Size() == 0)
            return;

        *size = font->MeasureText(text);
    }

    uint getFontSize(Font* font)
    {
        return font->GetSize();
    }
}

void Font::InitRuntime()
{
    // create type binding

    API_FILE("Graphics/Font.Gen.cs");
    {
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("Font class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Font", "Object", PARTIAL);
        {
            API_COMMENT("Loads font from file");
            API_METHOD(PUBLIC, STATIC, "Load", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.Font::Load", &Internal::loadFont);

                API_PARAM("string", "fileName");
                API_PARAM("int", "fontSize");

                API_RETURN("Font");
            }
            API_METHOD_END();

            API_COMMENT("Measures size of given string.");
            API_METHOD(INTERNAL, STATIC, "Internal_MeasureString", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.Font::Internal_MeasureString", &Internal::fontMeasure);

                API_PARAM("IntPtr", "nativePtr");
                API_PARAM("string", "text");
                API_PARAM("out Vector2", "size");

                API_RETURN("uint");
            }
            API_METHOD_END();

            API_COMMENT("The size of this font.");
            API_PROPERTY(PUBLIC, REGULAR, "uint", "Size", GETSET);
            {
                API_BIND("ReCrafted.API.Graphics.Font::Internal_Size_Get", &Internal::getFontSize);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
