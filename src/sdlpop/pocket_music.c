#if defined(POCKET_PORT) && !defined(OF_PC)
/* Pre-rendered user music: signed 16-bit mono at 22050 Hz, output at 44100 Hz. */
#include "pocket_music.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
extern short midi_playing;
static uint8_t *pcm;
static uint32_t pcm_length, position;
static unsigned repeat;
static uint32_t get32(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1]<<8 | (uint32_t)p[2]<<16 | (uint32_t)p[3]<<24;
}
void pocket_music_stop(void) {
    midi_playing = 0;
    free(pcm); pcm = NULL; pcm_length = 0; position = 0; repeat = 0;
}
int pocket_music_play(sound_buffer_type *buffer) {
    pocket_music_stop();
    int index = -1;
    for (int i=0; i<58; ++i) if (sound_pointers[i] == buffer) { index=i; break; }
    if (index < 0) return -1;
    FILE *f = fopen("popmusic.dat", "rb"); if (!f) return -1;
    uint8_t header[12], entry[8];
    if (fseek(f,0,SEEK_END)) { fclose(f); return -1; }
    long file_size = ftell(f);
    if (file_size < 476 || fseek(f,0,SEEK_SET) || fread(header,1,12,f)!=12 || memcmp(header,"POPMUS01",8) || get32(header+8)!=58 || fseek(f,12+index*8,SEEK_SET) || fread(entry,1,8,f)!=8) { fclose(f); return -1; }
    uint32_t offset=get32(entry), size=get32(entry+4);
    if (offset < 476 || size == 0 || (size & 1) || size > 16u*1024u*1024u || offset > (uint32_t)file_size || size > (uint32_t)file_size-offset || fseek(f,(long)offset,SEEK_SET)) { fclose(f); return -1; }
    pcm=malloc(size); if (!pcm) { fclose(f); return -1; }
    if (fread(pcm,1,size,f)!=size) { fclose(f); pocket_music_stop(); return -1; }
    fclose(f); pcm_length=size; position=0; repeat=0; midi_playing=1;
    return 0;
}
void pocket_music_mix(Uint8 *stream, int length) {
    int16_t *out=(int16_t *)stream;
    int frames=length/4;
    for (int i=0; i<frames && midi_playing; ++i) {
        if (position >= pcm_length) { midi_playing=0; break; }
        int16_t value=(int16_t)((uint16_t)pcm[position] | (uint16_t)pcm[position+1]<<8);
        for (int c=0;c<2;c++) {
            int sample=(int)out[2*i+c] + value;
            out[2*i+c]=(int16_t)(sample>32767?32767:sample< -32768? -32768:sample);
        }
        if (++repeat == 2) { repeat=0; position+=2; }
    }
    if (position >= pcm_length) midi_playing=0;
}
#endif
