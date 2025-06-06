// VectorMath (c) 2018-2019 Damian 'Erdroy' Korczowski

#pragma once

#include "Config.h"

class Math
{
public:
    template<typename TValue>
    static constexpr TValue Clamp(TValue value, TValue min, TValue max)
    {
        return value < min ? min : 
               value > max ? max : value;
    }

    template<typename TValue>
    static constexpr TValue Clamp01(TValue value)
    {
        return value < TValue(0) ? TValue(0) :
            value > TValue(1) ? TValue(1) : value;
    }

    template<typename TValue>
    static constexpr TValue Min(TValue a, TValue b)
    {
        return a < b ? a : b;
    }

    template<typename TValue>
    static constexpr TValue Max(TValue a, TValue b)
    {
        return a > b ? a : b;
    }

    template<typename TValue>
    static constexpr TValue Min(TValue a, TValue b, TValue c)
    {
        return Min(Min(a, b), Min(b, c));
    }

    template<typename TValue>
    static constexpr TValue Max(TValue a, TValue b, TValue c)
    {
        return Max(Max(a, b), Max(b, c));
    }

    template<typename TValue>
    static constexpr TValue Abs(TValue value)
    {
        return value >= 0 ? value : -value;
    }

    template<typename TBase, typename TExponent>
    static TBase Pow(TBase base, TExponent exponent)
    {
        return static_cast<TBase>(std::pow(base, exponent));
    }

    template<typename TValue>
    static TValue Sqrt(TValue value)
    {
        return std::sqrt(value);
    }

    template<typename TValue>
    static TValue Acos(TValue value)
    {
        return std::acos(value);
    }

    template<typename TValue>
    static TValue Asin(TValue value)
    {
        return std::asin(value);
    }

    template<typename TValue>
    static TValue Cos(TValue value)
    {
        return std::cos(value);
    }

    template<typename TValue>
    static TValue Sin(TValue value)
    {
        return std::sin(value);
    }

    template<typename TValue>
    static TValue Tan(TValue value)
    {
        return std::tan(value);
    }

    template<typename TValue>
    static TValue Atan(TValue value)
    {
        return std::atan(value);
    }

    template<typename TValue>
    static constexpr TValue SmoothStep(TValue value)
    {
        return (value <= 0) ? 0
             : (value >= 1) ? 1
             : value * value * (3 - (2 * value));
    }

    template<typename TValue>
    static constexpr TValue SmootherStep(TValue value)
    {
        return (value <= 0) ? 0
             : (value >= 1) ? 1
             : value * value * value * (value * ((value * 6) - 15) + 10);
    }

    template<typename TBase, typename TAmount>
    static constexpr TBase Lerp(TBase from, TBase to, TAmount amount)
    {
        return from + (to - from) * amount;
    }

    template<typename TValue>
    static constexpr int Sign(TValue value)
    {
        return value < 0 ? -1 : 1;
    }

    template<typename TValue>
    static constexpr bool NearEqual(TValue a, TValue b)
    {
        return Abs(a - b) < ZeroTolerance;
    }

    template<typename TValue>
    static constexpr bool IsOne(TValue value)
    {
        return IsZero(value - 1);
    }

    static int FloorToInt(const float value)
    {
        return static_cast<int>(floorf(value));
    }

    static int FloorToInt(const double value)
    {
        return static_cast<int>(floorf(static_cast<float>(value)));
    }

    static int CeilToInt(const float value)
    {
        return static_cast<int>(ceilf(value));
    }

    static int CeilToInt(const double value)
    {
        return static_cast<int>(ceilf(static_cast<float>(value)));
    }

    static constexpr bool IsZero(const int a)
    {
        return a == 0;
    }

    static constexpr unsigned long RoundUpToPow2(unsigned long v)
    {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;

        return v;
    }

    static int RoundUp(const int numToRound, const int multiple)
    {
        // source: http://stackoverflow.com/questions/3407012/c-rounding-up-to-the-nearest-multiple-of-a-number

        // allow round up to 0
        if (numToRound < 0 && -numToRound < multiple)
            return 0;

        const auto remainder = numToRound % multiple;

        if (remainder == 0)
            return numToRound;

        return numToRound + multiple - remainder;
    }

    static constexpr bool IsZero(const float a)
    {
        return Abs(a) < FLT_EPSILON;
    }

    static constexpr bool IsZero(const double a)
    {
        return Abs(a) < DBL_EPSILON;
    }

    static constexpr int Index2(const int x, const int y, const int size)
    {
        return x * size * size + y;
    }

    static constexpr int Index3(const int x, const int y, const int z, const int size)
    {
        return x * size * size + y * size + z;
    }

public:
    static const float ZeroTolerance;
    static const float Pi;
    static const float TwoPi;
    static const float PiOverTwo;
    static const float PiOverFour;

    static const float DegreeToRadian;
    static const float RadianToDegree;
};
