// ReCrafted (c) 2016-2018 Always Too Late

#include "Input/InputManager.h"
#if _WIN32

#include <Windows.h>

#include "ReCrafted.h"
#include "Common/Input/Input.h"
#include "Common/Input/KeyboardBuffer.h"
#include "Core/EngineMain.h"
#include "Core/Logger.h"

#include "imgui.h"

extern HICON m_currentCursor;

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowEventProcessor(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wparam, lparam))
        return 0;

    switch (msg)
    {
    case WM_GETMINMAXINFO:
        {
            cvar lpMMI = reinterpret_cast<LPMINMAXINFO>(lparam);
            lpMMI->ptMinTrackSize.x = 800;
            lpMMI->ptMinTrackSize.y = 600;
        }
    /*case WM_SETCURSOR:
        {
            if (LOWORD(lparam) == HTCLIENT)
            {
                if (Input::GetShowCursor())
                {
                    //SetCursor(m_currentCursor);
                    ShowCursor(true);
                }
                else
                {
                    ShowCursor(false);
                    //SetCursor(nullptr);
                }
            }
            //return 0;
            break;
        }*/
    case WM_SIZE:
        {
            if (wparam == 2)
                ApplicationWindow::WindowResize(hWnd);

            return 0;
        }

    case WM_EXITSIZEMOVE: // handle window resizing
        {
            ApplicationWindow::WindowResize(hWnd);
            return 0;
        }

    case WM_CLOSE: // handle window closing
        {
            EngineMain::GetInstance()->Quit();
            return 0;
        }

    case WM_CHAR:
        {
            KeyboardBuffer::Write(static_cast<Char>(wparam));
            InputManager::GetDevice<Keyboard>(DeviceType::Keyboard).EmitCharacter(Char(wparam));
            return 0;
        }

        // input handling
    case WM_KEYDOWN:
        {
            Input::GetInstance()->OnKeyEvent(false, uint(wparam));
            InputManager::GetDevice<Keyboard>(DeviceType::Keyboard).EmitInput(Key(wparam), KeyState::Down);
            return 0;
        }
    case WM_KEYUP:
        {
            Input::GetInstance()->OnKeyEvent(true, uint(wparam));
            InputManager::GetDevice<Keyboard>(DeviceType::Keyboard).EmitInput(Key(wparam), KeyState::Up);
            return 0;
        }
    case WM_CREATE:
        {
            // register for raw input
            // source: https://msdn.microsoft.com/pl-pl/library/windows/desktop/ms645546(v=vs.85).aspx#example_1
            RAWINPUTDEVICE Rid[1];

            Rid[0].usUsagePage = 0x01;
            Rid[0].usUsage = 0x02;
            Rid[0].dwFlags = 0;
            Rid[0].hwndTarget = static_cast<HWND>(Platform::GetCurrentWindow());

            if (RegisterRawInputDevices(Rid, 1, sizeof Rid[0]) == FALSE)
            {
                // error, failed to register
                Logger::LogError("Failed to register mouse for RAW Input");
                return 0;
            }
            return 0;
        }
    case WM_INPUT:
        {
            UINT dwSize;
            GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

            static LPBYTE lpb[40] = {};
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
                return DefWindowProc(hWnd, msg, wparam, lparam);

            auto raw = reinterpret_cast<RAWINPUT*>(lpb);
            
            if (raw->header.dwType == RIM_TYPEMOUSE)
            {
                rvar mouseDevice = InputManager::GetDevice<Mouse>(DeviceType::Mouse);

                if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
                {
                    // Update cursor pos
                    cvar currentWindow = Platform::GetCurrentWindow();

                    if (currentWindow)
                    {
                        POINT cursorPos;
                        GetCursorPos(&cursorPos);

                        ScreenToClient(static_cast<HWND>(currentWindow), &cursorPos);

                        Input::GetInstance()->OnCursorEvent(raw->data.mouse.lLastX, raw->data.mouse.lLastY, cursorPos.x,
                                                            cursorPos.y);

                        mouseDevice.EmitCursor(
                            Vector2(cursorPos.x, cursorPos.y), 
                            Vector2(raw->data.mouse.lLastX, raw->data.mouse.lLastY)
                        );
                    }
                    else 
                    {
                        Input::GetInstance()->OnCursorEvent(raw->data.mouse.lLastX, raw->data.mouse.lLastY, 0, 0);

                        mouseDevice.EmitCursor(
                            Vector2::Zero,
                            Vector2(raw->data.mouse.lLastX, raw->data.mouse.lLastY)
                        );
                    }
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_WHEEL)
                {
                    cvar scrollDelta = static_cast<SHORT>(raw->data.mouse.usButtonData) / static_cast<float>(WHEEL_DELTA);
                    
                    Input::GetInstance()->OnScrollEvent(scrollDelta);

                    mouseDevice.EmitScroll(scrollDelta);
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_DOWN)
                {
                    Input::GetInstance()->OnKeyEvent(false, INPUT_LBUTTON);
                    mouseDevice.EmitInput(Button::Left, ButtonState::Down);
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_UP)
                {
                    Input::GetInstance()->OnKeyEvent(true, INPUT_LBUTTON);
                    mouseDevice.EmitInput(Button::Left, ButtonState::Up);
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_DOWN)
                {
                    Input::GetInstance()->OnKeyEvent(false, INPUT_MBUTTON);
                    mouseDevice.EmitInput(Button::Middle, ButtonState::Down);
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_UP)
                {
                    Input::GetInstance()->OnKeyEvent(true, INPUT_MBUTTON);
                    mouseDevice.EmitInput(Button::Middle, ButtonState::Up);
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_DOWN)
                {
                    Input::GetInstance()->OnKeyEvent(false, INPUT_RBUTTON);
                    mouseDevice.EmitInput(Button::Right, ButtonState::Down);
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_UP)
                {
                    Input::GetInstance()->OnKeyEvent(true, INPUT_RBUTTON);
                    mouseDevice.EmitInput(Button::Right, ButtonState::Up);
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_4_DOWN)
                {
                    mouseDevice.EmitInput(Button::X1, ButtonState::Down);
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_4_UP)
                {
                    mouseDevice.EmitInput(Button::X1, ButtonState::Up);
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_5_DOWN)
                {
                    mouseDevice.EmitInput(Button::X2, ButtonState::Down);
                }

                if (raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_5_UP)
                {
                    mouseDevice.EmitInput(Button::X2, ButtonState::Up);
                }
            }

            return DefWindowProc(hWnd, msg, wparam, lparam);
        }

        // default
    default:
        return DefWindowProc(hWnd, msg, wparam, lparam);
    }

    return DefWindowProc(hWnd, msg, wparam, lparam);
}

#endif
