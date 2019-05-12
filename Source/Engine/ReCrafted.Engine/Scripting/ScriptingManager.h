// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Core/SubSystems/SubSystem.h"
#include "Scripting/Assembly.h"
#include "Scripting/Mono.h"
#include "Scripting/Domain.h"

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
private:
    bool m_attachDebugger = false;

    RefPtr<Assembly> m_apiAssembly;
    RefPtr<Assembly> m_gameAssembly;
    RefPtr<Assembly> m_coreAssembly;

private:
    void LoadAssemblies();
    void InitRuntime();

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
    static const RefPtr<Assembly>& GetAPIAssembly();
    static const RefPtr<Assembly>& GetGameAssembly();
    static const RefPtr<Assembly>& GetCoreAssembly();
};
