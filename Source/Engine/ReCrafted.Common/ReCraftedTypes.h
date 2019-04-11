// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#ifndef RECRAFTEDTYPES_H
#define RECRAFTEDTYPES_H

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned int uint;

template <class T>
using RefPtr = std::shared_ptr<T>;

template <class T>
using Ref = std::reference_wrapper<T>;

#endif // RECRAFTEDTYPES_H
