#ifndef TEST_OF_VIDEO_H
#define TEST_OF_VIDEO_H
#include <stdint.h>
#define OF_VIDEO_MODE_RGB565 3
typedef struct { uint16_t width, height, stride; uint8_t color_mode, reserved; } of_video_mode_t;
int of_video_set_mode(const of_video_mode_t *mode);
void of_video_get_mode(of_video_mode_t *mode);
uint8_t *of_video_surface(void);
void of_video_flip(void);
#endif
