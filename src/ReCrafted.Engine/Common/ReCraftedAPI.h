// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#define API_BUILD

// mono extensions
#define SCRIPTING_API_IMPL() friend class Bindings; protected: static void InitRuntime(); private:

#define MONO_TEXT(x) mono_string_chars(x)
#define MONO_ANSI_ERR() MonoError error;
#define MONO_ANSI(x) mono_string_to_utf8_checked(x, &error)
#define MONO_ANSI_FREE(x) mono_free(x)

#define MONO_STRING_FROM_TEXT(x) mono_string_new(mono_domain_get(), x.StdStr().c_str())
#define TEXT_FROM_MONO_STRING(x) Text(MONO_TEXT(x))

#define API_REGISTER_OBJECT(fullName, createFunction) Bindings::RegisterObject(fullName, Action<Object*, bool>::New(createFunction));
#define API_DEFINE_OBJECT_CREATOR(objectNamespace, objectName, objectType) \
        Object* Create##objectType(bool createManagedInstance) {\
            return createManagedInstance ? Object::CreateInstance<objectType>(objectNamespace, objectName) : new objectType##();\
        }

#define API_BIND(name, method) mono_add_internal_call(name, method)

// API
#define API_FILE(file_name)
#define API_FILE_END()

// common
#define API_COMMENT(comment)

#define PUBLIC
#define PROTECTED
#define INTERNAL
#define PRIVATE

#define REGULAR
#define OVERRIDE
#define STATIC
#define VIRTUAL
#define SEALED
#define ABSTRACT

// flags
#define PARTIAL
#define EXTERN

#define UNSAFE

#define NOCONSTRUCTOR
#define NOPROXY

#define REF
#define OUT

#define GET
#define GETSET
#define BY_REF

// usings
#define API_USING(using_name)

// class
#define API_CLASS(access, type, namespace, class_name, ...) //, base_name, PARTIAL
#define API_CLASS_END()

// method
#define API_METHOD(access, modifier, method_name, ...) //, EXTERN
#define API_METHOD_END()
#define API_PARAM(type, name, ...) //, REF/OUT 
#define API_RETURN(type)
#define API_CODE(code)

// property
#define API_PROPERTY(access, modifier, type, name, ...)
#define API_PROPERTY_END()
