// ReCrafted © 2016-2017 Always Too Late

#include "Platform.h"

#if _WIN32

#include <cstdio>
#include <Windows.h>

#pragma comment(lib, "Rpcrt4.lib")

void* Platform::m_gameWindow;
LARGE_INTEGER Platform::m_frequency;
double Platform::m_start;
double Platform::m_freqCoeff;
unsigned char Platform::m_theadCount;
int Platform::m_cpuCount;

File::~File()
{
	if (m_file)
		close();
}

void File::seek(long position) const
{
	auto file = static_cast<FILE*>(m_file);

	fseek(file, position, SEEK_SET);
}

void File::read(void* buffer, size_t length, size_t offset) const
{
	auto file = static_cast<FILE*>(m_file);

	if (offset > 0)
	{
		// set offset
		fseek(file, long(offset), SEEK_SET);
	}

	// read
	fread(buffer, length, 1, file);
}

void File::read(void* buffer) const
{
	read(buffer, FileSize);
}

void File::write(void* data, size_t data_lenght) const
{
	auto file = static_cast<FILE*>(m_file);

	fwrite(data, data_lenght, 1, file);
}

void File::flush() const
{
	auto file = static_cast<FILE*>(m_file);

	fflush(file);
}

void File::close()
{
	auto file = static_cast<FILE*>(m_file);

	fclose(file);
	m_file = nullptr;
}

void Platform::initialize()
{
	// initialize timer
	QueryPerformanceFrequency(&m_frequency);
	m_freqCoeff = double(m_frequency.QuadPart) / 1000.0;

	m_start = getMiliseconds();

	// get cpu count
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	m_cpuCount = sysinfo.dwNumberOfProcessors;

}

Guid Platform::newGuid()
{
	Guid guid = {};
	UUID uuid = {};

	// Create UUID
	UuidCreate(&uuid);

	// Copy data to our Guid structure
	memcpy_s(&guid, sizeof Guid, &uuid, sizeof UUID);

	return guid;
}

double Platform::getMiliseconds()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	return double(current.QuadPart) / m_freqCoeff - m_start;
}

int Platform::cpuCount()
{
	return m_cpuCount;
}

void* Platform::getGameWindow()
{
	return m_gameWindow;
}

void Platform::setGameWindow(void* gameWindow)
{
	m_gameWindow = gameWindow;
}

void Platform::getGameWindowSize(unsigned* width, unsigned* height)
{
	RECT windowRect;
	GetClientRect(static_cast<HWND>(m_gameWindow), &windowRect);

	*width = windowRect.right - windowRect.left;
	*height = windowRect.bottom - windowRect.top;
}

void Platform::setCursorPosition(uint16_t x, uint16_t y)
{
	POINT point = {};
	point.x = long(x);
	point.y = long(y);

	ClientToScreen(static_cast<HWND>(m_gameWindow), &point);
	SetCursorPos(point.x, point.y);
}

bool Platform::fileExists(const char* fileName)
{
	FILE* file;

	// check if file exists 
	// by just trying to open for read 
	// and checking the error
	auto error = fopen_s(&file, fileName, "r");

	// close the file now if exist to avoid some leaks or somethin
	if (error == 0)
		fclose(file);

	// return the value
	return error == 0;
}

void Platform::openFile(File* file, const char* fileName, OpenMode::Enum fileOpenMode)
{
	strcpy_s(file->FileName, fileName);

	// select open mode
	const char* openMode = "??";
	switch (fileOpenMode)
	{
	case OpenMode::OpenRead:
		openMode = "r";
		break;
	case OpenMode::OpenWrite:
		openMode = "w";
		break;
	case OpenMode::OpenAppend:
		openMode = "w+";
		break;
	case OpenMode::OpenReadWrite:
		openMode = "r+";
		break;
	default: break;
	}

	auto error = fopen_s(reinterpret_cast<FILE**>(&file->m_file), fileName, openMode);

	if (error != 0)
	{
		// file not found
		_ASSERT(false);
	}

	auto filep = static_cast<FILE*>(file->m_file);

	// measure file size
	fseek(filep, 0, SEEK_END);
	file->FileSize = ftell(filep);
	fseek(filep, 0, SEEK_SET);
}

void Platform::getWorkingDirectory(char* buffer)
{
	char i_buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, i_buffer, MAX_PATH);

	// remove 'ReCrafted.exe', and copy
	auto length = strlen(i_buffer);
	for (auto i = length - 13u; i < length; i++)
	{
		i_buffer[i] = '\0';
	}

	// copy
	strcpy_s(buffer, length, i_buffer);
}
#endif
