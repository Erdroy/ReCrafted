// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

// mono extensions
#define API_OBJECT friend class ScriptingEngine; protected: static void initRuntime(); private:

#define API_BIND(name, method) mono_add_internal_call(name, method)

// flags
#define ABSTRACT
#define VIRTUAL
#define OVERRIDE
#define SEALED
#define STATIC
#define INHERIT(class_name)

// API
#define API_BEGIN(file_name)
#define API_END()

// common
#define API_COMMENT(comment)
#define API_PUSH()

// class
#define API_CLASS(namespace, class_name, ...)

// method
#define API_PARAM(type, name)
#define API_PARAM_REF(type, name)
#define API_PARAM_OUT(type, name)

#define API_RETURN(type)
#define API_METHOD(method_name, ...)

// property
// TODO: properties
// TODO: enums