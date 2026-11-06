#if defined(OF_PC) && defined(POCKET_PORT)
/* Developer tool: render MIDI resources through SDLPoP's original OPL engine.
 * Run locally against user-supplied game data; never redistribute its output. */
#include "common.h"
#include "pocket_assets.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern short midi_playing;
extern int digi_unavailable;
static void put32(FILE *f, uint32_t x) { unsigned char b[4]={x,x>>8,x>>16,x>>24}; fwrite(b,1,4,f); }
int pocket_render_music(void) {
    const char *path = getenv("SDLPOP_RENDER_MUSIC");
    if (!path) return 0;
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) return -1;
    init_digi(); if (digi_unavailable) return -1;
    sound_flags = sfMidi | sfDigi;
    is_sound_on = 15; enable_music = 1;
    load_all_sounds();
    FILE *f=fopen(path,"wb+"); if (!f) return -1;
    fwrite("POPMUS01",1,8,f); put32(f,58);
    for (int i=0;i<58;i++) { put32(f,0); put32(f,0); }
    uint32_t offsets[58]={0}, lengths[58]={0};
    for (int i=0;i<58;i++) {
        if (!sound_pointers[i] || (sound_pointers[i]->type & 7) != sound_midi) continue;
        current_sound=i;
        play_midi_sound(sound_pointers[i]);
        if (!midi_playing) continue;
        offsets[i]=(uint32_t)ftell(f);
        unsigned frames=0;
        while (midi_playing && frames < 44100u*180u) {
            int16_t samples[1024*2]; memset(samples,0,sizeof samples);
            midi_callback(NULL,(Uint8*)samples,sizeof samples);
            for (int k=0;k<1024;k+=2) {
                int v=((int)samples[k*2]+samples[k*2+1])/2;
                unsigned char out[2]={(unsigned char)v,(unsigned char)((unsigned short)v>>8)};
                if (fwrite(out,1,2,f)!=2) { fclose(f); return -1; }
            }
            frames+=1024;
        }
        lengths[i]=(uint32_t)ftell(f)-offsets[i];
        fprintf(stderr,"Rendered sound %d: %.2fs\n",i,(double)frames/44100.0);
        if (midi_playing) { stop_midi(); fclose(f); return -1; }
    }
    fseek(f,12,SEEK_SET);
    for (int i=0;i<58;i++) { put32(f,offsets[i]); put32(f,lengths[i]); }
    if (fclose(f)!=0) return -1;
    return 1;
}
#endif
