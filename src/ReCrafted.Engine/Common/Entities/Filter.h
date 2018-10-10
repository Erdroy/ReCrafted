// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef FILTER_H
#define FILTER_H

#include "ClassTypeId.h"

#include <bitset>

using ComponentTypeList = std::bitset<ECS_MAX_COMPONENTS>;

struct Filter
{
private:
    ComponentTypeList m_requires = {};
    ComponentTypeList m_exludes = {};
    unsigned long long m_reqULL = 0u;
    unsigned long long m_exlULL = 0u;

public:
    void Require(const TypeId type)
    {
        m_requires.set(type);
        m_reqULL = m_requires.to_ullong();
    }

    void Exclude(const TypeId type)
    {
        m_exludes.set(type);
        m_exlULL = m_exludes.to_ullong();
    }

    bool Test(const TypeId type) const
    {
        return m_requires.test(type);
    }

    bool MatchAll(const ComponentTypeList& typeList) const
    {
        const auto typeList_ull = typeList.to_ullong();
        return (m_reqULL & typeList_ull) == m_reqULL && (m_exlULL & typeList_ull) == 0;
    }

    void Remove(const TypeId type)
    {
        m_requires.reset(type);
        m_exludes.reset(type);
    }

    void Reset()
    {
        m_requires.reset();
        m_exludes.reset();
    }
};

#endif // FILTER_H
