// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef LOGGER_H
#define LOGGER_H

// includes
#include "Platform/Platform.h"
#include "Common/ReCraftedAPI.h"
#include "Common/Text.h"
#include "fmt/format.h"

#ifndef LOGGER_MAXSIZE
#	define LOGGER_MAXSIZE 8192
#endif

/**
 * \brief Log level enum, specifies the log level.
 */
struct LogLevel
{
	enum Enum
	{
		Info = 0,
		Debug,
		Warning,
		Error,
		Fatal,

		Count
	};
};

/**
 * \brief Logger class - provides basic logging system.
 * Writes logs into log file or message event.
 */
class Logger
{
	friend class GameMain;

private:
	API_DEF

private:
	static Logger* m_instance;

private:
	File m_logFile = {};
	bool m_canWrite = false;
	
private:
	Logger() { m_instance = this; }

private:
	void init();
	void dispose();

private:
	void write(const char* message, LogLevel::Enum logLevel = LogLevel::Debug) const;
	void write(Char* message, LogLevel::Enum logLevel = LogLevel::Debug) const;

public:
	/**
	 * \brief Writes exception message into log file.
	 * \param message The message.
	 */
	FORCEINLINE static void logException(const char* message)
	{
		m_instance->write(message, LogLevel::Fatal);
	}

	/**
	* \brief Writes exception message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void logException(const char* format, fmt::ArgList args)
	{
		auto string = fmt::format(format, args);
		m_instance->write(string.c_str(), LogLevel::Fatal);
	}
	FMT_VARIADIC(static void, logException, const char *)

	/**
	* \brief Writes error message into log file.
	* \param message The message.
	*/
	FORCEINLINE static void logError(const char* message)
	{
		m_instance->write(message, LogLevel::Error);
	}

	/**
	* \brief Writes error message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void logError(const char* format, fmt::ArgList args)
	{
		auto string = fmt::format(format, args);
		m_instance->write(string.c_str(), LogLevel::Error);
	}
	FMT_VARIADIC(static void, logError, const char *)

	/**
	* \brief Writes warning message into log file.
	* \param message The message.
	*/
	FORCEINLINE static void logWarning(const char* message)
	{
		m_instance->write(message, LogLevel::Warning);
	}

	/**
	* \brief Writes warning message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void logWarning(const char* format, fmt::ArgList args)
	{
		auto string = fmt::format(format, args);
		m_instance->write(string.c_str(), LogLevel::Warning);
	}
	FMT_VARIADIC(static void, logWarning, const char *)

	/**
	* \brief Writes info message into log file.
	* \param message The message.
	*/
	FORCEINLINE static void logInfo(const char* message)
	{
		m_instance->write(message, LogLevel::Info);
	}

	/**
	* \brief Writes info message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void logInfo(const char* format, fmt::ArgList args)
	{
		auto string = fmt::format(format, args);
		m_instance->write(string.c_str(), LogLevel::Info);
	}
	FMT_VARIADIC(static void, logInfo, const char *)

	/**
	* \brief Writes debug message into log file.
	* \param message The message.
	*/
	FORCEINLINE static void logDebug(const char* message)
	{
		m_instance->write(message, LogLevel::Debug);
	}

	/**
	* \brief Writes debug message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void logDebug(const char* format, fmt::ArgList args)
	{
		auto string = fmt::format(format, args);
		m_instance->write(string.c_str(), LogLevel::Debug);
	}
	FMT_VARIADIC(static void, logDebug, const char *)

public:
	/**
	 * \brief Writes message into log file using given log level.
	 * \param logLevel The log level.
	 * \param message The message.
	 */
	FORCEINLINE static void log(LogLevel::Enum logLevel, const char* message)
	{
		m_instance->write(message, logLevel);
	}

	/**
	* \brief Writes debug message into log file.
	* \param format The message format.
	* \param args The message arguments.
	*/
	FORCEINLINE static void log(LogLevel::Enum logLevel, const char* format, fmt::ArgList args)
	{
		auto string = fmt::format(format, args);
		m_instance->write(string.c_str(), logLevel);
	}
	FMT_VARIADIC(static void, log, LogLevel::Enum, const char *)

	/**
	* \brief Writes (UTF-16) message into log file using given log level.
	* \param logLevel The log level.
	* \param message The message.
	*/
	FORCEINLINE static void log(LogLevel::Enum logLevel, Char* message)
	{
		m_instance->write(message, logLevel);
	}

	/**
	* \brief Writes (UTF-16) text message into log file using given log level.
	* \param logLevel The log level.
	* \param message The message.
	*/
	FORCEINLINE static void log(LogLevel::Enum logLevel, const Text& message)
	{
		m_instance->write(message.data(), logLevel);
	}
};

#endif // LOGGER_H