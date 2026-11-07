#ifndef POCKET_MUSIC_H
#define POCKET_MUSIC_H
#ifdef POCKET_MUSIC_TEST
#include "tests/music_test_types.h"
#else
#include "common.h"
#endif
int pocket_music_play(sound_buffer_type *buffer);
void pocket_music_mix(Uint8 *stream, int length);
void pocket_music_stop(void);
#endif
