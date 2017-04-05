// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#ifndef R3D_FILESYSTEM_H
#define R3D_FILESYSTEM_H

// includes
#include <cstdio>

class r3d_filesystem
{
public:
	static bool openFile(const char* filename, FILE** file, bool read = false)
	{
		return fopen_s(file, filename, read ? "r" : "w") == 0;
	}

	static void closeFile(FILE** file)
	{
		fclose(*file);
	}

	static void read(FILE** file, void* buffer, size_t size)
	{
		fread(buffer, size, 1u, *file);
	}

	static void write(FILE** file, void* buffer, size_t size)
	{
		fwrite(buffer, size, 1u, *file);
	}
};

#endif // R3D_FILESYSTEM_H
