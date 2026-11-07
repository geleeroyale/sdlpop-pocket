#include "pocket_music.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
sound_buffer_type *sound_pointers[58];
short midi_playing;
static void put32(FILE *f, uint32_t x) { unsigned char b[4]={x,x>>8,x>>16,x>>24}; assert(fwrite(b,1,4,f)==4); }
int main(void) {
    char dir[]="/tmp/sdlpop-music-test-XXXXXX"; assert(mkdtemp(dir)); assert(chdir(dir)==0);
    FILE *f=fopen("popmusic.dat","wb"); assert(f); assert(fwrite("POPMUS01",1,8,f)==8); put32(f,58);
    for (int i=0;i<58;i++) { put32(f,i==3?476:0); put32(f,i==3?4:0); }
    unsigned char pcm[]={0x10,0x27,0xf0,0xd8}; assert(fwrite(pcm,1,4,f)==4); assert(fclose(f)==0);
    sound_buffer_type s={0}; sound_pointers[3]=&s;
    assert(pocket_music_play(&s)==0 && midi_playing);
    int16_t out[]={0,0,1000,-1000, 30000,-30000,0,0};
    pocket_music_mix((Uint8*)out,sizeof out);
    assert(out[0]==10000 && out[1]==10000 && out[2]==11000 && out[3]==9000);
    assert(out[4]==20000 && out[5]==-32768 && out[6]==-10000 && out[7]==-10000 && !midi_playing);
    assert(pocket_music_play(&s)==0 && midi_playing); pocket_music_stop(); assert(!midi_playing);
    sound_buffer_type other={0}; assert(pocket_music_play(&other)<0);
    assert(unlink("popmusic.dat")==0); assert(chdir("/")==0); assert(rmdir(dir)==0);
    puts("Music asset lookup, sample timing, mixing, saturation, and stop passed.");
}
