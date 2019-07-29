//***************************************************************
// Breath effect  (pulse from hueA to hueB)
// Marc Miller, 2015
//***************************************************************
#include "FastLED.h"
#define LED_TYPE WS2812B
#define DATA_PIN 3
#define NUM_LEDS 300
CRGB leds[NUM_LEDS];

static float pulseSpeed = 0.1;  // Larger value gives faster pulse.

uint8_t hueA = 10;  // Start hue at valueMin.
uint8_t satA = 255;  // Start saturation at valueMin.
float valueMin = 10;  // Pulse minimum value (Should be less then valueMax).

uint8_t hueB = 360;  // End hue at valueMax.
uint8_t satB = 255;  // End saturation at valueMax.
float valueMax = 255;  // Pulse maximum value (Should be larger then valueMin).

uint8_t hue = hueA;  // Do Not Edit
uint8_t sat = satA;  // Do Not Edit
float val = valueMin;  // Do Not Edit
uint8_t hueDelta = hueA - hueB;  // Do Not Edit
static float delta = (valueMax - valueMin) / 2.35040238;  // Do Not Edit


//---------------------------------------------------------------
void setup(){
  delay(1000);  // Power-up delay
  //Serial.begin(57600);  // Allows serial monitor output (check baud rate)
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}


//---------------------------------------------------------------
void loop(){
  float dV = ((exp(sin(pulseSpeed * millis()/2000.0*PI)) -0.36787944) * delta);
  val = valueMin + dV;
  hue = map(val, valueMin, valueMax, hueA, hueB);  // Map hue based on current val
  sat = map(val, valueMin, valueMax, satA, satB);  // Map sat based on current val

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, sat, val);
  }
  FastLED.show();
  FastLED.delay(20);
 
  //Serial.print("FPS: "); Serial.println(LEDS.getFPS());   // Display FPS in serial monitor
}  // ----End main loop----
