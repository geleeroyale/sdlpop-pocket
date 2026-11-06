#if defined(POCKET_PORT) && !defined(OF_PC)
#include "pocket_mono.h"
#include <stdint.h>
int pocket_blit_mono(SDL_Surface *src, SDL_Surface *dst, int x, int y, Uint32 color) {
    if (!src || !dst || !src->pixels || !dst->pixels || src->format->BytesPerPixel != 1 || dst->format->BytesPerPixel != 4) return -1;
    for (int sy = 0; sy < src->h; ++sy) {
        int dy = y + sy;
        if (dy < 0 || dy >= dst->h || dy < dst->clip_rect.y || dy >= dst->clip_rect.y + dst->clip_rect.h) continue;
        const Uint8 *s = (const Uint8 *)src->pixels + sy * src->pitch;
        Uint32 *d = (Uint32 *)((Uint8 *)dst->pixels + dy * dst->pitch);
        for (int sx = 0; sx < src->w; ++sx) {
            int dx = x + sx;
            if (dx >= 0 && dx < dst->w && dx >= dst->clip_rect.x && dx < dst->clip_rect.x + dst->clip_rect.w && s[sx] != 0) d[dx] = color;
        }
    }
    return 0;
}
#endif
