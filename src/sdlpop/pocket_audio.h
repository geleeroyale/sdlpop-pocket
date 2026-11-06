#ifndef POCKET_AUDIO_H
#define POCKET_AUDIO_H
#include <SDL2/SDL.h>
int pocket_audio_open(SDL_AudioSpec *desired, SDL_AudioSpec *obtained);
void pocket_audio_pause(int pause_on);
void pocket_audio_close(void);
void pocket_audio_pump(void);
#endif
