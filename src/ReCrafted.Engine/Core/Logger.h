// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef LOGGER_H
#define LOGGER_H

// includes
#include "ReCrafted.h"
#include "Common/Text.h"
#include "fmt/format.h"

/**
 * \brief Log level enum, specifies the log level.
 */
struct LogLevel
{
	enum Enum
	{
		Message = 0,
		Warning,
		Error,
		Fatal,

		Count
	};
};

class Method;

/**
 * \brief Logger class - provides basic logging system.
 * Writes logs into log file or message event.
 */
class Logger
{
	friend class GameMain;

private:
	static Logger* m_instance;

private:
    Ptr<Method> m_api_log_callback = nullptr;
    Ptr<Method> m_api_log_shutdown = nullptr;
	
private:
	Logger() { m_instance = this; }

private:
	void init();
    void postInit();
	void dispose();

private:
    void invokeCallback(const char* message, LogLevel::Enum logLevel);

public:
	/**
	 * \brief Writes exception message into log file.
	 * \param message The message.
	 */
	FORCEINLINE static void logException(const char* message)
	{
        m_instance->invokeCallback(message, LogLevel::Fatal);
	}

	/**
	* \brief Writes exception message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void logException(const char* format, fmt::ArgList args)
	{
		auto string = fmt::format(format, args);
        auto cstring = string.c_str();
        m_instance->invokeCallback(cstring, LogLevel::Fatal);
	}
	FMT_VARIADIC(static void, logException, const char *)

	/**
	* \brief Writes error message into log file.
	* \param message The message.
	*/
	FORCEINLINE static void logError(const char* message)
	{
        m_instance->invokeCallback(message, LogLevel::Error);
	}

	/**
	* \brief Writes error message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void logError(const char* format, fmt::ArgList args)
	{
        auto string = fmt::format(format, args);
        auto cstring = string.c_str();
        m_instance->invokeCallback(cstring, LogLevel::Error);
	}
	FMT_VARIADIC(static void, logError, const char *)

	/**
	* \brief Writes warning message into log file.
	* \param message The message.
	*/
	FORCEINLINE static void logWarning(const char* message)
	{
        m_instance->invokeCallback(message, LogLevel::Warning);
	}

	/**
	* \brief Writes warning message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void logWarning(const char* format, fmt::ArgList args)
	{
        auto string = fmt::format(format, args);
        auto cstring = string.c_str();
        m_instance->invokeCallback(cstring, LogLevel::Warning);
	}
	FMT_VARIADIC(static void, logWarning, const char *)

	/**
	* \brief Writes info message into log file.
	* \param message The message.
	*/
	FORCEINLINE static void logInfo(const char* message)
	{
        m_instance->invokeCallback(message, LogLevel::Message);
	}

	/**
	* \brief Writes info message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void logInfo(const char* format, fmt::ArgList args)
	{
		auto string = fmt::format(format, args);
        auto cstring = string.c_str();
        m_instance->invokeCallback(cstring, LogLevel::Message);
	}
	FMT_VARIADIC(static void, logInfo, const char *)

	/**
	* \brief Writes info message into log file.
	* \param message The message.
	*/
	FORCEINLINE static void log(const char* message)
	{
        m_instance->invokeCallback(message, LogLevel::Message);
	}

	/**
	* \brief Writes info message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void log(const char* format, fmt::ArgList args)
	{
		auto string = fmt::format(format, args);
        auto cstring = string.c_str();
        m_instance->invokeCallback(cstring, LogLevel::Message);
	}
	FMT_VARIADIC(static void, log, const char *)
};

#endif // LOGGER_H