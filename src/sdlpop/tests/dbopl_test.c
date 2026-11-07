#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pocket_dbopl.h"
static void setup(opl3_chip *c) {
 OPL3_Reset(c,44100); OPL3_WriteReg(c,0x105,1); OPL3_WriteReg(c,0x20,1); OPL3_WriteReg(c,0x23,1); OPL3_WriteReg(c,0x40,0x10); OPL3_WriteReg(c,0x43,0); OPL3_WriteReg(c,0x60,0xf4); OPL3_WriteReg(c,0x63,0xf4); OPL3_WriteReg(c,0x80,0x77); OPL3_WriteReg(c,0x83,0x77); OPL3_WriteReg(c,0xc0,0x30); OPL3_WriteReg(c,0xa0,0x98); OPL3_WriteReg(c,0xb0,0x31);
}
int main(void) {
 opl3_chip a,b; int16_t block[2048], single[2048]; setup(&a); setup(&b);
 OPL3_GenerateStream(&a,block,1024); for(int i=0;i<1024;i++) OPL3_Generate(&b,single+2*i);
 if(memcmp(block,single,sizeof block)) return puts("Block output mismatch"),1;
 int nonzero=0; for(int i=0;i<2048;i++) nonzero+=block[i]!=0;
 if(!nonzero) return puts("No audio"),1;
 printf("DBOPL block output matches single-frame output; %d nonzero samples\n",nonzero); return 0;
}
