// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "GameCore.h"
#include "../Utils/Defines.h"

#include <Windows.h>
#include "../Platform/Platform.Windows.h"
#include "../r3d/r3d_commandlist.h"


//#define R3DTEST

GameBase* GameBase::m_instance;

GameBase* gameBase_instance;

int m_cursorX = 0u;
int m_cursorY = 0u;

#define GAMEBASE_CHECK_SHUTDOWN if (!m_running) break;

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_SIZE: // handle window resizing
	{
		auto width = LOWORD(lparam);
		auto height = HIWORD(lparam);
		gameBase_instance->onResize(width, height);
		break;
	}

	case WM_CLOSE: // handle window closing
	{
		gameBase_instance->shutdown();
		return 0;
	}

	// input handling
	case WM_KEYDOWN:
	{
		Input::getInstance()->emit(false, uint(wparam));
		return 0;
	}
	case WM_KEYUP:
	{
		Input::getInstance()->emit(true, uint(wparam));
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		// mouse keys are held the shitty way, so all of tho are on the end of table 'm_keys', INPUT_MAXKEYS is the count of elements.
		Input::getInstance()->emit(false, INPUT_LBUTTON);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		Input::getInstance()->emit(true, INPUT_LBUTTON);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		Input::getInstance()->emit(false, INPUT_MBUTTON);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		Input::getInstance()->emit(true, INPUT_MBUTTON);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		Input::getInstance()->emit(false, INPUT_RBUTTON);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		Input::getInstance()->emit(true, INPUT_RBUTTON);
		return 0;
	}

	// cursor
	case WM_MOUSEMOVE:
	{
		m_cursorX = LOWORD(lparam);
		m_cursorY = HIWORD(lparam);
		return 0;
	}

	// default
	default: 
		return DefWindowProc(hWnd, msg, wparam, lparam);
	}

	return DefWindowProc(hWnd, msg, wparam, lparam);
}

void GameBase::run()
{
	// set all needed instance handlers
	gameBase_instance = m_instance = this;

	m_input = new Input;
	m_time = new Time;

	auto instance = getHInstance();

	// TODO: load game config

	// initialize platform
	Platform::initialize();

#ifdef _WIN32
	// create gamewindow
	WNDCLASSEX wnd;
	memset(&wnd, 0, sizeof(wnd));
	wnd.cbSize = sizeof(wnd);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = wndProc;
	wnd.hInstance = instance;
	wnd.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd.lpszClassName = L"recrafted";
	wnd.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	RegisterClassEx(&wnd);

	Platform::setGameWindow(CreateWindowW(L"recrafted", L"ReCrafted", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 1280, 720, NULL, NULL, instance, nullptr));

	ShowWindow(Platform::getGameWindow(), SW_MAXIMIZE);
#endif

#ifndef R3DTEST
	// initialize bgfx platform data
	bgfx::PlatformData pd;
	memset(&pd, 0, sizeof(pd));
	pd.nwh = Platform::getGameWindow();
	bgfx::setPlatformData(pd);
#else
	r3d::init();
	auto windowHandle = r3d::create_window(Platform::getGameWindow(), "r3d example");

	auto shader_handle = r3d::create_shader();
	r3d::compile_shader("test.shader", "test.bin");
	r3d::load_shader("test.bin", &shader_handle);

#endif

	// run
	m_running = true;

#ifndef R3DTEST
	// load game assets, initialize everything
	onLoad();
#endif

	// save start time
	auto lastTime = Platform::getMiliseconds();

	while (m_running) // main loop
	{
		// update time
		auto currentTime = Platform::getMiliseconds();
		Time::m_instance->m_deltaTime = 1 / ((currentTime - lastTime) / 1000.0);
		Time::m_instance->m_time = float(currentTime / 1000.0);
		lastTime = currentTime;

		// update input
		m_input->update(m_cursorX, m_cursorY);

		// process input
		MSG msg;
		msg.message = WM_NULL;
		while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

#ifndef R3DTEST
		// frame
		{
			onUpdate();
			onSimulate(); // TODO: fixed-time step `onSimulate` call
			onDraw(); GAMEBASE_CHECK_SHUTDOWN
		}
#else
		float ccolor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };

		// say that we are going to push some information about current frame(for all contexts)
		r3d::beginframe();
		
		// prepare for frame render
		r3d::window_makecurrent(&windowHandle); // set windowHandle as current context(already it is set, bu this is only for example here)
		r3d::use_renderbuffer(nullptr); // set default render buffer for the current context

		r3d::clear_color(ccolor);
		r3d::clear_depth();

		// end the frame, process and present the whole frame
		r3d::endframe();
#endif
	}

#ifndef R3DTEST
	// unload game assets
	onUnload();
#endif

	// destroy gamewindow
	DestroyWindow(Platform::getGameWindow());
}

void GameBase::shutdown()
{
	// dispose
	SafeDispose(m_input);
	SafeDispose(m_time);

	m_running = false;
}

#undef GAMEBASE_CHECK_SHUTDOWN