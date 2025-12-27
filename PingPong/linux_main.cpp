#include <chrono>
#include <thread>
#include <cstring>

#include "X11/Xlib.h"
#include "X11/XKBlib.h"
#include "X11/Xutil.h"

#include "RenderState.hpp"
#include "renderer.hpp"
#include "platform_comon.hpp"
#include "game.hpp"

static bool running = true;

static Input input;

#define process_button(b, vk) \
input.buttons[b].changed = false; \
if(strcmp(key, vk) == 0) \
{ \
	input.buttons[b].changed = is_down != input.buttons[b].is_down; \
	input.buttons[b].is_down = is_down; \
} \

XImage* create_render_image(Display* display, int screen, int x, int y)
{
	XImage* image = XCreateImage(
		display,
		XDefaultVisual(display, screen),
		XDefaultDepth(display, screen),
		ZPixmap,
		0,
		nullptr,
		x,
		y,
		32,
		0
	);
	RenderState& render_state = get_render_state();
	if(render_state.buffer)
	{
		free(render_state.buffer);
	}
	image->data = (char*)malloc(image->bytes_per_line * image->height);
	render_state.buffer = image->data;
	render_state.width = x;
	render_state.height = y;
	
	return image;
}

int main()
{
	Display* display = XOpenDisplay(nullptr);
	
	int screen = DefaultScreen(display);
	Window window = XCreateSimpleWindow(
		display,
		RootWindow(display, screen), 
		0,
		0,
		1280,
		720,
		0,
		0,
		0
	);
	XSelectInput(display, window, KeyPressMask | KeyReleaseMask | StructureNotifyMask);
	XMapWindow(display, window);

	GC gc = XCreateGC(display, window, 0, nullptr);
	XSetForeground(display, gc, XBlackPixel(display, screen));

	XImage* image = create_render_image(display, screen, 1280, 720);

	constexpr int FRAME_RATE = 60;
	float delta_time = 1.f / FRAME_RATE;
	std::chrono::duration<float> MAX_FRAME_TIME(delta_time);
	
	RenderState& render_state = get_render_state();

	while (running)
	{
		namespace chrono = std::chrono;

		auto frame_start = chrono::steady_clock::now();

		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}

		XEvent event{};
		while(XPending(display) > 0)
		{
			XNextEvent(display, &event);
			if(event.type == KeyPress || event.type == KeyRelease)
			{
				bool is_down = event.type == KeyPress;
				auto keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
				auto key = XKeysymToString(keysym);
				process_button(BUTTON_UP, "Up");
				process_button(BUTTON_DOWN, "Down");
				process_button(BUTTON_W, "w");
				process_button(BUTTON_S, "s");
				process_button(BUTTON_LEFT, "Left");
				process_button(BUTTON_RIGHT, "Right");
				process_button(BUTTON_ENTER, "Return");
				
				if(strcmp(key, "Escape") == 0)
				{
					running = false;
					break;
				}
			}
	
			if(event.type == ConfigureNotify)
			{
				auto x = event.xconfigure.width;
				auto y = event.xconfigure.height;
				image = create_render_image(event.xconfigure.display, screen, x, y);
				XPutImage(display, window, gc, image, 0, 0, 0, 0, render_state.width, render_state.height);
		
			}
		}

		simulate_game(input, delta_time);
		
		XPutImage(display, window, gc, image, 0, 0, 0, 0, render_state.width, render_state.height);

		std::this_thread::sleep_until(frame_start + MAX_FRAME_TIME);
		delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - frame_start).count() / 1000.f;
	}

	XDestroyImage(image);
	XFreeGC(display, gc);
	XDestroyWindow(display, window);
	XCloseDisplay(display);
	return 0;
}
