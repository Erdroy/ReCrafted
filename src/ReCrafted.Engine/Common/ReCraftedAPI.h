// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#define API_BUILD

// mono extensions
#define API_DEF friend class Bindings; protected: static void initRuntime(); private:

#define BIND_OBJECT(namespace, name, method) \
	auto typeId = Assembly::API->findClass(namespace, name)->getType(); \
	Bindings::bindObject(typeId, reinterpret_cast<objectinstancer>(##method##));

#define MONO_TEXT(x) mono_string_chars(string)

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