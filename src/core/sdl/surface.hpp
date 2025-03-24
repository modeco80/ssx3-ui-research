#pragma once
#include <SDL2/SDL.h>

namespace core::sdl {

	struct Surface {
		Surface(SDL_Surface* pSurface)
			: surf(pSurface) {
		}

		~Surface() {
			if(surf)
				SDL_FreeSurface(surf);
		}

		auto* Raw() {
			return surf;
		}

	   private:
		SDL_Surface* surf;
	};

} // namespace core::sdl