#if defined(POCKET_PORT) && !defined(OF_PC)
/* SDLPoP draws RGB surfaces; present them through the Pocket's RGB565 mode. */
#include "pocket_video.h"
#include "pocket_audio.h"
#include <of_video.h>
#include <stdint.h>
#include <string.h>
int pocket_video_present(SDL_Surface *source) {
    static int initialized;
    static of_video_mode_t mode;
    if (!source || !source->pixels || source->format->BytesPerPixel != 4) return -1;
    if (!initialized) {
        of_video_mode_t wanted = {320, 240, 0, OF_VIDEO_MODE_RGB565, 0};
        if (of_video_set_mode(&wanted) != 0) return -1;
        of_video_get_mode(&mode);
        if (mode.width != 320 || mode.height != 240 || mode.color_mode != OF_VIDEO_MODE_RGB565) return -1;
        initialized = 1;
    }
    uint8_t *frame = of_video_surface();
    if (!frame) return -1;
    unsigned stride = mode.stride ? mode.stride : (unsigned)mode.width * 2;
    for (unsigned y = 0; y < mode.height; ++y) {
        uint16_t *dst = (uint16_t *)(frame + y * stride);
        if (y < 20 || y >= 220) { memset(dst, 0, 320 * sizeof(*dst)); continue; }
        const uint32_t *src = (const uint32_t *)((const uint8_t *)source->pixels + (y-20) * source->pitch);
        for (unsigned x = 0; x < 320; ++x) {
            uint32_t p = src[x];
            dst[x] = (uint16_t)(((p >> 8) & 0xf800) | ((p >> 5) & 0x07e0) | ((p >> 3) & 0x001f));
        }
    }
    of_video_flip();
    pocket_audio_pump();
    return 0;
}

#endif
