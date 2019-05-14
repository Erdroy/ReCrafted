// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Core/SubSystems/SubSystem.h"
#include "Scripting/Assembly.h"
#include "Scripting/Mono.h"
#include "Scripting/Domain.h"

#include <sparsepp/spp.h>

enum class ExceptionType : uint8_t
{
    Exception,
    MissingReferenceException,
    ReCraftedException,

    Count
};

/// <summary>
///     ScriptingManager class. Implements Mono backend for engine scripting.
/// </summary>
class ScriptingManager final : public SubSystem<ScriptingManager>
{
public:
    struct TypeInfo
    {
        MonoClass* typeClass = nullptr;
        MonoImage* typeAsmImage = nullptr;
    };

private:
    bool m_attachDebugger = false;

    RefPtr<Assembly> m_apiAssembly;
    RefPtr<Assembly> m_gameAssembly;
    RefPtr<Assembly> m_coreAssembly;

    spp::sparse_hash_map<size_t, TypeInfo> m_classMap;

private:
    void LoadAssemblies();
    void InitRuntime();
    void InitBuiltinTypes();

public:
    template<typename TType>
    void RegisterType(const RefPtr<Assembly>& assembly, const char* _name, const char* _namespace)
    {
        const auto asmImage = assembly->ToMonoImage();
        const auto typeClass = mono_class_from_name(asmImage, _namespace, _name);
        m_classMap.insert(std::make_pair(typeid(TType).hash_code(), TypeInfo{ typeClass, asmImage }));
    }

    template<typename TType>
    void RegisterType(const RefPtr<Assembly>& assembly, MonoType* type)
    {
        const auto asmImage = assembly->ToMonoImage();
        const auto typeClass = mono_type_get_class(type);
        m_classMap.insert(std::make_pair(typeid(TType).hash_code(), TypeInfo{ typeClass, asmImage }));
    }

    TypeInfo GetTypeName(size_t hash);

protected:
    void Initialize() override;
    void Shutdown() override;

private:
    static void InternalThrowException(ExceptionType type, bool unhandled, const std::basic_string<char>& message);

public:
    template<typename... TArgs>
    static void ThrowException(ExceptionType type, const char* format, const TArgs& ... args)
    {
        InternalThrowException(type, false, fmt::vformat(format, fmt::make_format_args(args...)));
    }

    template<typename... TArgs>
    static void ThrowUnhandledException(ExceptionType type, const char* format, const TArgs& ... args)
    {
        InternalThrowException(type, true, fmt::vformat(format, fmt::make_format_args(args...)));
    }

public:
    /// <summary>
    ///     Gets mono class of the given type.
    /// </summary>
    template<class TType>
    static MonoClass* GetClassOf(MonoImage** image = nullptr)
    {
        const auto typeHash = typeid(TType).hash_code();
        const auto typeInfo = GetInstance()->GetTypeName(typeHash);
        ASSERT(typeInfo.typeAsmImage);
        if(image)
            *image = typeInfo.typeAsmImage;
        return typeInfo.typeClass;
    }

public:
    static const RefPtr<Assembly>& GetAPIAssembly();
    static const RefPtr<Assembly>& GetGameAssembly();
    static const RefPtr<Assembly>& GetCoreAssembly();
};
