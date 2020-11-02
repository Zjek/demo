  
#ifndef __HR_H
#define __HR_H
typedef unsigned char uint8_t;


extern void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v);
extern void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);

#endif /* __H_R_H */
