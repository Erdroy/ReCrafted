// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TEXT_H
#define TEXT_H

// includes
#include <locale>
#include <codecvt>

#define USE_UTF16
#define USE_FMT

#include "ReCraftedConfig.h"

#ifdef USE_FMT
#include "Core/fmt/format.h"
#endif

#ifdef USE_UTF16
typedef unsigned short Char;
#else
typedef unsigned int Char;
#endif

#if COMPILE_WITH_RPMALLOC
#include <rpmalloc.h>
#define TEXT_ALLOC rpmalloc
#define TEXT_REALLOC rprealloc
#define TEXT_FREE rpfree
#else
#define TEXT_ALLOC malloc
#define TEXT_REALLOC realloc
#define TEXT_FREE free
#endif

/// <summary>
/// Text structure.
/// </summary>
struct Text
{
private:
    Char* m_data = nullptr;
    char* m_cstrData = nullptr;
    bool m_const = false;

private:
    void AllocData(int count)
    {
        m_data = static_cast<Char*>(TEXT_ALLOC(sizeof(Char) * (count + 1)));
    }

    void ReallocData(int count)
    {
        if (m_data == nullptr)
        {
            AllocData(count);
            return;
        }

        auto tmp = static_cast<Char*>(TEXT_REALLOC(m_data, sizeof(Char) * (count + 1)));

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
    explicit Text(Text* text)
    {
        auto len = text->Length();

        AllocData(len);
        CopyData(text->m_data, len);
    }

    explicit Text(const Text* text)
    {
        auto len = text->Length();

        AllocData(len);
        CopyData(text->m_data, len);
    }

    explicit Text(const Text* text, int start, int count)
    {
        AllocData(count);
        auto ptr = text->m_data + start;
        CopyData(ptr, count);
    }

public:
    /// <summary>
    /// Default constructor
    /// </summary>
    Text()
    {
    }

    /// <summary>
    /// Constructor of text that copies the other text.
    /// </summary>
    /// <param name="text">The text that will be copied.</param>
    Text(const Text& text)
    {
        if(text.m_const)
        {
            m_data = text.m_data;
            m_const = true;
            return;
        }

        const auto length = text.Length();
        AllocData(length);
        CopyData(text.m_data, length);
        m_cstrData = text.m_cstrData;
        m_const = text.m_const;
    }

    /// <summary>
    /// Constructor of text thats using const wide cstring.
    /// </summary>
    /// <param name="text">The wide cstring.</param>
    explicit Text(Char* string)
    {
        auto length = 0;
        while (string[length] != 0)
        {
            length++;
        }

        AllocData(length);
        CopyData(string, length);
    }

    /// <summary>
    /// Constructor of text thats using const wide cstring.
    /// </summary>
    /// <param name="text">The cstring.</param>
    explicit Text(const char* string)
    {
        auto length = 0;
        while (string[length] != 0)
        {
            length++;
        }

        AllocData(length);

        for (auto i = 0; i < length; i ++)
        {
            m_data[i] = static_cast<Char>(string[i]);
        }
        m_data[length] = 0;
    }

    /// <summary>
    /// Default destructor.
    /// </summary>
    ~Text()
    {
        if (!m_const)
        {
            if (m_data)
            {
                TEXT_FREE(m_data);
                m_data = nullptr;
            }

            if (m_cstrData)
            {
                TEXT_FREE(m_cstrData);
                m_cstrData = nullptr;
            }
        }
    }

public:
    /// <summary>
    /// Appents text to this text.
    /// </summary>
    /// <param name="text">The text to Append.</param>
    void Append(Text& text)
    {
        if (m_const)
            return;

        auto textlen = Length();
        auto sectextlen = text.Length();
        auto newlen = textlen + sectextlen;

        if (sectextlen == 0)
            return;

        // realloc
        ReallocData(newlen);

        // add
        AddData(text.m_data, sectextlen, newlen);
    }

    /// <summary>
    /// Returns a substring copy of this text.
    /// </summary>
    /// <param name="start">Where start reading.</param>
    /// <param name="count">How much characters we want.</param>
    /// <returns>The Subtext text.</returns>
    Text Subtext(int start, int count) const
    {
        auto textLength = Length();

        if (start + count > textLength)
            return Empty();

        return Text(this, start, count);
    }

    /// <summary>
    /// Remove characters from start.
    /// </summary>
    /// <param name="start">The start index when start removing characters.</param>
    /// <param name="count">The amount of characters to Remove.</param>
    /// <returns>The new text.</returns>
    Text Remove(int start, int count) const
    {
        if (m_const)
            return Empty();

        auto textLength = Length();
        auto s = start + count;

        if (start == 0)
            return Subtext(s, textLength - s);

        if (start + count >= textLength)
            return Subtext(0, start);

        auto s1 = Subtext(0, start);
        auto s2 = Subtext(s, textLength - s);

        s1.Append(s2);

        return Text(&s1);
    }

    /// <summary>
    /// Gets index of character in this text.
    /// </summary>
    /// <param name="character">The character for find.</param>
    /// <returns>The index.</returns>
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

    /// <summary>
    /// Gets last index of character in this text.
    /// </summary>
    /// <param name="character">The character for find.</param>
    /// <returns>The index.</returns>
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

    /// <summary>
    /// Returns the text Length.
    /// </summary>
    /// <returns>The Length.</returns>
    int Length() const
    {
        return Length(m_data);
    }

    /// <summary>
    /// Returns the size of this string in bytes.
    /// </summary>
    /// <returns>The size of this string.</returns>
    int Size() const
    {
        // character count * char size + zero terminating character.
        return Length() * sizeof(Char) + sizeof(Char);
    }

    /// <summary>
    /// Returns the text data / characters.
    /// </summary>
    Char* Data() const
    {
        return m_data;
    }

    /// <summary>
    /// Clones this string.
    /// </summary>
    Text Clone() const
    {
        return Text(m_data);
    }

    /// <summary>
    /// Builds const string.
    /// </summary>
    void CStr(char* buffer, int buffer_size) const
    {
        for (auto i = 0; i < buffer_size && i < Length(); i ++)
        {
            buffer[i] = static_cast<char>(m_data[i]);
        }
    }

    /// <summary>
    /// Returns const wide string.
    /// This is slow, and it isn't recommended to use.
    /// </summary>
    /// <returns>Const C string.</returns>
    std::string StdStr() const
    {
#ifdef USE_UTF16
        std::wstring_convert<std::codecvt_utf8_utf16<__int16>, __int16> conversion;
        return conversion.to_bytes(reinterpret_cast<__int16*>(m_data));
#else
		std::wstring_convert<std::codecvt_utf8_utf16<__int32>, __int32> conversion;
		return conversion.to_bytes(reinterpret_cast<__int32*>(m_data));
#endif
    }

    /// <summary>
    /// Returns const wide string.
    /// This is slow, and it isn't recommended to use.
    /// </summary>
    /// <returns>Const Wide C string.</returns>
    std::wstring StdWStr() const
    {
        auto str = StdStr();
        std::wstring temp(str.length(), L' ');
        copy(str.begin(), str.end(), temp.begin());

        return temp;
    }

public:
    Text& Text::operator=(const Text& r)
    {
        auto len = r.Length();

        AllocData(len);
        CopyData(r.m_data, len);

        return *this;
    }

    Text Text::operator+(Text& text) const
    {
        auto thisStr = Clone();

        thisStr.Append(text);

        return thisStr;
    }

    void Text::operator+=(const Text& text)
    {
        Append(const_cast<Text&>(text));
    }

    bool Text::operator ==(const Text& text) const
    {
        if (Length() != text.Length())
            return false;

        for (auto i = 0; i < Length(); i++)
        {
            if (m_data[i] != text[i])
                return false;
        }

        return true;
    }

    Char& operator[](size_t index)
    {
        return m_data[index];
    }

    const Char& operator[](size_t index) const
    {
        return m_data[index];
    }

public:
    /// <summary>
    /// Returns empty text.
    /// </summary>
    /// <returns>The new empty text.</returns>
    static Text Empty()
    {
        return Text();
    }

    /// <summary>
    /// Creates const text which cannot be modified,
    /// but works best for recursive calling (loading a file eg.: "Font.API::Internal::loadFont").
    /// </summary>
    static Text Constant(uint16_t* chars)
    {
        Text text;
        text.m_data = chars;
        text.m_const = true;
        return text;
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

#ifdef USE_FMT
    __forceinline static Text Format(const Text& format, fmt::ArgList args)
    {
        auto data = reinterpret_cast<wchar_t*>(format.Data());
        auto string = fmt::format(data, args);

        return Text((Char*)string.data());
    }

    FMT_VARIADIC_W(static Text, Format, const Text&)
#endif

    static bool Compare(const Char* str1, const Char* str2)
    {
        if (str1 == nullptr || str2 == nullptr)
            return false;

        auto l1 = Length(str1);
        auto l2 = Length(str2);

        if (l1 != l2)
            return false;

        for (auto i = 0; i < l1; i ++)
        {
            if (str1[i] != str2[i])
                return false;
        }

        return true;
    }
};

#if defined(TEXT)
#undef TEXT
#endif

#ifdef USE_UTF16
#define TEXT(text) Text((Char*)u##text)
#define TEXT_CHARS(text) (Char*)u##text
#define TEXT_CONST(text) Text::Constant((Char*)u##text)
#else
#define TEXT(text) Text((Char*)U##text)
#define TEXT_CHARS(text) (Char*)U##text
#define TEXT_CONST(text) Text::Constant((Char*)U##text)
#endif

#endif // TEXT_H
