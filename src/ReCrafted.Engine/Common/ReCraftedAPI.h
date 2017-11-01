// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#define API_BUILD

// mono extensions
#define API_DEF friend class Bindings; protected: static void initRuntime(); private:

#define MONO_TEXT(x) mono_string_chars(x)
#define MONO_ANSI_ERR() MonoError error;
#define MONO_ANSI(x) mono_string_to_utf8_checked(x, &error)
#define MONO_ANSI_FREE(x) mono_free(x)

#define MONO_STRING_FROM_TEXT(x) mono_string_new(mono_domain_get(), x.std_str().c_str())
#define TEXT_FROM_MONO_STRING(x) Text(MONO_TEXT(x))

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