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

private:
	static ScriptingEngine* m_instance;

private:
	static MonoMethod* load_method(const char* methodName, MonoClass* monoClass);

	static MonoObject* create_class_instance(MonoClass* monoClass);

private:
	static void initialize();

	static void update();

	static void simulate();

	static void drawui();

	static void shutdown();

public:
	static void run();

	static void bind_all();

	static MonoClass* getClass(const char* classNamespace, const char* className);

	static Object* create(MonoClass* monoClass);

	static void destroy(Object* object);

	static void onFinalized(Object* object);
};

#endif // SCRIPTINGENGINE_H
