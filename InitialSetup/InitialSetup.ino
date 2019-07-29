int analogPin = A0;
int ledPin = 3;
int digitalPin = 7;
int sensorValue = 0;
int max=0;
int count=0;
 
void setup () 
{
Serial.begin (115200);
pinMode(ledPin,OUTPUT);
pinMode(digitalPin,INPUT);
}
 
void loop () 
{
checkAnalog();

/*
  if(count<5000) {
    count++;
    if (max<sensorValue) {
      max=sensorValue;
    } else {
      Serial.println (max, DEC);
    }
  } else {
    count=0;
  }
  */
}

void checkAnalog() {
  static int maxA = 0;
  static int minA = 1023;
  int aValue = 0;
  float volts;
  float maxVolts;
  float minVolts;
  aValue = analogRead(analogPin);
  if (aValue<minA) {minA = aValue;}
  if (aValue>maxA) {maxA = aValue;}
  volts = (aValue*5.0)/1024.0;
  minVolts = (minA*5.0)/1024.0;
  maxVolts = (maxA*5.0)/1024.0;

  Serial.print("A = "); Serial.print(aValue,DEC);
  Serial.print(" Max = "); Serial.print(maxA);
  Serial.print(" Min = "); Serial.print(minA);
  Serial.print(" Volts = "); Serial.println(volts);

  if (aValue > 535) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    //delay(1000);
  }
}

/*#include <FastLED.h>
#define LED_PIN     7
#define NUM_LEDS    300
#define BRIGHTNESS  50
#define COLOR_ORDER RGB
#define DIGITALPIN 3

int analogPin = A3;
int val=0;
int sensorValue = 0;

CRGB leds[NUM_LEDS];
void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  pinMode (DIGITALPIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead (analogPin);
  Serial.println (sensorValue);
  val = digitalRead(DIGITALPIN);
  Serial.print(val);
  if (val == LOW)
  {
    for (int i = 0; i <= 299; i++) {
      leds[i] = CRGB ( 255,0, 0);
      FastLED.show();
      delay(1);
    }
  } else {
    for (int i = 0; i <= 299; i++) {
      leds[i] = CRGB ( 10,10, 10);
      FastLED.show();
      delay(1);
    }
  }
}*/
/*
const int ledpin=13; // ledpin and soundpin are not changed throughout the process 
const int soundpin=A0;
const int threshold=200; // sets threshold value for sound sensor
void setup() {
  Serial.begin(9600);
  pinMode(ledpin,OUTPUT);
  pinMode(soundpin,INPUT);
}
void loop() {
  int soundsens=analogRead(soundpin); // reads analog data from sound sensor
  //Serial.println(soundsens);
  if (soundsens>=threshold) {
      Serial.println(soundsens);
      digitalWrite(ledpin,HIGH); //turns led on
      delay(1000);
      digitalWrite(ledpin,LOW);
   } 
  else{
      digitalWrite(ledpin,LOW);
   }
}*/
