// ReCrafted © 2016-2017 Always Too Late

#ifndef HTML5_UI_H
#define HTML5_UI_H

class HTML5UI
{
	friend class GameMain;

public:
	static void init();
	static void initChildren();
	static void shutdown();
	static void draw();
	static void update();
	static void resized();
};

#endif // HTML5_UI_H
