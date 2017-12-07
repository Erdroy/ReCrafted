// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef TEXT_H
#define TEXT_H

// includes
#include <locale>
#include <codecvt>

#include "Core/Defines.h"

#define USE_UTF16
#define USE_FMT

#ifdef USE_FMT
#include "Core/fmt/format.h"
#endif

#ifdef USE_UTF16
typedef unsigned short Char;
#else
typedef unsigned int Char;
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
	void alloc_chars(int count)
	{
		m_data = static_cast<Char*>(malloc(sizeof(Char) * (count + 1)));
	}

	void realloc_chars(int count)
	{
		if(m_data == nullptr)
		{
			alloc_chars(count);
			return;
		}

		auto tmp = static_cast<Char*>(realloc(m_data, sizeof(Char) * (count + 1)));

		if (tmp == nullptr)
			throw;

		m_data = tmp;
	}

	void copy_chars(Char* source, int count) const
	{
		auto sz = sizeof(Char) * count;
		memcpy(m_data, source, sz);
		m_data[count] = 0;
	}

	void add_chars(Char* source, int count, int totalcount) const
	{
		auto sz = sizeof(Char) * count;

		auto ptr = m_data + length();
		memcpy(ptr, source, sz);

		m_data[totalcount] = 0;
	}

private:
	explicit Text(Text* text)
	{
		auto len = text->length();

		alloc_chars(len);
		copy_chars(text->m_data, len);
	}

	explicit Text(const Text* text)
	{
		auto len = text->length();

		alloc_chars(len);
		copy_chars(text->m_data, len);
	}

	explicit Text(const Text* text, int start, int count)
	{
		alloc_chars(count);
		auto ptr = text->m_data + start;
		copy_chars(ptr, count);
	}

public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Text() { }

	/// <summary>
	/// Constructor of text that copies the other text.
	/// </summary>
	/// <param name="text">The text that will be copied.</param>
	Text(const Text& text)
	{
		auto length = text.length();

		alloc_chars(length);
		copy_chars(text.m_data, length);
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

		alloc_chars(length);
		copy_chars(string, length);
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

		alloc_chars(length);

		for(auto i = 0; i < length; i ++)
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
			SafeFree(m_data);
			SafeFree(m_cstrData);
		}
	}

public:
	/// <summary>
	/// Appents text to this text.
	/// </summary>
	/// <param name="text">The text to append.</param>
	void append(Text& text)
	{
        if (m_const)
            return;

		auto textlen = length();
		auto sectextlen = text.length();
		auto newlen = textlen + sectextlen;

		if (sectextlen == 0)
			return;

		// realloc
		realloc_chars(newlen);

		// add
		add_chars(text.m_data, sectextlen, newlen);
	}

	/// <summary>
	/// Returns a substring copy of this text.
	/// </summary>
	/// <param name="start">Where start reading.</param>
	/// <param name="count">How much characters we want.</param>
	/// <returns>The subtext text.</returns>
	Text subtext(int start, int count) const
	{
		auto textLength = length();

		if (start + count > textLength)
			return empty();

		return Text(this, start, count);
	}

	/// <summary>
	/// Remove characters from start.
	/// </summary>
	/// <param name="start">The start index when start removing characters.</param>
	/// <param name="count">The amount of characters to remove.</param>
	/// <returns>The new text.</returns>
	Text remove(int start, int count) const
	{
        if (m_const)
            return empty();

		auto textLength = length();
		auto s = start + count;

		if(start == 0)
			return subtext(s, textLength - s);

		if (start + count >= textLength)
			return subtext(0, start);

		auto s1 = subtext(0, start);
		auto s2 = subtext(s, textLength - s);

		s1.append(s2);

		return Text(&s1);
	}

	/// <summary>
	/// Gets index of character in this text.
	/// </summary>
	/// <param name="character">The character for find.</param>
	/// <returns>The index.</returns>
	int indexOf(Char character) const
	{
		auto i = 0;
		while(true)
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
	int lastIndexOf(Char character) const
	{
		auto i = length()-1;
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
	/// Returns the text length.
	/// </summary>
	/// <returns>The length.</returns>
	int length() const
	{
		return length(m_data);
	}

	/// <summary>
	/// Returns the size of this string in bytes.
	/// </summary>
	/// <returns>The size of this string.</returns>
	int size() const
	{
		// character count * char size + zero terminating character.
		return length() * sizeof(Char) + sizeof(Char);
	}

	/// <summary>
	/// Returns the text data / characters.
	/// </summary>
	Char* data() const
	{
		return m_data;
	}

	/// <summary>
	/// Clones this string.
	/// </summary>
	Text clone() const
	{
		return Text(m_data);
	}

	/// <summary>
	/// Builds const string.
	/// </summary>
	void c_str(char* buffer, int buffer_size) const
	{
		for(auto i = 0; i < buffer_size && i < length(); i ++)
		{
			buffer[i] = static_cast<char>(m_data[i]);
		}
	}

	/// <summary>
	/// Returns const wide string.
	/// This is slow, and it isn't recommended to use.
	/// </summary>
	/// <returns>Const C string.</returns>
	std::string std_str() const
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
	std::wstring wstr() const
	{
		auto str = std_str();
		std::wstring temp(str.length(), L' ');
		copy(str.begin(), str.end(), temp.begin());

		return temp;
	}

public:
	Text& Text::operator=(const Text& r){
		auto len = r.length();
		
		alloc_chars(len);
		copy_chars(r.m_data, len);

		return *this;
	}

	Text Text::operator+(Text& text) const
	{
		auto thisStr = clone();

		thisStr.append(text);

		return thisStr;
	}

	void Text::operator+=(const Text& text){
		append(const_cast<Text&>(text));
	}

	bool Text::operator == (const Text& text) const {
		if (length() != text.length())
			return false;

		for (auto i = 0; i < length(); i++)
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
	static Text empty()
	{
		return Text();
	}

	/// <summary>
	/// Creates const text which cannot be modified,
	/// but works best for recursive calling (loading a file eg.: "Font.API::Internal::loadFont").
	/// </summary>
	static Text constant(uint16_t* chars)
	{
		Text text = {};
		text.m_data = chars;
		text.m_const = true;
		return text;
	}

	static int length(const Char* str)
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
	FORCEINLINE static Text format(const Text& format, fmt::ArgList args)
	{
		auto data = reinterpret_cast<wchar_t*>(format.data());
		auto string = fmt::format(data, args);

		return Text((Char*)string.data());
	}
	FMT_VARIADIC_W(static Text, format, const Text&)
#endif
};

#ifdef TEXT
#undef TEXT
#endif

#ifdef USE_UTF16
#define TEXT(text) Text((Char*)u##text)
#define TEXT_CHARS(text) (Char*)u##text
#define TEXT_CONST(text) Text::constant((Char*)u##text)
#else
#define TEXT(text) Text((Char*)U##text)
#define TEXT_CHARS(text) (Char*)U##text
#define TEXT_CONST(text) Text::constant((Char*)U##text)
#endif

#endif // TEXT_H
