#if defined(POCKET_PORT) && !defined(OF_PC)
/* Bound queued sound to reduce latency. Called cooperatively by the game loop. */
#include "pocket_audio.h"
#include <of_audio.h>
#include <stdint.h>
static SDL_AudioCallback callback;
static void *callback_data;
static int capacity, paused = 1, in_pump;
int pocket_audio_open(SDL_AudioSpec *desired, SDL_AudioSpec *obtained) {
    if (!desired || !desired->callback || desired->format != AUDIO_S16SYS || desired->channels != 2) return -1;
    of_audio_init();
    capacity = of_audio_free();
    if (capacity <= 0 || of_audio_stream_open(desired->freq) < 0) return -1;
    callback = desired->callback; callback_data = desired->userdata; paused = 1;
    if (obtained) *obtained = *desired;
    return 0;
}
void pocket_audio_pause(int pause_on) { paused = pause_on; }
void pocket_audio_close(void) { paused = 1; callback = 0; of_audio_stream_close(); }
void pocket_audio_pump(void) {
    if (!callback || paused || in_pump) return;
    in_pump = 1;
    int available = of_audio_free();
    int queued = capacity - available;
    int frames = 2048 - (queued > 0 ? queued : 0);
    if (frames > available) frames = available;
    int16_t samples[512 * 2];
    while (frames > 0 && !paused) {
        int n = frames > 512 ? 512 : frames;
        callback(callback_data, (Uint8 *)samples, n * 4);
        if (of_audio_write(samples, n) < 0) break;
        frames -= n;
    }
    in_pump = 0;
}
#endif
