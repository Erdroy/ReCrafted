// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include <locale>
#include <codecvt>

#ifndef STRING_USE_UTF16
# define STRING_USE_UTF16
#endif
#ifndef STRING_USE_FMT
//# define STRING_USE_FMT
#endif

#ifdef STRING_USE_FMT
# include "fmt/format.h"
#endif

#ifndef _UNICODE_
# define _UNICODE_(s) u ##s
#endif

#ifdef STRING_USE_UTF16
typedef unsigned short Char;
#else
typedef unsigned int Char;
#endif

#define STRING_ALLOC rc_malloc
#define STRING_REALLOC rc_realloc
#define STRING_FREE rc_free

struct String
{
private:
    Char* m_data = nullptr;
    char* m_cstrData = nullptr;
    bool m_const = false;

private:
    void AllocData(int count)
    {
        m_data = static_cast<Char*>(STRING_ALLOC(sizeof(Char) * (count + 1)));
    }

    void ReallocData(int count)
    {
        if (m_data == nullptr)
        {
            AllocData(count);
            return;
        }

        auto tmp = static_cast<Char*>(STRING_REALLOC(m_data, sizeof(Char) * (count + 1)));

        if (tmp == nullptr)
            throw;

        m_data = tmp;
    }

    void CopyData(Char* source, int count) const
    {
        auto sz = sizeof(Char) * count;
        memcpy(m_data, source, sz);
        m_data[count] = 0;
    }

    void AddData(Char* source, int count, int totalcount) const
    {
        auto sz = sizeof(Char) * count;

        auto ptr = m_data + Length();
        memcpy(ptr, source, sz);

        m_data[totalcount] = 0;
    }

private:
    explicit String(String* string)
    {
        auto len = string->Length();

        AllocData(len);
        CopyData(string->m_data, len);
    }

    explicit String(const String* string)
    {
        auto len = string->Length();

        AllocData(len);
        CopyData(string->m_data, len);
    }

    explicit String(const String* string, int start, int count)
    {
        AllocData(count);
        auto ptr = string->m_data + start;
        CopyData(ptr, count);
    }

public:
    /**
     * \brief Default constructor
     */
    String() = default;

    /**
     * \brief Constructor of string that copies the other string.
     * \param string The string that will be copied.
     */
    String(const String& string)
    {
        if (string.m_const)
        {
            m_data = string.m_data;
            m_const = true;
            return;
        }

        const auto length = string.Length();
        AllocData(length);
        CopyData(string.m_data, length);
        m_cstrData = string.m_cstrData;
        m_const = string.m_const;
    }

    /**
     * \brief Constructor of string that is using const wide cstring.
     * \param string The wide cstring.
     */
    explicit String(Char* string)
    {
        auto length = 0;
        while (string[length] != 0)
        {
            length++;
        }

        AllocData(length);
        CopyData(string, length);
    }

    /**
     * \brief Constructor of string that is using const wide cstring.
     * \param string The cstring.
     */
    explicit String(const char* string)
    {
        auto length = 0;
        while (string[length] != 0)
        {
            length++;
        }

        AllocData(length);

        for (auto i = 0; i < length; i++)
        {
            m_data[i] = static_cast<Char>(string[i]);
        }
        m_data[length] = 0;
    }

    /**
     * \briefDefault destructor.
     */
    ~String()
    {
        if (!m_const)
        {
            if (m_data)
            {
                STRING_FREE(m_data);
                m_data = nullptr;
            }

            if (m_cstrData)
            {
                STRING_FREE(m_cstrData);
                m_cstrData = nullptr;
            }
        }
    }

public:
    /**
     * \brief Appends string to this string.
     * \param string The string to Append.
     */
    void Append(const String& string)
    {
        if (m_const)
            return;

        auto stringlen = Length();
        auto secstringlen = string.Length();
        auto newlen = stringlen + secstringlen;

        if (secstringlen == 0)
            return;

        // realloc
        ReallocData(newlen);

        // add
        AddData(string.m_data, secstringlen, newlen);
    }

    /**
     * \brief Returns a substring copy of this string.
     * \param start Where start reading.
     * \param count How much characters we want.
     * \return The Substring string.
     */
    String Substring(const int start, const int count) const
    {
        auto stringLength = Length();

        if (start + count > stringLength)
            return Empty();

        return String(this, start, count);
    }

    /**
     * \brief Remove characters from start.
     * \param start The start index when start removing characters.
     * \param count The amount of characters to Remove.
     * \return The new string.
     */
    String Remove(const int start, const int count) const
    {
        if (m_const)
            return Empty();

        auto stringLength = Length();
        auto s = start + count;

        if (start == 0)
            return Substring(s, stringLength - s);

        if (start + count >= stringLength)
            return Substring(0, start);

        auto s1 = Substring(0, start);
        auto s2 = Substring(s, stringLength - s);

        s1.Append(s2);

        return String(&s1);
    }

    /**
     * \brief Gets index of character in this string.
     * \param character The character for find.
     * \return The index.
     */
    int IndexOf(Char character) const
    {
        auto i = 0;
        while (true)
        {
            if (m_data[i] == character)
                return i;

            if (m_data[i] == 0)
                return -1;

            i++;
        }
    }

    /**
     * \brief Gets last index of character in this string.
     * \param character The character for find.
     * \return The index.
     */
    int LastIndexOf(Char character) const
    {
        auto i = Length() - 1;
        while (true)
        {
            if (m_data[i] == character)
                return i;

            if (m_data[i] == 0)
                return -1;

            i--;
        }
    }

    /**
     * \brief Returns the string Length.
     * \return The Length.
     */
    int Length() const
    {
        return Length(m_data);
    }

    /**
     * \brief Returns the size of this string in bytes.
     * \return The size of this string.
     */
    int Size() const
    {
        // character count * char size + zero terminating character.
        return Length() * sizeof(Char) + sizeof(Char);
    }

    /**
     * \brief Returns the string data / characters.
     */
    Char* Data() const
    {
        return m_data;
    }

    /**
     * \brief Clones this string.
     */
    String Clone() const
    {
        return String(m_data);
    }

    /**
     * \brief Builds const string.
     */
    void CStr(char* buffer, const int buffer_size) const
    {
        for (auto i = 0; i < buffer_size && i < Length(); i++)
        {
            buffer[i] = static_cast<char>(m_data[i]);
        }
    }

    /**
     * \brief Returns const wide string.  This is slow, and it isn't recommended to use.
     */
    std::string StdStr() const
    {
#ifdef STRING_USE_UTF16
        std::wstring_convert<std::codecvt_utf8_utf16<__int16>, __int16> conversion;
        return conversion.to_bytes(reinterpret_cast<__int16*>(m_data));
#else
        std::wstring_convert<std::codecvt_utf8_utf16<__int32>, __int32> conversion;
        return conversion.to_bytes(reinterpret_cast<__int32*>(m_data));
#endif
    }

    /**
     * \brief Returns const wide string. This is slow, and it isn't recommended to use.
     * \return 
     */
    std::wstring StdWStr() const
    {
        auto str = StdStr();
        std::wstring temp(str.length(), L' ');
        copy(str.begin(), str.end(), temp.begin());

        return temp;
    }

public:
    String& operator=(const String& r)
    {
        auto len = r.Length();

        AllocData(len);
        CopyData(r.m_data, len);

        return *this;
    }

    String operator+(String& string) const
    {
        auto thisStr = Clone();

        thisStr.Append(string);

        return thisStr;
    }

    void operator+=(const String& string)
    {
        Append(const_cast<String&>(string));
    }

    bool operator ==(const String& string) const
    {
        if (Length() != string.Length())
            return false;

        for (auto i = 0; i < Length(); i++)
        {
            if (m_data[i] != string[i])
                return false;
        }

        return true;
    }

    Char& operator[](const size_t index)
    {
        return m_data[index];
    }

    const Char& operator[](const size_t index) const
    {
        return m_data[index];
    }

public:
    /**
     * \brief Returns empty string.
     * \return The new empty string.
     */
    static String Empty()
    {
        return String();
    }

    /**
     * \brief Creates const string which cannot be modified,
     *  but works best for scoped calls (loading a file eg.: "Font.API::Internal::loadFont").
     */
    static String Constant(uint16_t* chars)
    {
        String string;
        string.m_data = chars;
        string.m_const = true;
        return string;
    }

    static int Length(const Char* str)
    {
        if (str == nullptr)
            return 0;

        auto i = 0;
        while (str[i] != 0)
        {
            i++;
        }

        return i;
    }

#ifdef STRING_USE_FMT
    template<typename... Args>
    __forceinline static String Format(const String& format, const Args&... args)
    {
        auto data = reinterpret_cast<wchar_t*>(format.Data());
        auto string = fmt::format(data, args);

        return String((Char*)string.data());
    }

#endif

    static bool Compare(const Char* str1, const Char* str2)
    {
        if (str1 == nullptr || str2 == nullptr)
            return false;

        auto l1 = Length(str1);
        auto l2 = Length(str2);

        if (l1 != l2)
            return false;

        for (auto i = 0; i < l1; i++)
        {
            if (str1[i] != str2[i])
                return false;
        }

        return true;
    }
};

#ifdef STRING
# undef STRING
#endif

#ifdef STRING_USE_UTF16
# define STRING(string) String((Char*)u##string)
# define STRING_CHARS(string) (Char*)u##string
# define STRING_CONST(string) String::Constant((Char*)u##string)
#else
# define STRING(string) String((Char*)U##string)
# define STRING_CHARS(string) (Char*)U##string
# define STRING_CONST(string) String::Constant((Char*)U##string)
#endif
