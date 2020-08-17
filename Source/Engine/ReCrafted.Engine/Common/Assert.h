// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#define _ASSERT_(expression, message)                                                                                                               \
        (void)(                                                                                                                                     \
            (!!(expression)) ||                                                                                                                     \
            (Platform::ReportAssert(String::Constant((uint16_t*)_UNICODE_(#expression)), String(__FILE__), (unsigned)(__LINE__), String(message)), 0)     \
        )

#define ASSERT(expression) _ASSERT_(expression, "")

#if _DEBUG
#   define DEBUG_ASSERT(expression) _ASSERT_(expression, "")
#else
#   define DEBUG_ASSERT(expression)
#endif

#define IS_MAIN_THREAD() Platform::GetMainThreadId() == std::this_thread::get_id()

#define MAIN_THREAD_ONLY() \
    _ASSERT_(IS_MAIN_THREAD(), __FUNCTION__" can be called only from the main thread!");
