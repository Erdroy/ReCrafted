// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Display.h"

uint Display::m_width;
uint Display::m_height;

void Display::ForceUpdate()
{
    uint width;
    uint height;
    Platform::GetWindowSize(Platform::GetCurrentWindow(), &width, &height);

    m_width = width;
    m_height = height;
}

float Display::GetAspectRatio()
{
    return static_cast<float>(m_width) / static_cast<float>(m_height);
}

uint Display::GetWidth()
{
    return m_width;
}

uint Display::GetHeight()
{
    return m_height;
}
