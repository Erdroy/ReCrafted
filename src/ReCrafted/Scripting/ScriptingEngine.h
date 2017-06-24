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

	static void load_assembly(const char* assemblyName);

	static void shutdown();
};

#endif // SCRIPTINGENGINE_H
