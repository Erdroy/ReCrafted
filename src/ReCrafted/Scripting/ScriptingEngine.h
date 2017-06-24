// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#ifndef SCRIPTINGENGINE_H
#define SCRIPTINGENGINE_H

class ScriptingEngine
{
private:
	static ScriptingEngine* m_instance;

private:

public:
	static void run();

	static void shutdown();
};

#endif // SCRIPTINGENGINE_H
