// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef UTILS_H
#define UTILS_H

#define INVALID_HANDLE uint(0)

#define IS_INVALID(handleValue) (handleValue.m_idx == INVALID_HANDLE)

#define OBJECT_ALLOCATOR(type, name, handleType, amount) \
	inline handleType name##_alloc() \
	{\
		for(auto i = 1u; i < amount ; i ++) { \
			if(m_##name##Array[i] == nullptr) { \
				m_##name##Array[i] = nullptr; \
				return handleType##(i); \
			} \
		} \
		return handleType##(); \
	}\
	inline type name##_get(handleType handle) \
	{\
		return m_##name##Array[handle.m_idx]; \
	}

#define OBJECT_DEALLOCATOR(name, handleType)\
	inline void name##_free( handleType value ) \
	{ m_##name##Array[value.m_idx] = nullptr; }

#define OBJECT_ARRAY(type, name, handleType, amount) \
	type m_##name##Array [ amount ]; \
	OBJECT_ALLOCATOR(type, name, handleType, amount)\
	OBJECT_DEALLOCATOR(name, handleType)\
	OBJECT_RELEASE_DEF(name, handleType)
	
#endif // UTILS_H
