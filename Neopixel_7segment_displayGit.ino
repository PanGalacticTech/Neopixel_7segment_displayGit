#include <FastLED.h>
#include "globals.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    22
//#define CLK_PIN   4
#define LED_TYPE    WS2812B

//#define LED_TYPE WS2811_400

#define COLOR_ORDER RGB
#define NUM_LEDS   21

//uint8_t numberOfLEDs = NUM_LEDS

//CRGB leds[NUM_LEDS];

CRGBArray<NUM_LEDS> digit;



//CRGB *digitLEDs[NUM_LEDS];

//CRGBArray<3> A;


//CRGB digitLEDs[NUM_LEDS];



//CRGBSet A(digitLEDs, 21);

#define BRIGHTNESS          20
#define FRAMES_PER_SECOND  120



uint8_t maxBrightness = 1;



struct singleDigit {

  uint8_t  ledArray[21];




};



uint8_t  ZeroArray[21] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 , 0, 0, 0};

uint8_t  OneArray[21] =  {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0};


struct anydigit {

  uint8_t A;
  uint8_t B;
  uint8_t C;
  uint8_t D;
  uint8_t E;
  uint8_t F;
  uint8_t G;



};


uint8_t digitA[3] = {0 , 1, 2};
uint8_t digitB[3] = {3 , 4, 5};
uint8_t digitC[3] = {6 , 7, 8};
uint8_t digitD[3] = {9 , 10, 11};
uint8_t digitE[3] = {12 , 13, 14};
uint8_t digitF[3] = {15 , 16, 17};
uint8_t digitG[3] = {28 , 19, 20};


anydigit zero = {1, 1, 1, 1, 1, 1, 0};
anydigit one = {0, 1, 1, 0, 0, 0, 0};
anydigit current;


struct redgreenblue {

  byte r;
  byte g;
  byte b;


};


redgreenblue skyroraBlue = { 0 , 0 , 255};






void setup() {

  delay(3000); // 3 second delay for recovery

  Serial.begin(115200);


  // CRGBset A(digitLEDs, 3);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(digit, 21).setCorrection(TypicalLEDStrip);



  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);


  for (int i = 0; i < NUM_LEDS; i++) {
    //  leds[i] = CRGB::Black;
  }

}







uint8_t ledStepper = 0;




void loop() {

current = zero;


setDigit();







  FastLED.show();
  delay(1000);


}





void setDigit () {

if (current.A){
    digit(0, 2) = CRGB::Blue;
}
if (current.B){
    digit(3, 5) = CRGB::Blue;
}
if (current.C){
    digit(6, 8) = CRGB::Blue;
}
if (current.D){
    digit(9, 11) = CRGB::Blue;
}
if (current.E){
    digit(12, 14) = CRGB::Blue;
}
if (current.F){
    digit(15, 17) = CRGB::Blue;
}
if (current.G){
    digit(18, 20) = CRGB::Blue;
}




}
