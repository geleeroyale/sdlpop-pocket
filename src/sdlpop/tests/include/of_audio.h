#ifndef TEST_AUDIO_H
#define TEST_AUDIO_H
#include <stdint.h>
void of_audio_init(void);
int of_audio_free(void);
int of_audio_stream_open(int rate);
void of_audio_stream_close(void);
int of_audio_write(const int16_t *samples, int count);
#endif
