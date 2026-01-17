#include "window.h"
using namespace std;

Window* window;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		 case WM_DESTROY:                  // completely destroys window, clean up
		 {
			 PostQuitMessage(0);
			 Window* wnd = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			 if (wnd) wnd->shouldClose = true;
			 return 0;
		 }

		 case WM_CLOSE:                    // closes window, hey can you close now?
		 {
			 PostQuitMessage(0);
			 Window* wnd = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			 if (wnd) wnd->shouldClose = true;
			 return 0;
		 }

		case WM_KEYDOWN:
		{
			 Window* wnd = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			 if (wnd) wnd->keys[(unsigned int)wParam] = true;
			return 0;
		}

		case WM_KEYUP:
		{
			 Window* wnd = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			 if (wnd) wnd->keys[(unsigned int)wParam] = false;
			return 0;
		}

		// Mouse position when on window
		case WM_LBUTTONDOWN:
		{
			 Window* wnd = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			 if (wnd) {
				 wnd->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
				 wnd->mouseButtons[0] = true;
			 }
			return 0;
		}

		case WM_LBUTTONUP:
		{
			window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
			window->mouseButtons[0] = false;
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			 Window* wnd = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			 if (wnd) wnd->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
			return 0;
		}

		default:
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

	}
}



void Window::initialize(std::string window_name, int window_width, int window_height)   // window x and window y are 0, 0
{
    WNDCLASSEX wc;
	hinstance = GetModuleHandle(NULL);
	name = window_name;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    std::wstring wname = std::wstring(name.begin(), name.end());
    wc.lpszClassName = wname.c_str();
    if (!RegisterClassEx(&wc)) {
        MessageBoxA(NULL, "Failed to register window class.", "Window Error", MB_OK | MB_ICONERROR);
        return;
    }

	width = window_width;
	height = window_height;
    DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    hwnd = CreateWindowEx(WS_EX_APPWINDOW, wname.c_str(), wname.c_str(), style,
        0, 0, width, height, NULL, NULL, hinstance, this);

    if (!hwnd) {
        MessageBoxA(NULL, "Failed to create window.", "Window Error", MB_OK | MB_ICONERROR);
        return;
    }

    // store pointer for later retrieval in WndProc
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

    window = this;

    // initialize input arrays
    ZeroMemory(keys, sizeof(keys));
    ZeroMemory(mouseButtons, sizeof(mouseButtons));
    mousex = mousey = 0;

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

}

void Window::processMessages() 
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
        if (msg.message == WM_QUIT) {
            shouldClose = true;
        }
	}
}

// Tells windows and NVIDIA to prefer the dedicated NVIDIA GPU
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}