// ReCrafted © 2016-2017 Always Too Late

#ifndef GUID_H
#define GUID_H

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
    Text toString()
    {
        char buffer[37];
        
        sprintf(buffer, GUID_FORMAT, GUID_ARG(value));

        return Text(buffer);
    }
};

#endif // GUID_H
