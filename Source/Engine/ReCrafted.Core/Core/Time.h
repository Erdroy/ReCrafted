// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#ifndef TIME_H
#define TIME_H

template<typename TSelf, typename TType, TType(TSelf::*getter)() = nullptr, void (TSelf::*setter)(TType) = nullptr>
struct Property
{
private:
    TType _value;

public:
    Property()
    {
        _value = TType();
    }

    explicit Property(TType&& value) : _value(value) { }

    Property& operator = (const TType& val)
    {
        // TODO: Call setter
        _value = val;
        return *this;
    }

    explicit operator const TType&() const
    {
        // TODO: Call getter
        return _value;
    }
};

class Time
{
    friend class MainLoop;

private:
    void Initialize();

    float GetFloat();
    void SetFloat(float v);

public:
    Property<Time, float, &Time::GetFloat, &Time::SetFloat> DeltaTime;
};

#endif // TIME_H
