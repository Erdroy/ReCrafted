// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#ifndef SCRIPTINGENGINE_H
#define SCRIPTINGENGINE_H

// includes
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "Core/Object.h"

class ScriptingEngine
{
	friend class GameMain;
	friend class Object;

private:
	static ScriptingEngine* m_instance;

private:
	static MonoMethod* load_method(const char* methodName, MonoClass* monoClass);

	static MonoObject* create_class_instance(MonoClass* monoClass);

private:
	static void run();

	static void bind_all();

	static void destroy(Object* object);

	static void onFinalized(Object* object);

private:
	static void initialize();

	static void update();

	static void simulate();

	static void drawui();

	static void shutdown();

public:
	/// <summary>
	/// Gets Mono class.
	/// </summary>
	static MonoClass* getClass(const char* classNamespace, const char* className);

	/// <summary>
	/// Creates new instance of given Object type.
	/// </summary>
	template <class T>
	static T* create(MonoClass* monoClass);
};

template <class T>
T* ScriptingEngine::create(MonoClass* monoClass)
{
	return new T(create_class_instance(monoClass));
}

#endif // SCRIPTINGENGINE_H
