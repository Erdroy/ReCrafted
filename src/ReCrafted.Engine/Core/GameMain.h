// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef GAMEMAIN_H
#define GAMEMAIN_H

// includes
#include "recraftedConfig.h"
#include "Common/Input/Input.h"
#include "Common/Time.h"
#include "Game/Universe.h"
#include "Graphics/Rendering.h"
#include "Graphics/Camera.h"
#include "Graphics/UI/UI.h"
#include "Scripting/Domain.h"
#include "Scripting/Method.h"
#include "Common/EntityPool.h"
#include "Logger.h"

class GameMain
{
	API_DEF

	friend class Rendering;

private:
	static GameMain* m_instance;

private:
	volatile bool m_running = false;
	bool m_initialized = false;
	bool m_wireframe = false;

	EntityPool* m_entityPool = nullptr;
	Rendering* m_rendering = nullptr;
	Universe* m_universe = nullptr;
	Logger* m_logger = nullptr;
	Input* m_input = nullptr;
	Time* m_time = nullptr;
	UI* m_ui = nullptr;

	bool m_showCursor = true;
	bool m_lockCursor = true;

	double m_simulationAcc = 0.0;

	int m_targetFps = 60;

#if _WIN32
	HCURSOR m_currentCursor = nullptr;
	int m_currentCursorInt = 0;
#endif

	// scripting

	Ptr<Domain> m_domain = nullptr;
	Ptr<Object> m_gamemain = nullptr;

	// methods

	Ptr<Method> m_init_method = nullptr;
	Ptr<Method> m_update_method = nullptr;
	Ptr<Method> m_simulate_method = nullptr;
	Ptr<Method> m_drawui_method = nullptr;
	Ptr<Method> m_shutdown_method = nullptr;

private:
	void initScripting();
	void waitForTargetFps(double last);
	void runEvents();
	void simulate();

public:
	/// <summary>
	/// Run game
	/// </summary>
	void run();

	/// <summary>
	/// Shutdown the game
	/// </summary>
	void shutdown();

public:
	void onLoad();
	void onUnload();
	void onResize(uint width, uint height);
	void onUpdate();
	void onSimulate();
	void onDraw();

public:	   
	void onCursorRequest();

public:
	/// <summary>
	/// Shutdowns the game.
	/// </summary>
	FORCEINLINE static void quit()
	{
		m_instance->m_running = false;
	}

	/// <summary>
	/// Sets the cursor state.
	/// </summary>
	FORCEINLINE static void setShowCursor(bool state)
	{
		m_instance->m_showCursor = state;
	}

	/// <summary>
	/// Gets the cursor state.
	/// </summary>
	FORCEINLINE static bool getShowCursor()
	{
		return m_instance->m_showCursor;
	}

	/// <summary>
	/// Sets the cursor lock mode.
	/// </summary>
	FORCEINLINE static void setLockCursor(bool state)
	{
		m_instance->m_lockCursor = state;
	}

	/// <summary>
	/// Gets the cursor lock mode.
	/// </summary>
	FORCEINLINE static bool getLockCursor()
	{
		return m_instance->m_lockCursor;
	}

	/**
	 * \brief Sets the target FPS.
	 * \param targetFps The target FPS value.
	 */
	FORCEINLINE static void setTargetFps(int targetFps)
	{
		m_instance->m_targetFps = targetFps;
	}

	/**
	* \brief Gets the target FPS.
	 */
	FORCEINLINE static int getTargetFps()
	{
		return m_instance->m_targetFps;
	}

	/// <summary>
	/// Check if the engine is running.
	/// </summary>
	/// <returns>Returns true when is running.</returns>
	FORCEINLINE static bool isRunning()
	{
		return m_instance->m_running;
	}

	/// <summary>
	/// Sets the cursor icon.
	/// </summary>
	FORCEINLINE static void setCursorIcon(int icon)
	{
		switch (icon)
		{
			case 1:
				m_instance->m_currentCursor = LoadCursor(nullptr, IDC_IBEAM);
				break;
			default:
				m_instance->m_currentCursor = LoadCursor(nullptr, IDC_ARROW);
				break;
		}
		m_instance->m_currentCursorInt = icon;
	}

	/// <summary>
	/// Gets the cursor icon int.
	/// </summary>
	FORCEINLINE static int getCursorIcon()
	{
		return m_instance->m_currentCursorInt;
	}

	/// <summary>
	/// Sets clipboard data.
	/// </summary>
	FORCEINLINE static void setClipboardData(MonoString* string)
	{
		MONO_ANSI_ERR();
		auto str = MONO_ANSI(string);
		auto s = std::string(str);
		if (s.empty())
		{ 
			MONO_ANSI_FREE(str);
			return;
		}
		OpenClipboard(0);
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
		if (!hg) {
			CloseClipboard();
			MONO_ANSI_FREE(str);
			return;
		}
		memcpy(GlobalLock(hg), s.c_str(), s.size());
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT, hg);
		CloseClipboard();
		GlobalFree(hg);
		MONO_ANSI_FREE(str);
	}


	/// <summary>
	/// Gets clipboard data.
	/// </summary>
	FORCEINLINE static MonoString* getClipboardData()
	{
		std::string text = "";
		if (OpenClipboard(nullptr))
		{
			HANDLE hData = GetClipboardData(CF_TEXT);
			if (hData != nullptr)
			{
				char * pszText = static_cast<char*>(GlobalLock(hData));
				if (pszText != nullptr)
				{
					text = pszText;
					GlobalUnlock(hData);
					CloseClipboard();
				}
			}
		}

		return mono_string_new(mono_domain_get(), text.c_str());
	}
};

#endif // GAMEMAIN_H
