// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#ifndef SCRIPTINGENGINE_H
#define SCRIPTINGENGINE_H

// includes
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

class ScriptingEngine
{
private:
	static ScriptingEngine* m_instance;

private:

public:
	static void run();

	static void bind_all();

	static MonoClass* load_class(const char* classNamespace, const char* className);

	static MonoMethod* load_method(const char* methodName, MonoClass* monoClass);

	static MonoObject* create_class_instance(MonoClass* monoClass);

	static void initialize();

	static void update();

	static void simulate();

	static void shutdown();
};

#endif // SCRIPTINGENGINE_H
