// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#ifndef UI_H
#define UI_H

// includes

class UI
{
private:
	static UI* m_instance;

private:

public:
	UI() { m_instance = this; }

public:
	void init();

	void begin_draw();
	void end_draw();

	void test_draw();
};

#endif // UI_H
