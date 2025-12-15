#include <windows.h>
#include <stdio.h>
#include <cstdint>

#include "RenderState.hpp"
#include "renderer.hpp"
#include "platform_comon.hpp"
#include "game.hpp"

static bool running = true;

static Input input;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	auto& render_state = get_render_state();
	switch (uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			running = false;
			break;
		}
		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int buffer_size = render_state.width * render_state.height * sizeof(uint32_t);
			
			if (render_state.buffer)
			{
				VirtualFree(render_state.buffer, 0, MEM_RELEASE);
			}
			render_state.buffer = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			BITMAPINFO& buffer_bitmap_info = render_state.bitmap_info;
			buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
			buffer_bitmap_info.bmiHeader.biWidth = render_state.width;
			buffer_bitmap_info.bmiHeader.biHeight = render_state.height;
			buffer_bitmap_info.bmiHeader.biPlanes = 1;
			buffer_bitmap_info.bmiHeader.biBitCount = 32;
			buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
			break;
		}

		default:
		{
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	return result;
}

#define process_button(b, vk) \
case vk: { \
input.buttons[b].changed = is_down != input.buttons[b].is_down; \
input.buttons[b].is_down = is_down; \
} break


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS window_class{};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Game Window Class";
	window_class.lpfnWndProc = WindowProc;

	RegisterClass(&window_class);

	HWND window = CreateWindowA(
		window_class.lpszClassName,
		"Awesome Ping Pong",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1280,
		720,
		0,
		0,
		hInstance,
		0
	);
	
	// Fullscreen
	/*{
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}*/
	
	HDC hdc = GetDC(window);

	float delta_time = 1.0 / 60.0;
	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	auto& render_state = get_render_state();

	while (running)
	{

		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
				case WM_KEYUP:
				case WM_KEYDOWN:
				{
					uint32_t vk_code = (uint32_t)message.wParam;
					bool is_down = ((message.lParam & (1 << 31)) == 0);

					switch (vk_code)
					{
						process_button(BUTTON_UP, VK_UP);
						process_button(BUTTON_DOWN, VK_DOWN);
						process_button(BUTTON_W, 'W');
						process_button(BUTTON_S, 'S');
						process_button(BUTTON_LEFT, VK_LEFT);
						process_button(BUTTON_RIGHT, VK_RIGHT);
						process_button(BUTTON_ENTER, VK_RETURN);

						case VK_ESCAPE:
						{
							exit(0);
						}
					}
				} break;
			
				default:
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}

		simulate_game(input, delta_time);

		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.buffer, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;
	}
}