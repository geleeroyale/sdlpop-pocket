/* Placeholder: DAT images are decoded by SDLPoP; PNG extras are unsupported. */
#include "SDL2/SDL_image.h"
SDL_Surface *IMG_Load(const char *file) { (void)file; return NULL; }
SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc) { (void)src; (void)freesrc; return NULL; }
int IMG_SavePNG(SDL_Surface *surface, const char *file) { (void)surface; (void)file; return -1; }
