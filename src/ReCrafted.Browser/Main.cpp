
#include <internal/cef_win.h>
#include <cef_app.h>

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    CefMainArgs args(hInstance);

    const auto result = CefExecuteProcess(args, nullptr, nullptr);
    printf("ReCrafted.Browser CEF exit with code %d", result);
    return result;
}
