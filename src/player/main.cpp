
#include <core/file.hpp>
#include <core/hashname.hpp>
#include <core/mmap_file.hpp>
#include <core/refpack.hpp>
#include <core/stringprintf.hpp>
#include <cstdio>
#include <lui/structs/lui_file.hpp>

#include <core/sdl/window.hpp>
#include <core/sdl/surface.hpp>
#include <core/sdl/texture.hpp>
#include <SDL2/SDL_ttf.h>

struct TtfFont {
    TtfFont(TTF_Font* pFont)
        : pFont(pFont) {
    }

    ~TtfFont() {
        if(pFont) { 
            TTF_CloseFont(pFont);
        }
    }

    operator bool() const {
        return pFont != nullptr;
    }

    operator TTF_Font* () {
        return pFont;
    }

private:
    TTF_Font* pFont;
};

int main(int argc, char** argv) {
#if 0
	auto mm = core::MmapFile::Open("/home/lily/ssx3ui/FE.LUI");
	auto view = mm.MapWholeView();
	auto viewSpan = view.GetView();

	auto* pLuiHeader = reinterpret_cast<lui::structs::Header*>(viewSpan.data());
    if(!pLuiHeader->Valid()) {
        printf("Invalid LUI file\n");
        return 1;
    }
#endif

	SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

	auto window = core::sdl::Window("UIPlayer", { 512, 448 });

	bool stop = false;

	window.On(SDL_QUIT, [&](auto& ev) {
		stop = true;
	});

    auto pArial12 = TtfFont(TTF_OpenFont("./arial.ttf", 24));
    if(!pArial12) {
        printf("???\n");
        return 1;
    }

    auto surf = core::sdl::Surface(TTF_RenderText_Blended(pArial12, "This is a test", {
        .r = 255,
        .g = 255,
        .b = 255,
        .a = 0
    }));

    if(surf.Raw() == nullptr) {
        printf("?????\n");
        return 1;
    }

    auto tex = core::sdl::Texture(surf, window.GetRenderer());
    auto texSize = tex.GetSize();

    auto texRect = SDL_Rect {
        .x = 0,
        .y = 0,
        .w = (i32)texSize.width,
        .h = (i32)texSize.height
    };

	while(!stop) {
        SDL_RenderClear(window.GetRenderer());


        SDL_RenderCopy(window.GetRenderer(), tex.Raw(), nullptr, &texRect);
        


		window.Poll();
	}

    // Clean up SDL
    TTF_Quit();
    SDL_Quit();
	return 0;
}