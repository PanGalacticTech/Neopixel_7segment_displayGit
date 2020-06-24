#include <FastLED.h>
#include "globals.h"

FASTLED_USING_NAMESPACE



#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    22                    // Data pin connection to LED string
//#define CLK_PIN   4
#define LED_TYPE    WS2812B                // 

//#define LED_TYPE WS2811_400

#define COLOR_ORDER GRB

//#define NUM_LEDS   21                        // Number of LEDs in a single digit [Depreciated]

#define NUM_LEDS 126                             //Number of LEDS in 6 digits (will add more for the sign and dots later  need at least 7 for +- and 4 for dots            




//uint8_t numberOfLEDs = NUM_LEDS               // alternative methods, depreciated for now

//CRGB leds[NUM_LEDS];                            // alternative methods, depreciated for now

//CRGBSet A(digitLEDs, 21);                        // alternative methods, depreciated for now

//CRGBArray<NUM_LEDS> digit0;                        // Set up an array containing the number of LEDs in a single digit
//CRGBArray<NUM_LEDS> digit1;                        // Set up an array containing the number of LEDs in a single digit
//CRGBArray<NUM_LEDS> digit2;
//CRGBArray<NUM_LEDS> digit3;                        // Set up an array containing the number of LEDs in a single digit
//CRGBArray<NUM_LEDS> digit4;                        // Set up an array containing the number of LEDs in a single digit
//CRGBArray<NUM_LEDS> digit5;                        // Set up an array containing the number of LEDs in a single digit


// I could not work out how  to make this work so we are going for the big dumb hammer approach
//CRGBArray<21> digit0, digit1, digit2, digit3, digit4, digit5;


CRGBArray<NUM_LEDS> ledString;



uint8_t maxBrightness = 20;                      // Define the max brightness of all LEDs. 0 - 255. 20 reccomended for testing: MUST use seperate 5v supply for full brightness operation! Do not use USB power!



int8_t colourSelect = 0;                        // selectes the display colour. 0 = skyrora blue, 1 = offwhite, 2 = yellowOrange. (currently cycles through each)





void setup() {

  delay(3000); // 3 second delay for recovery

  Serial.begin(115200);                                    // Serial for debugging if required


  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(ledString, NUM_LEDS).setCorrection(TypicalLEDStrip);            // This sets up the fist digit of the display


  // set master brightness control
  FastLED.setBrightness(maxBrightness);                                                                        // Sets the brightness for the entire string


  currentColour = skyroraBlue;   // Preset the LED colour, can be changed later in program

  for (int i = 0; i < NUM_LEDS; i++) {                                                                          // loop could be used to reset all LEDs to black on startup. Currently depreciated
    //  leds[i] = CRGB::Black;
  }

}






int8_t countdown = 0;            // Countdown timer placeholder for testing.


int8_t seconds = 99;    // initialised at 99 for testing
int8_t minuites;
int8_t hours;




int8_t activeDigit = 0;  // this value used to control the digit that is currently being written to



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main Loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void loop() {


  

  displayedDigits[0]= alldigits[countdown];                                           // current display data handed a digit from alldigits array, number set by the countdown placeholder
  // This line can be put into a for loop, and the pre seperated timing values passed to displayed digits array, then to the setDigit function



  setDigit(displayedDigits[0], activeDigit, currentColour.r, currentColour.g, currentColour.b);                                      // Turns LED segments on and off. Brightness is already set in setup loop,.

  //     Colour for "ON" LEDs passed as seperate RGB values an argument to the function
  //      currentColour variable is passed as seperate .r .b .g values, so these can be replaced directly with RGB values 0-255 


  FastLED.show();                          // print the data to all the LEDs
  Serial.println(countdown);
  delay(1000);                          // delay for testing purposes only



  countdown--;                                         // These control our test countdown clock

  if (countdown < 0) {                               // Reset the timer when it reaches zero
    // Once other digits are added this figure will count down
    countdown = 9;                                     // hours, minuites, seconds, then increment past zero for post launch timing

    // Just a way of testing the colour changing function through each cycle
    currentColour =  colourArray[colourSelect];            // pass currentColour the array containing data structure containing
    // colour data. Can be used in response to other events
    // in response to other events. e.g. when value turns negative
    // to positive.

    colourSelect++;                                     // increment current colour - cycle through colours
    if (colourSelect > 2) {                              // reset when reach the end of array
      colourSelect = 0;
    }

  }
}









// this function needs to be a method for a "digit" object but I couldn't work out how to do that. 
// This method is slightly more inelegent but it should work (UNTESTED with more than 1 digit)



void setDigit (digitSeg current, int8_t digitNumber, uint8_t red, uint8_t green, uint8_t blue ) {           // This function sets the first digit based on the data structure passed to it.


  // A new instance of digitSeg has been set up ready to take whatever data is placed into it
  // After this,  a variable to denote which digit we are setting is passed as an argument.
  // Also passed to function - rgb colour value, which sets the colour for the entire digit



  uint8_t  q = (digitNumber) * 21;    // This variable is added onto the array numbers, advancing down the LED array as each successive digit is selected to be written to.



  if (current.A) {                                                         // if the A segment contains a 1
    ledString((0 + q), (2 + q)) = CRGB(red, green, blue);                  // print the RGB colour to that segment
  } else {
    ledString(0+q, 2+q) = CRGB::Black;                                           // Else turn it off
  }


  if (current.B) {
    ledString(3+q, 5+q) = CRGB(red, green, blue);
  } else {
    ledString(3+q, 5+q) = CRGB::Black;
  }


  if (current.C) {
    ledString(6+q, 8+q) = CRGB(red, green, blue);
  } else {
    ledString(6+q, 8+q) = CRGB::Black;
  }


  if (current.D) {
    ledString(9+q, 11+q) = CRGB(red, green, blue);
  } else {
    ledString(9+q, 11+q) = CRGB::Black;
  }


  if (current.E) {
    ledString(12+q, 14+q) = CRGB(red, green, blue);
  } else {
    ledString(12+q, 14+q) = CRGB::Black;
  }


  if (current.F) {
    ledString(15+q, 17+q) = CRGB(red, green, blue);
  } else {
    ledString(15+q, 17+q) = CRGB::Black;
  }


  if (current.G) {
    ledString(18+q, 20+q) = CRGB(red, green, blue);
  } else {
    ledString(18+q, 20+q) = CRGB::Black;
  }


}
