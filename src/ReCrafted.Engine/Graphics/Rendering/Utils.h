// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef UTILS_H
#define UTILS_H

#define IS_INVALID(handleValue) (handleValue.m_idx == 0u)

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
		if(handle.m_idx >= amount)\
			return nullptr;\
		return m_##name##Array[handle.m_idx]; \
	}\
	inline void name##_set(handleType handle, type value) \
	{\
		if(handle.m_idx >= amount)\
			return;\
		m_##name##Array[handle.m_idx] = value; \
	}

#define OBJECT_DEALLOCATOR(name, handleType, amount)\
	inline void name##_free( handleType handle ) \
	{\
		if(handle.m_idx >= amount)\
			return;\
		m_##name##Array[handle.m_idx] = nullptr; \
	}

#define OBJECT_ARRAY(type, name, handleType, amount) \
	type m_##name##Array [ amount ]; \
	OBJECT_ALLOCATOR(type, name, handleType, amount)\
	OBJECT_DEALLOCATOR(name, handleType, amount)\
	OBJECT_RELEASE_DEF(name, handleType, amount)
	
#define OBJECT_INFO_ARRAY(type, name, handleType, amount)\
	type m_##name##Array [ amount ]; \
	void name##_set(handleType handle, type value) { m_##name##Array[handle.m_idx] = value; } \
	type* name##_get(handleType handle) { return &m_##name##Array[handle.m_idx]; } \

#endif // UTILS_H
