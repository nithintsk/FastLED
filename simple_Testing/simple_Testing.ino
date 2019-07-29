#include "FastLED.h"  

#define LED_DT 6                                             // Data pin to connect to the strip.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE WS2812B                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 7                                           // Number of LED's.


struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

void setup() { 
      pinMode(LED_DT,OUTPUT);        // Make the LED data pin as output
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812
  
}

void loop()
{ 
      leds[0] = CRGB::Blue;
      
      FastLED.show();
      
      delay(200);
      
      leds[0] = CRGB::Black;
      
      FastLED.show();
      
      delay(200);
      

      //digitalWrite(6, !digitalRead(6));
      delay(1000);

}
