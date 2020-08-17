// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Application.h"
#include "Common/Logger.h"
#include "Input/InputManager.h"

#ifdef _WIN32

#include <Windows.h>

uint64_t EventProcessor(void* window, const uint32_t msg, const uint64_t param0, const uint64_t param1)
{
    const auto hWnd = static_cast<HWND>(window);
    const auto wParam = static_cast<WPARAM>(param0);
    const auto lParam = static_cast<LPARAM>(param1);

    switch (msg)
    {
    case WM_GETMINMAXINFO:
    {
        const auto lpMMI = reinterpret_cast<LPMINMAXINFO>(lParam);
        lpMMI->ptMinTrackSize.x = 800;
        lpMMI->ptMinTrackSize.y = 600;
    }
    /*case WM_SETCURSOR:
        {
            if (LOWORD(lParam) == HTCLIENT)
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
        if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
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
        Application::Quit();
        return 0;
    }

    case WM_CHAR:
    {
        InputManager::GetInstance()->BeginEmitInput();
        InputData data{};
        data.character = Char(wParam);
        InputManager::GetInstance()->EmitInput(InputType::Character, data);
        InputManager::GetInstance()->EndEmitInput();
        return 0;
    }

    // input handling
    case WM_KEYDOWN:
    {
        InputManager::GetInstance()->BeginEmitInput();
        InputData data{};
        data.key = Key(wParam);
        data.keyState = KeyState::Down;
        InputManager::GetInstance()->EmitInput(InputType::Key, data);
        InputManager::GetInstance()->EndEmitInput();
        return 0;
    }
    case WM_KEYUP:
    {
        InputManager::GetInstance()->BeginEmitInput();
        InputData data{};
        data.key = Key(wParam);
        data.keyState = KeyState::Up;
        InputManager::GetInstance()->EmitInput(InputType::Key, data);
        InputManager::GetInstance()->EndEmitInput();
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
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

        static LPBYTE lpb[40] = {};
        if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
            return DefWindowProc(hWnd, msg, wParam, lParam);

        auto raw = reinterpret_cast<RAWINPUT*>(lpb);

        InputManager::GetInstance()->BeginEmitInput();
        if (raw->header.dwType == RIM_TYPEMOUSE)
        {
            InputData inputData{};

            if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
            {
                // Update cursor pos
                const auto currentWindow = Platform::GetCurrentWindow();

                if (currentWindow)
                {
                    POINT cursorPos;
                    GetCursorPos(&cursorPos);

                    ScreenToClient(static_cast<HWND>(currentWindow), &cursorPos);

                    inputData.axis2D = Vector2(float(cursorPos.x), float(cursorPos.y));
                    inputData.axis3D = Vector3(float(raw->data.mouse.lLastX), float(raw->data.mouse.lLastY), 0.0f);
                    InputManager::GetInstance()->EmitInput(InputType::Cursor, inputData);
                }
                else
                {
                    inputData.axis2D = Vector2::Zero;
                    inputData.axis3D = Vector3(float(raw->data.mouse.lLastX), float(raw->data.mouse.lLastY), 0.0f);
                    InputManager::GetInstance()->EmitInput(InputType::Cursor, inputData);
                }
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_WHEEL)
            {
                const auto scrollDelta = static_cast<SHORT>(raw->data.mouse.usButtonData) / static_cast<float>(WHEEL_DELTA);

                inputData.axis1D = scrollDelta;
                InputManager::GetInstance()->EmitInput(InputType::Scroll, inputData);
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_DOWN)
            {
                inputData.button = Button::Left;
                inputData.buttonState = ButtonState::Down;
                InputManager::GetInstance()->EmitInput(InputType::Button, inputData);
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_UP)
            {
                inputData.button = Button::Left;
                inputData.buttonState = ButtonState::Up;
                InputManager::GetInstance()->EmitInput(InputType::Button, inputData);
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_DOWN)
            {
                inputData.button = Button::Middle;
                inputData.buttonState = ButtonState::Down;
                InputManager::GetInstance()->EmitInput(InputType::Button, inputData);
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_UP)
            {
                inputData.button = Button::Middle;
                inputData.buttonState = ButtonState::Up;
                InputManager::GetInstance()->EmitInput(InputType::Button, inputData);
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_DOWN)
            {
                inputData.button = Button::Right;
                inputData.buttonState = ButtonState::Down;
                InputManager::GetInstance()->EmitInput(InputType::Button, inputData);
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_UP)
            {
                inputData.button = Button::Right;
                inputData.buttonState = ButtonState::Up;
                InputManager::GetInstance()->EmitInput(InputType::Button, inputData);
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_4_DOWN)
            {
                inputData.button = Button::X1;
                inputData.buttonState = ButtonState::Down;
                InputManager::GetInstance()->EmitInput(InputType::Button, inputData);
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_4_UP)
            {
                inputData.button = Button::X1;
                inputData.buttonState = ButtonState::Up;
                InputManager::GetInstance()->EmitInput(InputType::Button, inputData);
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_5_DOWN)
            {
                inputData.button = Button::X2;
                inputData.buttonState = ButtonState::Down;
                InputManager::GetInstance()->EmitInput(InputType::Button, inputData);
            }

            if (raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_5_UP)
            {
                inputData.button = Button::X2;
                inputData.buttonState = ButtonState::Up;
                InputManager::GetInstance()->EmitInput(InputType::Button, inputData);
            }
        }
        InputManager::GetInstance()->EndEmitInput();
        
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    // default
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

#endif