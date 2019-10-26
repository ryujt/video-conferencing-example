#ifndef RYU_SDL_WINDOW_HPP
#define RYU_SDL_WINDOW_HPP

#include <SDL2/SDL.h>

using namespace std;

class WindowSDL {
public:
	bool open(string caption, int width, int height)
	{
		close();

		width_ = width;
		height_ = height;

		SDL_Init(SDL_INIT_EVERYTHING);
		window_ = SDL_CreateWindow(
			caption.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_RESIZABLE
		);
		renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

		is_opened_ = true;
		return true;
	}

	void close()
	{
		if (is_opened_) {
			is_opened_ = false;
			SDL_DestroyWindow(window_);
			SDL_Quit();
		}
	}

	void showBitmap32(void* bitmap)
	{
		surface_ = SDL_CreateRGBSurfaceFrom(bitmap, width_, height_, 32, width_*4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		texture_ = SDL_CreateTextureFromSurface(renderer_, surface_);
		try {
			SDL_RenderCopy(renderer_, texture_, NULL, NULL);
			SDL_RenderPresent(renderer_);
		} catch (...) {
			//
		}
		SDL_FreeSurface(surface_);
		SDL_DestroyTexture(texture_);
	}

private:
	bool is_opened_ = false;

	int width_;
	int height_;

	SDL_Window* window_;
	SDL_Renderer* renderer_;
	SDL_Surface* surface_;
	SDL_Texture* texture_;
};

#endif  // RYU_SDL_WINDOW_HPP