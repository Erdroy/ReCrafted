// ReCrafted (c) 2016-2019 Always Too Late. All rights reserved.. All rights reserved.

#pragma once

#ifndef RECRAFTEDDEFINES_HPP
#define RECRAFTEDDEFINES_HPP

#ifdef RC_API
# undef RC_API
#endif

#ifdef _WIN32
# if defined(RC_API_EXPORT_COMMON) || defined(RC_API_EXPORT_CORE)
#  define RC_API __declspec(dllexport)
# else
#  define RC_API __declspec(dllimport)
# endif
#else
# define RC_API
#endif

#endif // RECRAFTEDDEFINES_HPP
