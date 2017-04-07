// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef LOGGER_H
#define LOGGER_H

// includes
#include "../recraftedPrerequisites.h"
#include "../Platform/Platform.h"

#ifndef LOGGER_MAXSIZE
	#define LOGGER_MAXSIZE 8192
#endif

/// <summary>
/// Log level enum, specifies the log level.
/// </summary>
struct LogLevel
{
	enum Enum
	{
		Info,
		Debug,
		Warning,
		Error,
		Fatal,

		Count
	};
};

/// <summary>
/// Logger class.
/// </summary>
class Logger
{
	friend class GameMain;

private:
	static Logger* m_instance;

private:
	File m_logFile = {};
	
private:
	Logger() { m_instance = this; }

private:
	void init();
	void dispose();

public:
	/// <summary>
	/// Write into the log.
	/// Joins all message parts.
	/// </summary>
	/// <param name="messageA">The first message part.</param>
	/// <param name="messageB">The second message part.</param>
	/// <param name="messageC">The third message part.</param>
	/// <param name="logLevel">The log level.</param>
	FORCEINLINE static void write(const char* messageA, const char* messageB, const char* messageC, LogLevel::Enum logLevel = LogLevel::Debug)
	{
#ifndef _DEBUG
		if (logLevel == LogLevel::Debug)
			return;
#endif
		static char buffer[LOGGER_MAXSIZE] = {};

		strcpy_s(buffer, messageA);
		strcat_s(buffer, messageB);
		strcat_s(buffer, messageC);

		write(buffer, logLevel);
	}

	/// <summary>
	/// Write into the log.
	/// Joins all message parts.
	/// </summary>
	/// <param name="messageA">The first message part.</param>
	/// <param name="messageB">The second message part.</param>
	/// <param name="logLevel">The log level.</param>
	FORCEINLINE static void write(const char* messageA, const char* messageB, LogLevel::Enum logLevel = LogLevel::Debug)
	{
		write(messageA, messageB, "", logLevel);
	}

	/// <summary>
	/// Write into the log.
	/// </summary>
	/// <param name="logLevel">The log level.</param>
	FORCEINLINE static void write(const char* message, LogLevel::Enum logLevel = LogLevel::Debug)
	{
#ifndef _DEBUG
		if (logLevel == LogLevel::Debug)
			return;
#endif

		static char buffer[LOGGER_MAXSIZE] = {};
		static char timeBuffer[64] = {};
		buffer[0] = '\0';
		timeBuffer[0] = '\0';

		// create date
		time_t rawtime;
		time(&rawtime);
		struct tm timeinfo;
		localtime_s(&timeinfo, &rawtime);
		strftime(reinterpret_cast<char*>(timeBuffer), 64, "%T ", &timeinfo);

		// write date
		strcat_s(buffer, timeBuffer);

		switch(logLevel)
		{
		case LogLevel::Info: strcat_s(buffer, "[INFO] "); break;
		case LogLevel::Debug: strcat_s(buffer, "[DEBUG] "); break;
		case LogLevel::Warning: strcat_s(buffer, "[WARNING] "); break;
		case LogLevel::Error: strcat_s(buffer, "[ERROR] "); break;
		case LogLevel::Fatal: strcat_s(buffer, "[FATAL] "); break;
		case LogLevel::Count: break;
		default: break;
		}

		strcat_s(buffer, message);
		strcat_s(buffer, "\n");

		m_instance->m_logFile.write(static_cast<void*>(buffer), size_t(strlen(buffer)));
		m_instance->m_logFile.flush();
	}
};

#endif // LOGGER_H