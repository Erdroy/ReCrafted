// ReCrafted (c) 2016-2018 Always Too Late

#include "Platform.h"
#include "ReCrafted.h"
#include "Common/Profiler/Profiler.h"

#if _WIN32

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

void File::write(void* data, const size_t dataLenght) const
{
    cvar file = static_cast<FILE*>(m_file);

    fwrite(data, dataLenght, 1, file);
}

void File::flush() const
{
    cvar file = static_cast<FILE*>(m_file);

    fflush(file);
}

void File::close() const
{
    cvar file = static_cast<FILE*>(m_file);

    fclose(file);
}
#endif
