#ifndef POCKET_SDL_COMPAT_H
#define POCKET_SDL_COMPAT_H
#ifndef SDL_COMPILE_TIME_ASSERT
#define SDL_COMPILE_TIME_ASSERT(name, x) typedef char sdl_static_assert_##name[(x) ? 1 : -1]
#endif
/* SDK surfaces are 32-bit RGB; keep streaming textures in the same format. */
#define SDL_PIXELFORMAT_RGB24 SDL_PIXELFORMAT_RGB888
#define SDL_SCANCODE_APPLICATION ((SDL_Scancode)101)
#define SDL_SCANCODE_MUTE ((SDL_Scancode)127)
#define SDL_SCANCODE_VOLUMEUP ((SDL_Scancode)128)
#define SDL_SCANCODE_VOLUMEDOWN ((SDL_Scancode)129)
#define SDL_SCANCODE_AUDIOMUTE ((SDL_Scancode)262)
#define SDL_SCANCODE_CLEAR ((SDL_Scancode)156)

#define SDL_ISPIXELFORMAT_INDEXED(format) ((format) == SDL_PIXELFORMAT_INDEX8)
static inline int SDL_GameControllerAddMappingsFromFile(const char *path) { (void)path; return 0; }
static inline int SDL_GameControllerRumble(SDL_GameController *p, Uint16 l, Uint16 h, Uint32 ms) { (void)p; (void)l; (void)h; (void)ms; return -1; }
static inline int SDL_JoystickRumble(SDL_Joystick *p, Uint16 l, Uint16 h, Uint32 ms) { (void)p; (void)l; (void)h; (void)ms; return -1; }
#include "pocket_audio.h"
#define SDL_OpenAudio pocket_audio_open
#define SDL_PauseAudio pocket_audio_pause
#define SDL_CloseAudio pocket_audio_close
typedef struct SDL_Haptic SDL_Haptic;
#define SDL_INIT_HAPTIC 0
static inline SDL_Haptic *SDL_HapticOpen(int n) { (void)n; return NULL; }
static inline int SDL_HapticRumbleInit(SDL_Haptic *h) { (void)h; return -1; }
static inline int SDL_HapticRumblePlay(SDL_Haptic *h, float strength, Uint32 length) { (void)h; (void)strength; (void)length; return -1; }
#endif
