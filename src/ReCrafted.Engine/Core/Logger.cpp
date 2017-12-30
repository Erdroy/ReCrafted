// ReCrafted © 2016-2017 Always Too Late

#include "Logger.h"
#include "Scripting/Method.h"
#include "Scripting/Object.h"

Logger* Logger::m_instance;

void Logger::init()
{
	/*if(!GameInfo::containsArgument(TEXT("-nolog")))
	{
		m_canWrite = true;

		// open log file
		Platform::openFile(&m_logFile, "log.txt", OpenMode::OpenWrite);
	}*/
}

void Logger::postInit()
{
    m_api_log_callback = Object::findStaticMethod("ReCrafted.API.Core.Logger::OnEngineLog");
    m_api_log_shutdown= Object::findStaticMethod("ReCrafted.API.Core.Logger::Shutdown");
}

void Logger::dispose()
{
	/*if(m_canWrite)
	{
		// close log file
		m_logFile.close();
	}*/
    m_api_log_shutdown->invokeStatic();

	delete this;
}
/*
void Logger::write(const char* message, LogLevel::Enum logLevel) const
{
	if (!m_canWrite)
		return;

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

	switch (logLevel)
	{
	case LogLevel::Info:	strcat_s(buffer, "[INFO] "); break;
	case LogLevel::Debug:	strcat_s(buffer, "[DEBUG] "); break;
	case LogLevel::Warning: strcat_s(buffer, "[WARNING] "); break;
	case LogLevel::Error:	strcat_s(buffer, "[ERROR] "); break;
	case LogLevel::Fatal:	strcat_s(buffer, "[FATAL] "); break;
	case LogLevel::Count: break;
	default: break;
	}

	strcat_s(buffer, message);
	strcat_s(buffer, "\n");

    cvar stringLength = size_t(strlen(buffer));

	m_logFile.write(static_cast<void*>(buffer), stringLength);
	m_logFile.flush();
}

void Logger::write(Char* message, LogLevel::Enum logLevel) const
{
	if (!m_canWrite)
		return;

	std::wstring_convert<std::codecvt_utf8_utf16<__int16>, __int16> conversion;
	auto data = conversion.to_bytes(reinterpret_cast<__int16*>(message));
	write(data.data(), logLevel);
}*/

void Logger::invokeCallback(const char* message, LogLevel::Enum logLevel)
{
    if (m_api_log_callback)
    {
        cvar string = mono_string_new(mono_domain_get(), message);
        void* params[] = { string, &logLevel };
        m_api_log_callback->invokeStatic(params);
    }
}
