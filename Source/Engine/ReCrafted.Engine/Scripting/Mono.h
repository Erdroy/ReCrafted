// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/exception.h>

#define mono_object_get_type(obj)   \
    static_cast<MonoTypeEnum>(mono_type_get_type(mono_class_get_type(mono_object_get_class(obj))));

/// <summary>
/// Only valid for class types.
/// </summary>
#define mono_type_get_token(type)   \
    mono_class_get_type_token(mono_type_get_class(type))

#define MONO_CHECK_OBJECT(objInst, name)    \
    if(objInst == nullptr)                  \
        ScriptingManager::ThrowException(ExceptionType::MissingReferenceException, "The object of type '{0}' has been destroyed but you are still trying to access it.", name);\
    ASSERT(objInst);

#define API_BIND(name, method)      \
    mono_add_internal_call(name, (const void*)(method))

#define MONO_STRING_TO_CSTR(str)    \
    mono_string_to_utf8(str)

#define MONO_STRING_TO_STR(str)     \
    String((Char*)mono_string_chars(str))

#define MONO_STRING_FROM_CSTR(str)  \
    mono_string_new(mono_domain_get(), str)

#define MONO_STRING_FROM_STDSTR(str)  \
    mono_string_new(mono_domain_get(), str.c_str())

#define MONO_STRING_FROM_STR(str)  \
    mono_string_new_utf16(mono_domain_get(), reinterpret_cast<const mono_unichar2*>(str.Data()), str.Length())

#define MONO_FREE(ptr)              \
    mono_free(ptr);

#define MONO_DELEGATE_TO_ACTION_2(p, tb0, t0, t0_target, t0_conv, tb1, t1, t1_target, t1_conv) \
     Action<t0, t1>([p](t1 _t1) {           \
        auto param = t1_conv ;              \
        mono_runtime_delegate_invoke(p,     \
        reinterpret_cast<void**>(&param),   \
        nullptr);                           \
    });

#define MONO_DELEGATE_TO_ACTION_3(p, tb0, t0, t0_target, t0_conv, tb1, t1, t1_target, t1_conv, tb2, t2, t2_target, t2_conv) \
     Action<t0, t1, t2>([p](t1 _t1, t2 _t2) {           \
        struct { t1_target a; t2_target b; } params;    \
        params.a = t1_conv ;                            \
        params.b = t2_conv ;                            \
        mono_runtime_delegate_invoke(p,                 \
        reinterpret_cast<void**>(&params),              \
        nullptr);                                       \
    });

#define MONO_DELEGATE_TO_ACTION_4(p, tb0, t0, t0_target, t0_conv, tb1, t1, t1_target, t1_conv, tb2, t2, t2_target, t2_conv, tb3, t3, t3_target, t3_conv) \
     Action<t0, t1, t2, t3>([p](t1 _t1, t2 _t2, t3 _t3) {               \
        struct { t1_target a; t2_target b; t3_target c; } params;       \
        params.a = t1_conv ;                                            \
        params.b = t2_conv ;                                            \
        params.c = t3_conv ;                                            \
        mono_runtime_delegate_invoke(p,                                 \
        reinterpret_cast<void**>(&params),                              \
        nullptr);                                                       \
    });

#define MONO_FREE_STUB(value)
#define MONO_DELEGATE_FREE_STUB(value)

#define MONO_REGISTER_OBJECT(func)      \
    ObjectManager::GetInstance()->RegisterObjectCreator(                            \
        mono_reflection_type_from_name(                                             \
            const_cast<char*>(Fullname()),                                          \
            mono_assembly_get_image(ScriptingManager::GetAPIAssembly()->ToMono())   \
        ),                                                                          \
        Action<Object*, bool>::New(func)                                            \
    )

#define MONO_REGISTER_OBJECT_TYPE(type)      \
    ScriptingManager::GetInstance()->RegisterType< type >(ScriptingManager::GetAPIAssembly(), \
        mono_reflection_type_from_name(                                             \
            const_cast<char*>(Fullname()),                                          \
            mono_assembly_get_image(ScriptingManager::GetAPIAssembly()->ToMono())   \
        ))

#define MONO_ARRAY_FROM_OBJECT_ARRAY(val, tb0, t0, t0_target, t0_conv)\
    MonoArrayFromObjectArray<t0, tb0>(val);

#define MONO_ARRAY_FROM_ARRAY(val, tb0, t0, t0_target, t0_conv)\
    MonoArrayFromArray<t0, tb0>(val);

#define MONO_ARRAY_TO_OBJECT_ARRAY(val, tb0, t0, t0_target, t0_conv)\
    MonoArrayToObjectArray<t0, tb0>(val);

#define MONO_ARRAY_TO_ARRAY(val, tb0, t0, t0_target, t0_conv)\
    MonoArrayToArray<t0>(val);

#define MONO_FREE_ARRAY(arr)\
    delete arr.Data()

template<typename TType, typename TBaseType>
inline static MonoArray* MonoArrayFromObjectArray(const Array<TType>& arr)
{
    // Create array
    auto monoArray = mono_array_new(Domain::Root->ToMono(), ScriptingManager::GetClassOf<TBaseType>(), arr.Count());

    // Copy array
    for (auto i = 0; i < arr.Count(); i++)
        mono_array_set(monoArray, MonoObject*, i, arr[i]->ToManaged());

    // Return mono array
    return monoArray;
}

template<typename TType, typename TBaseType>
inline static MonoArray* MonoArrayFromArray(const Array<TType>& arr)
{
    // Create array
    auto monoArray = mono_array_new(Domain::Root->ToMono(), ScriptingManager::GetClassOf<TBaseType>(), arr.Count());

    // Copy array
    const auto monoArrayPtr = mono_array_addr(monoArray, TType, 0); // TODO: Make sure that TType has the same size as the managed equivalent
    memcpy(monoArrayPtr, arr.Data(), arr.Count() * sizeof(TType));

    // Return mono array
    return monoArray;
}

template<typename TType>
inline static Array<TType> MonoArrayToArray(MonoArray* arr)
{
    const auto len = mono_array_length(arr);
    const auto monoArrayPtr = mono_array_addr(arr, TType, 0); // TODO: Make sure that TType has the same size as the managed equivalent

    // Allocate temporary data - this will be cleared later with MONO_FREE_ARRAY
    const auto nativeArrayData = new TType[len];
    auto nativeArray = Array<TType>(nativeArrayData, len);

    // Copy array
    memcpy(nativeArrayData, monoArrayPtr, len * sizeof(TType));

    return nativeArray;
}
