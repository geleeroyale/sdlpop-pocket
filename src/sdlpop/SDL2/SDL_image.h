#ifndef POCKET_SDL_IMAGE_H
#define POCKET_SDL_IMAGE_H
#include <SDL2/SDL.h>
SDL_Surface *IMG_Load(const char *file);
SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc);
int IMG_SavePNG(SDL_Surface *surface, const char *file);
#define IMG_GetError SDL_GetError
#endif
