/**
 *  @brief 
 */

#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#define RED_PIN          3  // pb6 timer4 ch1
#define GREEN_PIN        6  // pa8 timer1 ch1
#define BLUE_PIN         8  // pa1 timer2 ch2

#ifdef __cplusplus
extern "C"{
#endif

void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );
void lights_set_rgb(float r, float g, float b);

#ifdef __cplusplus
} // extern "C"
#endif



#endif

