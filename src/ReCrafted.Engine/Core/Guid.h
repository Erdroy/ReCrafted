// ReCrafted (c) 2016-2019 Always Too Late

#ifndef GUID_H
#define GUID_H

// includes
#include "../Common/Text.h"

// source: https://stackoverflow.com/questions/1672677/print-a-guid-variable
#define GUID_FORMAT "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX"
#define GUID_ARG(guid) guid.data1, guid.data2, guid.data3, guid.data4[0], guid.data4[1], guid.data4[2], guid.data4[3], guid.data4[4], guid.data4[5], guid.data4[6], guid.data4[7]

struct Guid
{
public:
    struct _value
    {
        long data1;
        short data2;
        short data3;
        char data4[8];
    } value;

public:
    Text ToString()
    {
        char buffer[37];

        sprintf(buffer, GUID_FORMAT, GUID_ARG(value));

        return Text(buffer);
    }

public:
    bool operator ==(const Guid& rhs) const
    {
        return value.data1 == rhs.value.data1
        && value.data2 == rhs.value.data2
        && value.data3 == rhs.value.data3
        && strcmp(value.data4, rhs.value.data4) == 0;
    }

public:
    static Guid Parse(const std::string& string)
    {
        Guid guid{};

        unsigned long p0;
        int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

        auto err = sscanf_s(string.c_str(), "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
            &p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);

        guid.value.data1 = p0;
        guid.value.data2 = p1;
        guid.value.data3 = p2;
        guid.value.data4[0] = p3;
        guid.value.data4[1] = p4;
        guid.value.data4[2] = p5;
        guid.value.data4[3] = p6;
        guid.value.data4[4] = p7;
        guid.value.data4[5] = p8;
        guid.value.data4[6] = p9;
        guid.value.data4[7] = p10;

        return guid;
    }
};

namespace std {
    template<> struct hash<Guid>
    {
        size_t operator()(const Guid& guid) const noexcept {
            const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(&guid);
            std::hash<std::uint64_t> hash;
            return hash(p[0]) ^ hash(p[1]);
        }
    };
}

#endif // GUID_H
