// ReCrafted © 2016-2017 Always Too Late

#ifndef HTML5_UI_H
#define HTML5_UI_H

class HTML5UI
{
	friend class GameMain;

private:
	static void init();
	static void shutdown();
	static void draw();
};

#endif // HTML5_UI_H
