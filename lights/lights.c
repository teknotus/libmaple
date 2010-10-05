#include <math.h>

#include "libmaple.h"
#include "lights.h"

#define DUTY_MIN 8

// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//              if s == 0, then h = -1 (undefined)

void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
   float min, max, delta;

   min = MIN( MIN(r, g), b );
   max = MAX( MAX(r, g), b );
   *v = max;                             // v

   delta = max - min;

   if( max != 0 )
      *s = delta / max;           // s
   else {
      // r = g = b = 0            // s = 0, v is undefined
      *s = 0;
      *h = -1;
      return;
   }

   if( r == max )
      *h = ( g - b ) / delta;             // between yellow & magenta
   else if( g == max )
      *h = 2 + ( b - r ) / delta; // between cyan & yellow
   else
      *h = 4 + ( r - g ) / delta; // between magenta & cyan

   *h *= 60;                             // degrees
   if( *h < 0 )
      *h += 360;

}

void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
   int i;
   float f, p, q, t;

   if( s == 0 ) {
      // achromatic (grey)
      *r = *g = *b = v;
      return;
   }

   h /= 60;                      // sector 0 to 5
   i = floor( h );
   //i = h;

   f = h - i;                    // factorial part of h
   p = v * ( 1 - s );
   q = v * ( 1 - s * f );
   t = v * ( 1 - s * ( 1 - f ) );

   switch( i ) {
   case 0:
      *r = v;
      *g = t;
      *b = p;
      break;
   case 1:
      *r = q;
      *g = v;
      *b = p;
      break;
   case 2:
      *r = p;
      *g = v;
      *b = t;
      break;
   case 3:
      *r = p;
      *g = q;
      *b = v;
      break;
   case 4:
      *r = t;
      *g = p;
      *b = v;
      break;
   default:              // case 5:
      *r = v;
      *g = p;
      *b = q;
      break;
   }
}

void lights_set_rgb(float r, float g, float b) {
   uint16 red;
   uint16 green;
   uint16 blue;

   red   = r * 65535;
   green = g * 65535;
   blue  = b * 65535;

//   red   = (red < DUTY_MIN)   ? DUTY_MIN : red;
//   green = (green < DUTY_MIN) ? DUTY_MIN : green;
//   blue  = (blue < DUTY_MIN)  ? DUTY_MIN : blue;

   pwmWrite(RED_PIN,   red);
   pwmWrite(GREEN_PIN, green);
   pwmWrite(BLUE_PIN,  blue);
}
