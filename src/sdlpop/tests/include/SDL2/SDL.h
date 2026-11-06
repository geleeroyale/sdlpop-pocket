#ifndef TEST_SDL_H
#define TEST_SDL_H
#include <stdint.h>
typedef uint8_t Uint8;
typedef uint32_t Uint32;
typedef struct { int x,y,w,h; } SDL_Rect;
typedef struct { unsigned char BytesPerPixel; } SDL_PixelFormat;
typedef struct { void *pixels; int pitch; SDL_PixelFormat *format; int w,h; SDL_Rect clip_rect; } SDL_Surface;
typedef void (*SDL_AudioCallback)(void *, Uint8 *, int);
typedef struct { int freq; uint16_t format; uint8_t channels; SDL_AudioCallback callback; void *userdata; } SDL_AudioSpec;
#define AUDIO_S16SYS 0x8010
void SDL_AudioPump(void);
#endif
