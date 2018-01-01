// ReCrafted (c) 2016-2018 Always Too Late

#include "Script.h"

#include "Common/Entity.h"

void Script::dispose()
{
    m_onUpdate = nullptr;
    m_onSimulate = nullptr;
    m_onDestroy = nullptr;
}
