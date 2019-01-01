// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef OBJECTID_H
#define OBJECTID_H

#include <atomic>

template<typename TIdType>
struct ObjectId
{
private:
    static std::atomic<TIdType> m_lastObjectId;

public:
    static TIdType GetNextId()
    {
        static const auto id = m_lastObjectId++;
        return id;
    }
};

template <typename TIdType>
std::atomic<TIdType> ObjectId<TIdType>::m_lastObjectId = { 0 };

#endif // OBJECTID_H
