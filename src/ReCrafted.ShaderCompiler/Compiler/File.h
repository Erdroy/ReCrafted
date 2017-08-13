#pragma once

#ifndef FILE_H
#define FILE_H

#include <cstdio>
#include <crtdbg.h>

#include <string.h>

/// <summary>
/// OpenMode
/// </summary>
struct OpenMode
{
	enum Enum
	{
		OpenRead,
		OpenWrite,
		OpenAppend,

		OpenReadWrite
	};
};

/// <summary>
/// File structure
/// </summary>
struct File
{
	friend class Platform;

private:
	FILE* m_file = nullptr;

public:
	char FileName[512];
	size_t FileSize = 0u;
	size_t ReadPosition = 0u;

	~File()
	{
		// out of scope, close the file
		if (m_file)
			close();
	}

	void seek(long position) const
	{
		fseek(m_file, position, SEEK_SET);
	}

	void read(void* buffer, size_t length, size_t offset = 0)
	{
		if(offset > 0)
		{
			// set offset
			fseek(m_file, long(offset), SEEK_SET);
		}

		ReadPosition += length;

		// read
		fread(buffer, 1, length, m_file);
	}

	void read(void* buffer)
	{
		read(buffer, FileSize);
	}

	void write(void* data, size_t data_lenght) const
	{
		fwrite(data, 1, data_lenght, m_file);
	}
	
	void write_string(std::string string)
	{
		write_string(string.c_str(), (int)string.length());
	}

	void write_string(const char* string, int len)
	{
		write(&len, sizeof(int));
		write((void*)string, len);
	}

	std::string read_string()
	{
		int length = 0;
		read(&length, sizeof(int));

		std::string str(length, '\0');

		read(&str[0], length);

		return str;
	}

	template<class T>
	void write(T data)
	{
		write(&data, sizeof T);
	}

	template<class T>
	T read()
	{
		T tmp = {};
		read(&tmp, sizeof T);
		return tmp;
	}

	void flush() const
	{
		fflush(m_file);
	}

	void close()
	{
		fclose(m_file);
		m_file = nullptr;
	}

public:
	static bool fileExists(const char* fileName)
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

	static void openFile(File* file, const char* fileName, OpenMode::Enum fileOpenMode = OpenMode::OpenReadWrite)
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

		auto error = fopen_s(&file->m_file, fileName, openMode);

		if (error != 0)
		{
			// file not found
			_ASSERT(false);
		}

		// measure file size
		fseek(file->m_file, 0, SEEK_END);
		file->FileSize = ftell(file->m_file);
		fseek(file->m_file, 0, SEEK_SET);
	}
};

#endif // FILE_H
