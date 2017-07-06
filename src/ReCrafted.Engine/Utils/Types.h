// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef TYPES_H
#define TYPES_H

// includes
#include <memory>

typedef unsigned char byte;

typedef signed char sbyte;

typedef unsigned short uint16;

typedef unsigned int uint32;

typedef uint32 uint;

typedef unsigned long uint64;

template <class T>
using Ptr = std::shared_ptr<T>;

#endif // TYPES_H
