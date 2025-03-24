#include <core/sdl/window.hpp>

#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>

namespace core::sdl {

	Window::Window(const std::string_view title, const SizeT& size) {
		Create(title, size);
	}

	Window::~Window() {
		Destroy();
	}

	void Window::Create(const std::string_view title, const SizeT& size) {
		// TODO Error handle
		if(!window) {
			window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.width, size.height, SDL_WINDOW_SHOWN);
			renderer = SDL_CreateRenderer(window, -1, 0);
		}
	}

	void Window::Destroy() {
		if(window) {
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			window = nullptr;
			renderer = nullptr;
		}
	}

	SDL_Renderer* Window::GetRenderer() {
		return renderer;
	}


	void Window::Poll() {
		SDL_Event ev;

		SDL_RenderPresent(renderer);
		while(SDL_PollEvent(&ev) > 0) {
			// Call the given event handler if found
			if(events.find(ev.type) != events.end()) {
				events[ev.type](ev);
			}
		}
	}

	void Window::On(SDL_EventType ev, const EventHandlerFn& fn) {
		events[ev] = fn;
	}

	SDL_Rect Window::GetRect() {
		SDL_Rect rect {};
		SDL_GetWindowSize(window, &rect.w, &rect.h);
		return rect;
	}

	void Window::Resize(const SizeT& size) {
		SDL_SetWindowSize(window, size.width, size.height);
	}

} // namespace vncc::sdl