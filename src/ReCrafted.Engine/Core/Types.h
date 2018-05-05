// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TYPES_H
#define TYPES_H

// includes
#include <memory>

typedef unsigned char byte;

typedef signed char sbyte;

typedef unsigned int uint;

template <class T>
using Ref = std::shared_ptr<T>;

#endif // TYPES_H
