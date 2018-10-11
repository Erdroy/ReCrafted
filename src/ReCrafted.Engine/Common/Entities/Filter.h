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

public:
    void Require(const TypeId type)
    {
        m_requires.set(type);
    }

    void Exclude(const TypeId type)
    {
        m_exludes.set(type);
    }

    bool Test(const TypeId type) const
    {
        return m_requires.test(type);
    }

    bool MatchAll(const ComponentTypeList& typeList) const
    {
        const auto passExclude = (m_exludes & typeList).none();

        if (!passExclude)
            return false;

        const auto passRequire = (m_requires & typeList) == m_requires;

        return passRequire && passExclude;
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
