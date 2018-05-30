// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ATLAS_H
#define ATLAS_H

// includes
#include "Texture2D.h"
#include "Core/Math/Rect.h"
#include "Core/Containers/Array.h"

struct Text;

/**
 * \brief Atlas class.
 */
class Atlas : public IResource
{
public:
    struct Element
    {
        char name[32] = {};
        Rect rect = {};
    };

private:
    RefPtr<Texture2D> m_texture = nullptr;
    Array<Element> m_elements = {};

private:
IRESOURCE_IMPL(Atlas)

public:
    /**
	 * \brief Gets element's rect by name.
	 * \param name The element's name.
	 * \return The rect.
	 */
    Rect GetRect(const char* name);

    /**
	 * \brief Gets texture of this atlas.
	 * \return The texture pointer.
	 */
    RefPtr<Texture2D> GetTexture() const;

    /**
	 * \brief Disposes this atlas.
	 */
    void Dispose() override;

public:
    /**
	 * \brief Loads atlas from given JSON file.
	 * \param fileName The JSON file name.
	 * \return The loaded atlas, or null when file is not found.
	 */
    static RefPtr<Atlas> Load(Text& fileName);
};

#endif // ATLAS_H
