#pragma once
#include <SDL2/SDL.h>
#include <SDL_render.h>

#include "core/gfxtypes.hpp"
#include "core/types.hpp"
#include "surface.hpp"

namespace core::sdl {

	struct Texture {
		Texture(Surface& surface, SDL_Renderer* renderer) {
			texture = SDL_CreateTextureFromSurface(renderer, surface.Raw());
		}

		~Texture() {
            if(texture)
                SDL_DestroyTexture(texture);
        }

		core::Size<u32> GetSize() {
            int w = 0;
            int h = 0;
            SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
            return {(u32)w, (u32)h};
        }

		SDL_Texture* Raw() { return texture; } 

	   private:
		SDL_Texture* texture;
	};

} // namespace core::sdl