#include <FastLED.h>
#include "globals.h"

FASTLED_USING_NAMESPACE



#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    22                    // Data pin connection to LED string
//#define CLK_PIN   4
#define LED_TYPE    WS2812B                // =

//#define LED_TYPE WS2811_400

#define COLOR_ORDER RGB
#define NUM_LEDS   21                        // Number of LEDs in a single digit

//uint8_t numberOfLEDs = NUM_LEDS               // alternative methods, depreciated for now

//CRGB leds[NUM_LEDS];                            // alternative methods, depreciated for now     

//CRGBSet A(digitLEDs, 21);                        // alternative methods, depreciated for now     

CRGBArray<NUM_LEDS> digit;                        // Set up an array containing the number of LEDs in a single digit



uint8_t maxBrightness = 20;                      // Define the max brightness of all LEDs. 0 - 255. 20 reccomended for testing: MUST use seperate 5v supply for full brightness operation! Do not use USB power!







struct anydigit {                                // Generic data Structure to contain the data for "characters" divided up into 7 segments. Each segment can be on or off.

  uint8_t A;
  uint8_t B;
  uint8_t C;
  uint8_t D;
  uint8_t E;
  uint8_t F;
  uint8_t G;

};


uint8_t digitA[3] = {0 , 1, 2};                        // arrays that contain the ID numbers for each LED in each segment of a single digit
uint8_t digitB[3] = {3 , 4, 5};
uint8_t digitC[3] = {6 , 7, 8};
uint8_t digitD[3] = {9 , 10, 11};
uint8_t digitE[3] = {12 , 13, 14};
uint8_t digitF[3] = {15 , 16, 17};
uint8_t digitG[3] = {28 , 19, 20};

anydigit current;                                          //Holds the current data to be printed to the first character of the display [ will be depreciated]

anydigit digitZero;                                           // holds the current value of the first digit
anydigit digitOne;                                            // holds the current value of the 2nd digit
anydigit digitTwo;                                            // holds the current value of the 3rd digit
anydigit digitThree;
anydigit digitFour;
anydigit digitFive;
anydigit plusminus;     

                   // Example Display:
                   // [plusminus>(T-) [digitFive> 00:00:00  <digit zero]

anydigit zero = {1, 1, 1, 1, 1, 1, 0};                      // these data structures hold the arrays for each unique character. Bits mirror the segments of a typical 7 segment display
anydigit one = {0, 1, 1, 0, 0, 0, 0};
anydigit two = {1, 1, 0, 1, 1, 0, 1};
anydigit three = {1, 1, 1, 1, 0, 0, 1};
anydigit four = {0, 1, 1, 0, 0, 1, 1};
anydigit five = {1, 0, 1, 1, 0, 1, 1};
anydigit six = {1, 0, 1, 1, 1, 1, 1};
anydigit seven = {1, 1, 1, 0, 0, 0, 0};
anydigit eight = {1, 1, 1, 1, 1, 1, 1};
anydigit nine = {1, 1, 1, 0, 0, 1, 1};


 anydigit alldigits[10] ={zero, one, two, three, four, five, six, seven, eight, nine};    // array to hold data structures above. Array mumber matches the character being recalled. 

// This can be accessed as:

//alldigits[0];   // contains the display data for the character 0
//alldigits[1];   // Contains the display data for the character 1
// et al




struct redgreenblue {             // data structure to save named RGB values that could be passed to functions

  byte r;
  byte g;
  byte b;


};


redgreenblue skyroraBlue = { 0 , 0 , 255};    // Data structure for "skyroraBlue" colour as an RGB value

redgreenblue offWhite = { 255, 255, 255};      // data structure for an offwhite colour, all LEDs on max (these figures can be calibrated later to produce a cleaner white if required

redgreenblue blackout = {0 , 0 , 0};          // data structure holding "black" - all LEDS off.

redgreenblue currentColour;                  // data structure to hold the current LED colour


bool setBlue = true;    // testing variable can be used to test changing colours



void setup() {

  delay(3000); // 3 second delay for recovery

  Serial.begin(115200);                                    // Serial for debugging if required


  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(digit, 21).setCorrection(TypicalLEDStrip);            // This sets up the fist digit of the display


  // set master brightness control
  FastLED.setBrightness(maxBrightness);                                                                        // Sets the brightness for the entire string


currentColour = skyroraBlue;   // Preset the LED colour, can be changed later in program

  for (int i = 0; i < NUM_LEDS; i++) {                                                                          // loop could be used to reset all LEDs to black on startup. Currently depreciated
    //  leds[i] = CRGB::Black;
  }

}




int8_t countdown = 0;            // Countdown timer placeholder for testing. 




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main Loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void loop() {


current = alldigits[countdown];                   // current display data handed a digit from alldigits array, number set by the countdown placeholder

if (setBlue){
currentColour = skyroraBlue;                              // pass currentColour the data structure containing 
} else {                                                           // the RGB colour defined earlier. This could be 
 currentColour = offWhite;                                                         // Placed in other places and used to change the colour
}                                                          // in response to other events. e.g. when value turns negative
                                                          // to positive.

setDigit(currentColour.r, currentColour.g, currentColour.b);                                      // Turns LED segments on and off. Brightness is already set in setup loop,.
                                                                //     Colour for "ON" LEDs passed as seperate RGB values an argument to the function


  FastLED.show();                          // print the data to all the LEDs
  delay(1000);                          // delay for testing purposes only



countdown--;                                // These control our test countdown clock

if (countdown < 0){

  countdown = 9;
}

}



// this function needs to be a method for a digit object.


void setDigit (uint8_t red, uint8_t green, uint8_t blue ) {           // This function sets the first digit based on the data structure passed to it.
                                                                       // Also passed rgb colour value, can be used later to change colour of
                                                                       // the entire digit

if (current.A){   
     digit(0, 2) = CRGB(red, green, blue);
   // digit(0, 2) = CRGB::Blue;
} else {
    digit(0, 2) = CRGB::Black;
}


if (current.B){
    digit(3, 5) = CRGB(red, green, blue);
} else {
    digit(3, 5) = CRGB::Black;
}


if (current.C){
    digit(6, 8) = CRGB(red, green, blue);
}else {
    digit(6, 8) = CRGB::Black;
}


if (current.D){
    digit(9, 11) = CRGB(red, green, blue);
}else {
    digit(9, 11) = CRGB::Black;
}


if (current.E){
    digit(12, 14) = CRGB(red, green, blue);
}else {
    digit(12, 14) = CRGB::Black;
}


if (current.F){
    digit(15, 17) = CRGB(red, green, blue);
}else {
    digit(15, 17) = CRGB::Black;
}


if (current.G){
    digit(18, 20) = CRGB(red, green, blue);
}else {
    digit(18, 20) = CRGB::Black;
}


}
