#include <FastLED.h>

#define LED_PIN     3
#define DIGITAL_PIN 7
#define SOUND_PIN 13
#define THRESHOLD_PIN 11
#define RAINBOW_PIN 9
#define NUM_LEDS    300
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

int analogPin = A0;
int avgValue = 0;
int max=0;
int fadeCount=-1;
int sparkleCount=-1;
int effectType=0;
static uint8_t fade_red=random8(), fade_green=random8(), fade_blue=random8();
static uint8_t sparkle_red=random8(), sparkle_green=random8(), sparkle_blue=random8();

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int sPin = 0; 
int tPin = 0; 
int rPin = 0; 
uint8_t gHue = 0; 


int BRIGHTNESS = 255;

void setup() {
    delay( 3000 ); // power-up safety delay
    Serial.begin (115200);
    pinMode(LED_PIN,OUTPUT);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    // initialize the pushbutton pin as an input:
    pinMode(SOUND_PIN, INPUT);
    pinMode(THRESHOLD_PIN, INPUT);
    pinMode(RAINBOW_PIN, INPUT);

    long sum = 0;
    sum = analogRead(analogPin);
    for(long i = 0; i<10; i++) {
        for(long j = 0; j<999;j++)
        {
           sum += analogRead(analogPin);
        }
        sum = sum/1000;
    }
    avgValue = (int)sum;
    Serial.println("End of Initialization.");
    Serial.print("A = "); Serial.print(avgValue,DEC);
    colorWipe(0x00,0xff,0x00, 8);
    colorWipe(0x00,0x00,0x00, 8);
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop()
{ 
      rPin = digitalRead(RAINBOW_PIN);
      sPin = digitalRead(SOUND_PIN);
      if (sPin == HIGH) {
          static int maxA = 0;
          static int minA = 1023;
          int aValue = 0;
          float volts;
          float maxVolts;
          float minVolts;
          aValue = analogRead(analogPin);
          //aValue = avgValue+8;
          if (aValue<minA) {minA = aValue;}
          if (aValue>maxA) {maxA = aValue;}
          volts = (aValue*5.0)/1024.0;
          minVolts = (minA*5.0)/1024.0;
          maxVolts = (maxA*5.0)/1024.0;
          FastLED.setBrightness(  BRIGHTNESS );
    
          ChangePalettePeriodically();
          
          static int sparklePixelNum = 0;
          changePaletteBasedOnBeat(aValue, sparklePixelNum); 
          static uint8_t startIndex = 0;
          startIndex = startIndex + 2; // motion speed
          FillLEDsFromPaletteColors(startIndex, sparklePixelNum);
      } else if (rPin == HIGH) {
            int pos = beatsin16(5,0,192); // generating the sinwave 
            fill_solid(leds, NUM_LEDS, CHSV( gHue, 255, pos)); // CHSV (hue, saturation, value);
            FastLED.show();
            EVERY_N_MILLISECONDS(20) {gHue++;} 
      } else if (rPin == LOW) {
            FastLED.show();
      }
      
}

void FillLEDsFromPaletteColors(uint8_t colorIndex, int pixelNum)
{
      if ( fadeCount >= 0 ) {
          fadeCount-=8;
          FastLED.setBrightness(fadeCount);
          FastLED.show();
          FastLED.delay(1000 / UPDATES_PER_SECOND);
      } /*else if (sparkleCount >= 0 || effectType == 3) {
          if (effectType == 2) {
                showStrip();
                setPixel(pixelNum,0,0,0);
                sparkleCount-=4;
                int temp = pixelNum;
                for (int i = 0; i<14; i++) {
                    temp += 20;
                    setPixel(temp%300,0,0,0);
                }
          }*/ else {
                  for(int j = 0; j < 5; j++) {
                    setAll(sparkle_red,sparkle_green,sparkle_blue);
                    showStrip();
                    delay(50);
                    setAll(0,0,0);
                    showStrip();
                    delay(50);
                  }
                 delay(500);
                 sparkleCount=-1;
          }
       } else if (effectType==0) {
          uint8_t brightness = 255;
          for( int i = 0; i < NUM_LEDS; i++) {
              leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
              colorIndex += 2;
          }
          FastLED.show();
          FastLED.delay(1000 / UPDATES_PER_SECOND);
      } 
}

void changePaletteBasedOnBeat(int aValue, int &sparklePixel)
{
    if (aValue > avgValue+10) {
        fade_red = random8();
        fade_green = random8();
        fade_blue = random8();
        Serial.println("Analog Value is greater than 535. ");
        setAll(fade_red,fade_green,fade_blue);
        showStrip();
        FastLED.delay(10);
        fadeCount = 254;
        sparkleCount=0;
        effectType=1;
    } else if (aValue <= avgValue+4){
        Serial.print("Analog Value is "); Serial.println(aValue);
        effectType=0;
    } else if (aValue >avgValue + 4 && avgValue <= avgValue+10) {
        if (effectType != 2) {
          for (int i = 0; i<300; i++) {
              setPixel(i,0,0,0);
          }
        }
      if (random8() > 251) {
            sparkle_red = random8();
              sparkle_green = random8();
              sparkle_blue = random8();
            setAll(sparkle_red,sparkle_green,sparkle_blue);
            effectType=3;
        } else {
            sparkleCount = 255;
            effectType=2;
            for (int i = 0; i<150; i++) {
              sparkle_red = random8();
              sparkle_green = random8();
              sparkle_blue = random8();
              sparklePixel = random(NUM_LEDS);
              setPixel(sparklePixel,sparkle_red,sparkle_green,sparkle_blue);
            } 
        }

    }
}

void showStrip() {
   // FastLED
   FastLED.show();
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
  showStrip();
}

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i=i+2) {
      setPixel(i, red, green, blue);
      showStrip();
      delay(SpeedDelay);
  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

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

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


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


void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}
