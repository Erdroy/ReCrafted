// ReCrafted (c) 2016-2018 Always Too Late

#include "Platform.h"
#include "ReCrafted.h"
#include "Common/Profiler/Profiler.h"

#if _WIN32

File::~File()
{
    if (m_file && AutoClose)
    {
        Close();
    }
}

void File::Seek(long position) const
{
    auto file = static_cast<FILE*>(m_file);

    fseek(file, position, SEEK_SET);
}

void File::Read(void* buffer, size_t length, size_t offset) const
{
    ASSERT(m_file);

    auto file = static_cast<FILE*>(m_file);

    if (offset > 0)
    {
        // set offset
        fseek(file, long(offset), SEEK_SET);
    }

    // read
    fread(buffer, 1, length, file);
}

void File::Read(void* buffer) const
{
    Read(buffer, FileSize);
}

void File::Write(void* data, const size_t dataLenght) const
{
    cvar file = static_cast<FILE*>(m_file);

    fwrite(data, dataLenght, 1, file);
}

void File::Flush() const
{
    cvar file = static_cast<FILE*>(m_file);

    fflush(file);
}

void File::Close() const
{
    cvar file = static_cast<FILE*>(m_file);

    fclose(file);
}
#endif
