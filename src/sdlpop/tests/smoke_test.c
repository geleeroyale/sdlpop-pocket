#include "pocket_video.h"
#include "pocket_assets.h"
#include "pocket_mono.h"
#include "pocket_audio.h"
#include <of_video.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static uint16_t frame[320*240];
static uint32_t pixels[320*200];
static int flips, queued, calls, rate;
int of_video_set_mode(const of_video_mode_t *m) { return !(m->width == 320 && m->height == 240 && m->color_mode == OF_VIDEO_MODE_RGB565); }
void of_video_get_mode(of_video_mode_t *m) { *m = (of_video_mode_t){320,240,640,OF_VIDEO_MODE_RGB565,0}; }
uint8_t *of_video_surface(void) { return (uint8_t *)frame; }
void of_video_flip(void) { ++flips; }
void of_audio_init(void) { queued=0; }
int of_audio_free(void) { return 131072-queued; }
int of_audio_stream_open(int r) { rate=r; return 0; }
void of_audio_stream_close(void) {}
int of_audio_write(const int16_t *p, int n) { (void)p; queued+=n; return n; }
static void callback(void *u, Uint8 *stream, int len) { (void)u; memset(stream,0,len); ++calls; }
int main(void) {
    SDL_PixelFormat fmt = {4}; SDL_Surface s = {pixels, 320*4, &fmt, 320, 200, {0,0,320,200}};
    pixels[0]=0xff0000; pixels[1]=0x00ff00; pixels[2]=0x0000ff; pixels[3]=0xffffff;
    memset(frame, 0xff, sizeof frame);
    assert(pocket_video_present(&s) == 0);
    assert(frame[0]==0 && frame[20*320]==0xf800 && frame[20*320+1]==0x07e0 && frame[20*320+2]==0x001f && frame[20*320+3]==0xffff && frame[239*320]==0 && flips==1);
    unsigned char bitmap[]={0,1,1,0}; uint32_t out[]={7,7,7,7}; SDL_PixelFormat f8={1};
    SDL_Surface glyph={bitmap,2,&f8,2,2,{0,0,2,2}}, dst={out,8,&fmt,2,2,{0,0,2,2}};
    assert(pocket_blit_mono(&glyph,&dst,0,0,99)==0 && out[0]==7 && out[1]==99 && out[2]==99 && out[3]==7);
    out[0]=out[1]=out[2]=out[3]=7; dst.clip_rect=(SDL_Rect){1,0,1,2};
    assert(pocket_blit_mono(&glyph,&dst,0,0,99)==0 && out[0]==7 && out[1]==99 && out[2]==7 && out[3]==7);
    SDL_AudioSpec spec={44100,AUDIO_S16SYS,2,callback,NULL};
    assert(pocket_audio_open(&spec,NULL)==0 && rate==44100); pocket_audio_pause(0); pocket_audio_pump();
    assert(queued==2048 && calls==4); pocket_audio_pump(); assert(queued==2048 && calls==4);
    queued-=512; pocket_audio_pump(); assert(queued==2048 && calls==5);
    pocket_audio_pause(1); queued=0; pocket_audio_pump(); assert(queued==0);
    assert(pocket_assets_init()==0);
    FILE *f = pocket_open_dat("prince.dat"); assert(f); unsigned char bytes[6]; assert(fread(bytes,1,6,f)==6); fclose(f);
    assert(pocket_open_dat("NOT-EXISTS.DAT")==NULL);
    puts("RGB565, transparent clipped glyphs, bounded audio queue, and DAT access passed.");
}
