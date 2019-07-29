
// Examples taken from: https://github.com/atuline/FastLED-Demos/blob/master

#include <FastLED.h>
/*
 * Pin 13 Effects
 * 0. Sound Mode.
 * Off: No action
 * 
 * 1. Solid Color Gradient
 * Off: Pause at color
 * 
 * 2. Blend Wave
 * Off: Pause at gradient
 * 
 * 3. Rainbow Beat
 * Off: Stop at Color
 * 
 * 4. Serendipity
 * Off: Turn off Completely
 */

/*
 * TODO:
 * 1. Adjust FadeCount
 */

// Constant Defined Variables
#define LED_PIN     3               // Pin 3 is the data pin to the LED
#define EFFECT_PIN 13               // Pin 13 is used to control the effect
#define BRIGHTNESS_PIN 11           // To control the brightness of the strips
#define THRESHOLD_PIN 9             // To control the sound threshold
#define NUM_LEDS    7             // Total Number of LEDs
#define LED_TYPE    WS2812B         // Type of Strip
#define COLOR_ORDER GRB             // Order of WS2812B is GRB
#define ANALOG_PIN A0

// Declare the led array
CRGB leds[NUM_LEDS];

// Delay for Palette Colors
#define UPDATES_PER_SECOND 100

// Variable to control brightness
uint8_t brightness = 255;

// Pin related variables
uint8_t effectValue = 0; 
uint8_t effectCount = 0;
uint8_t brightnessValue = 0; 
uint8_t thresholdValue = 0; 
uint8_t thresholdCount = 0;

// Sound related Variables
int avgValue = 0;                   // Find the average value to control threshold
int fadeCount=-1;                   // Count for the fade effect with Sound
int sparkleCount=-1;                // Count for the sparkle effect with Sound
int effectType=0;                   // To keep track of the type of effect.
static uint8_t fade_red=random8(), fade_green=random8(), fade_blue=random8();             // Fade RGB Values
static uint8_t sparkle_red=random8(), sparkle_green=random8(), sparkle_blue=random8();    // Sparkle RGB values

// Pallette related variables
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

// Solid rainbow variable
uint8_t gHue = 0; 

// Blend wave variables
CRGB clr1;
CRGB clr2;
uint8_t speed;
uint8_t loc1;
uint8_t loc2;
uint8_t ran1;
uint8_t ran2;

// Confetti Variables
unsigned long previousMillis;                                 // Store last time the strip was updated.
uint8_t  thisfade = 8;                                        // How quickly does it fade? Lower = slower fade rate.
int       thishue = 50;                                       // Starting hue.
uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
uint8_t   thissat = 200;                                      // The saturation, where 255 = brilliant colours.
uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
int       huediff = 256;                                      // Range of random #'s to use for hue
uint8_t thisdelay = 5;                                        // We don't need much delay (if any)

//Rainbow March variables
uint8_t thisdelayR = 40;                                       // A delay value for the sequence(s)
uint8_t thishueR = 0;                                          // Starting hue value.
int8_t thisrotR = 1;                                           // Hue rotation speed. Includes direction.
uint8_t deltahueR = 1;                                         // Hue change between pixels.
bool thisdirR = 0;                                             // I use a direction variable instead of signed math so I can use it in multiple routines.

// Setup Function
void setup() {
    delay( 1000 ); // power-up safety delay
    Serial.begin (115200);

    // Pin mode initialization
    pinMode(LED_PIN,OUTPUT);        // Make the LED data pin as output
    pinMode(EFFECT_PIN, INPUT);     // Make the effect pin as input
    pinMode(BRIGHTNESS_PIN, INPUT); // Make the brightness pin as input
    pinMode(THRESHOLD_PIN, INPUT);  // Make the threshold pin as input

    // Initialize the LEDs
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  brightness );

    // Get the average sound sensor reading when there is no noise
    // Take 10000 Samples
    long sum = 0;
    sum = analogRead(ANALOG_PIN);
    for(long i = 0; i<10; i++) {
        for(long j = 0; j<999;j++)
        {
           sum += analogRead(ANALOG_PIN);
        }
        sum = sum/1000;
    }
    avgValue = (int)sum;
    Serial.println("End of Initialization.");
    Serial.print("A = "); Serial.print(avgValue,DEC);

    // Run a green colorwipe to show end of Initialization
    colorWipe(0x00,0xff,0x00, 4);
    colorWipe(0x00,0x00,0x00, 4);

    // Default Pallette is the rainbow palette
    currentPalette = RainbowStripeColors_p;
    currentBlending = LINEARBLEND;
}

void loop()
{ 
      effectValue = digitalRead(EFFECT_PIN);; 
      brightnessValue = digitalRead(BRIGHTNESS_PIN);; 
      thresholdValue = digitalRead(THRESHOLD_PIN);; 

      static int effectPast = effectValue;
      static int brightnessPast = brightnessValue;
      static int thresholdPast = thresholdValue;
      
      // Check if effect Pin has toggled and increment count when going from LOW to HIGH
      if (effectValue!= effectPast) {
          if (effectValue) {
              effectCount = ((effectCount+1) % 5);
              if (effectCount == 0) {
                    setAll(61,61,212);
                    FastLED.show();
                    delay(250);
                    setAll(0,0,0);
                    FastLED.show();
                    delay(250);
                    setAll(61,61,212);
                    FastLED.show();
                    delay(250);
                    setAll(0,0,0);
                    FastLED.show();
                    delay(250);
                    long sum = 0;
                    sum = analogRead(ANALOG_PIN);
                    for(long i = 0; i<10; i++) {
                        for(long j = 0; j<999;j++)
                        {
                           sum += analogRead(ANALOG_PIN);
                        }
                        sum = sum/1000;
                    }
                    avgValue = (int)sum;
                    colorWipe(0x00,0xff,0x00, 1);
                    colorWipe(0x00,0x00,0x00, 1);
              }
          } 
          effectPast = effectValue;
          Serial.print("Effect Count has changed to ");Serial.println(effectCount);
          Serial.print("Effect Value is ");Serial.println(effectValue);
      }
      
      switch(effectCount) {
          case 0:   if (effectValue == HIGH) {
                        //Read the Analog Value of Sound Pin
                        int aValue = 0;
                        aValue = analogRead(ANALOG_PIN);
                        // Set the brightness according to alteration of the Brightness Pin
                        FastLED.setBrightness(  brightness );
                        // Change the Base pallette periodically
                        // ChangePalettePeriodically();
                        // Check sound level and alter the effect
                        changePaletteBasedOnBeat(aValue);
                        static uint8_t startIndex = 0;
                        startIndex = startIndex + 1; // motion speed
                        FillLEDsFromPaletteColors_Sound(startIndex);
                    } else {
                        setAll(0,0,0);
                        FastLED.show();
                        delay(50);
                    }
                    break;    
          case 1:   if (effectValue == HIGH) {
                        int pos = beatsin16(8,25,230); // generating the sinwave 
                        fill_solid(leds, NUM_LEDS, CHSV( gHue, 255, pos)); // CHSV (hue, saturation, value);
                        FastLED.show();
                        EVERY_N_MILLISECONDS(250) {gHue++;} 
                    }
                    break;
          case 2:   if (effectValue == HIGH) {
                        blendwave();
                        FastLED.show();
                    }
                    break;
          case 3:   if (effectValue == HIGH) {
                        ChangeMeConfetti();                                                 // Check the demo loop for changes to the variables.
                        EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
                            confetti();
                        }
                        FastLED.show();  
                    }
                    break;
          case 4:   if (effectValue == HIGH) {
                        ChangeMeRainbow();                                                 // Check the demo loop for changes to the variables.
                        EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
                            rainbow_march();
                        }
                        FastLED.show();  
                    }
                    break;
      }
      
      // Check if Brightness pin has toggled and change brightness by 32
      if (brightnessValue!= brightnessPast) {
          brightness = (brightness+32);
          brightnessPast = brightnessValue;
          Serial.print("Brightness has changed to ");Serial.println(brightness);
          Serial.print("Brightness Pin is ");Serial.println(brightnessValue);
      }
}

void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels. You would need to change the max_bright value.
  
  if (thisdirR == 0) thishueR += thisrotR; else thishueR-= thisrotR;  // I could use signed math, but 'thisdir' works with other routines.
  fill_rainbow(leds, NUM_LEDS, thishueR, deltahueR);            // I don't change deltahue on the fly as it's too fast near the end of the strip.
  
} // rainbow_march()



void ChangeMeRainbow() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  
  uint8_t secondHand = (millis() / 1000) % 60;                // Change '60' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.

  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: thisrotR=1; deltahueR=5; break;
      case  5: thisdirR=-1; deltahueR=10; break;
      case 10: thisrotR=5; break;
      case 15: thisrotR=5; thisdirR=-1; deltahueR=20; break;
      case 20: deltahueR=30; break;
      case 25: deltahueR=2; thisrotR=5; break;
      case 30: break;
    }
  }
  
} // ChangeMe()

void confetti() {                                             // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, thisfade);                    // Low values = slower fthanade.
  int pos = random16(NUM_LEDS);                               // Pick an LED at random.
  leds[pos] += CHSV((thishue + random16(huediff))/4 , thissat, thisbri);  // I use 12 bits for hue so that the hue increment isn't too quick.
  thishue = thishue + thisinc;                                // It increments here.
} // confetti()


void ChangeMeConfetti() {                                             // A time (rather  loop) based demo sequencer. This gives us full control over the length of each sequence.
  uint8_t secondHand = (millis() / 1000) % 15;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: thisinc=1; thishue=192; thissat=255; thisfade=2; huediff=256; break;  // You can change values here, one at a time , or altogether.
      case  5: thisinc=2; thishue=128; thisfade=8; huediff=64; break;
      case 10: thisinc=1; thishue=random16(255); thisfade=1; huediff=16; break;      // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 15: break;                                                                // Here's the matching 15 for the other one.
    }
  }
}

void blendwave() {

  speed = beatsin8(6,0,255);

  clr1 = blend(CHSV(beatsin8(3,0,255),255,brightness), CHSV(beatsin8(4,0,255),255,brightness), speed);
  clr2 = blend(CHSV(beatsin8(4,0,255),255,brightness), CHSV(beatsin8(3,0,255),255,brightness), speed);

  loc1 = beatsin8(10,0,NUM_LEDS-1);
  
  fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
  fill_gradient_RGB(leds, loc1, clr2, NUM_LEDS-1, clr1);

}

void FillLEDsFromPaletteColors_Sound(uint8_t colorIndex)
{
      if ( fadeCount >= 0 || effectType == 2) {
            if (effectType == 2) {
                  for(int j = 0; j < 5; j++) {
                      setAll(fade_red,fade_green,fade_blue);
                      FastLED.show();
                      delay(50);
                      setAll(0,0,0);
                      FastLED.show();
                      delay(50);
                  }
                  fadeCount = -1;
                  Serial.println("Effect Strobe");
            } else {
                  fadeCount-=6;
                  FastLED.setBrightness((fadeCount/255.0) * brightness);
                  FastLED.show();
                  FastLED.delay(1000 / UPDATES_PER_SECOND);
                  Serial.println("Effect Fade");
                  effectType=1;
            }
      } else if (sparkleCount >= 0) {
            ChangeMeConfetti();
            EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
                confetti();
            }
            FastLED.show();  
            Serial.println("Effect Sparkle");
            sparkleCount-=6;
       } else if (effectType==0 || effectType == 4
       
       
       
       
       
       ) {
            //Serial.println("Executing Effect Type 0");
            for( int i = 0; i < NUM_LEDS; i++) {
                leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
                colorIndex += 2;
            }
            FastLED.show();
            FastLED.delay(1000 / UPDATES_PER_SECOND);
      } 
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  FastLED.show();
}

void changePaletteBasedOnBeat(int aValue)
{
    /*
     * EffectType 1 = Fade in and out
     * EffectType 2 = FadeStrobe
     * EffectType 3 = Sparkle
    */
    if (aValue > avgValue+15) {
          if (random8() < 251) {
                fade_red = random8();
                fade_green = random8();
                fade_blue = random8();
                Serial.println("Avg Value + 15 reached. ");
                setAll(fade_red,fade_green,fade_blue);
                FastLED.show();
                FastLED.delay(10);
                fadeCount = 254;
                effectType=1;                                       // Fade in and out
             
          } else {
                fade_red = random8();
                fade_green = random8();
                fade_blue = random8();
                setAll(fade_red,fade_green,fade_blue);
                effectType=2;                                       // Fade Strobe  
          }

    } else if (aValue > avgValue + 10 && avgValue <= avgValue+15 && fadeCount < 50) {
          // If we are changing into sparkle from another effect Type, set all Pixels to 0.
          if (effectType != 3) {
              for (int i = 0; i<300; i++) {
                  setPixel(i,0,0,0);
              }
          }
          effectType = 3;
          sparkleCount = 255;
    } else if (aValue > avgValue + 4 && avgValue <= avgValue+10 && fadeCount < 50 && sparkleCount < 50) {
          ChangePalette();
          effectType = 4;
    } else if (aValue <= avgValue+4 && fadeCount < 0){
        //Serial.print("Analog Value is "); Serial.println(aValue);
        effectType=0;
    }
}

// Setup a pallette of red, white and blue
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

void ChangePalette()
{
    int val = random(5);
    switch(val) {
      case 0: currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; 
              break;
      case 1: SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND;
              break;
      case 2: currentPalette = CloudColors_p;           currentBlending = LINEARBLEND;
              break;
      case 3: currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND;
              break;
      default: currentPalette = LavaColors_p; currentBlending = LINEARBLEND;
              break;    
    }
}

// Function to change effect based on time for base pallette
void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand == 0)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND;  }
        if( secondHand == 20)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// Function to show end of initialization
void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i=i+2) {
      setPixel(i, red, green, blue);
      FastLED.show();
      delay(SpeedDelay);
  }
}
