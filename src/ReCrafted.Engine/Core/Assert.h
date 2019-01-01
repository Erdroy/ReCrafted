// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef ASSERT_H
#define ASSERT_H

#include "Platform/Platform.h"

#define _UNICODE_(s) u ##s

#define _ASSERT_(expression, message)                                                                                                               \
        (void)(                                                                                                                                     \
            (!!(expression)) ||                                                                                                                     \
            (Platform::ReportAssert(Text::Constant((uint16_t*)_UNICODE_(#expression)), Text(__FILE__), (unsigned)(__LINE__), Text(message)), 0)     \
        )

#define ASSERT(expression) _ASSERT_(expression, "")

#if _DEBUG
#   define DEBUG_ASSERT(expression) _ASSERT_(expression, "")
#else
#   define DEBUG_ASSERT(expression)
#endif

#define STATIC_ASSERT(expr) typedef char __static_assert_t[(expr) != 0]

#endif // ASSERT_H
