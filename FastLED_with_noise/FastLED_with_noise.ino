/* Title: inoise8_pal_demo.ino
 *
 * By: Andrew Tuline
 *
 * Date: August, 2016
 *
 * This short sketch demonstrates some of the functions of FastLED, including:
 *
 * Perlin noise
 * Palettes
 * Palette blending
 * Alternatives to blocking delays
 * Beats (and not the Dr. Dre kind, but rather the sinewave kind)
 *
 * Refer to the FastLED noise.h and lib8tion.h routines for more information on these functions.
 *
 *
 * Recommendations for high performance routines:
 *
 *  Don't use blocking delays, especially if you plan to use buttons for input.
 *  Keep loops to a minimum, and don't use nested loops.
 *  Don't use floating point math. It's slow. Use 8 bit where possible.
 *  Let high school and not elementary school math do the work for you, i.e. don't just count pixels; use sine waves or other math functions instead.
 *  FastLED math functions are faster than built in math functions.
 *  
 */
 
 
#include "FastLED.h"
 
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
#define LED_PIN     3
//#define CLK_PIN     11
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
 
#define NUM_LEDS 300
 
struct CRGB leds[NUM_LEDS];
 
CRGBPalette16 currentPalette(CRGB::Black);
CRGBPalette16 targetPalette(OceanColors_p);

void setup() {
  
  Serial.begin(57600);
  delay(1000);
 
  LEDS.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds,NUM_LEDS);                // WS2812
//  LEDS.addLeds<LED_TYPE,LED_PIN,CLK_PIN, COLOR_ORDER>(leds,NUM_LEDS);         // APA102, WS8201
 
  LEDS.setBrightness(BRIGHTNESS);
  
} // setup()



void loop() {
  
  EVERY_N_MILLIS(10) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, 48);          // Blend towards the target palette over 48 iterations
    fillnoise8();                                                           // Update the LED array with noise at the new location. You can put this in its own EVERY_N_MILLIS() function to speed up/slow down.
  }
 
  EVERY_N_SECONDS(5) {                                                      // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
  
  LEDS.show();                                                              // Display the LED's at every loop cycle.
  
} // loop()
 


void fillnoise8() {

  #define scale 30                                                          // Don't change this programmatically or everything shakes.

  static uint16_t dist;                                                     // A random number for our noise generator.
  
  for(int i = 0; i < NUM_LEDS; i++) {                                       // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i*scale, dist+i*scale);                         // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);    // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  dist += beatsin8(10,1, 4);                                                // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
                                                                            // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
} // fillnoise8()
