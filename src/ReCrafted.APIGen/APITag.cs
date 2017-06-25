// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

namespace ReCrafted.APIGen
{
    public abstract class APITag
    {
        public abstract void Process(string token, string[] parameters);
    }
}
/*
 #define API_BUILD

// mono extensions
#define API_DEF friend class ScriptingEngine; protected: static void initRuntime(); private:

#define API_BIND(name, method) mono_add_internal_call(name, method)

// API
#define API_FILE(file_name)
#define API_FILE_END()

// common
#define API_COMMENT(comment)

#define PUBLIC
#define INTERNAL
#define PRIVATE

#define REGULAR
#define OVERRIDE
#define STATIC
#define VIRTUAL
#define SEALED
#define ABSTRACT

// usings
#define API_USING(using_name)

// class
#define API_CLASS(access, type, namespace, class_name)
#define API_CLASS_INHERIT(access, type, namespace, class_name, inherit_class_name)
#define API_CLASS_END()

// method
#define API_METHOD(access, type, method_name)
#define API_METHOD_END()
#define API_PARAM(type, name)
#define API_PARAM_REF(type, name)
#define API_PARAM_OUT(type, name)
#define API_RETURN(type)


*/
