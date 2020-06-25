#include <FastLED.h>
#include "globals.h"

FASTLED_USING_NAMESPACE



#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    22                    // Data pin connection to LED string
//#define CLK_PIN   4                      // Not required for 3 pin LED strings
#define LED_TYPE    WS2812B                // This must be set to match the specific LED driver used

//#define LED_TYPE WS2811_400               //

#define COLOR_ORDER GRB

//#define NUM_LEDS   21                        // Number of LEDs in a single digit [Depreciated]

#define NUM_LEDS 126                             //Number of LEDS in 6 digits (will add more for the sign and dots later  need at least 7 for +- and 4 for dots            




//uint8_t numberOfLEDs = NUM_LEDS               // alternative methods, depreciated for now

//CRGB leds[NUM_LEDS];                            // alternative methods, depreciated for now

//CRGBSet A(digitLEDs, 21);                        // alternative methods, depreciated for now

// I could not work out how  to make this work so we are going for the big dumb hammer approach
//CRGBArray<21> digit0, digit1, digit2, digit3, digit4, digit5;


CRGBArray<NUM_LEDS> ledString;



uint8_t maxBrightness = 20;                      // Define the max brightness of all LEDs. 0 - 255. 20-50 reccomended for testing: MUST use seperate 5v supply for full brightness operation! Do not use USB power!

//   N.B. on power consumption:
// At full brightness each LED can draw 60 mA

// 60 mA * 126 = 7560 mA = 7.6 A !
// including the 14 extra = 8.4 A
// These LEDs can draw a decent amount of current and require a decent PSU. 10A @ 5v Recommended



int8_t colourSelect = 0;                        // selectes the display colour. 0 = skyrora blue, 1 = offwhite, 2 = yellowOrange. (currently cycles through each)

bool countingDown = true;                         // while true clock is "counting down" to t=0, if false clock is counting up from t=0



bool demoMode = false;                              // demo mode for animations // Defaults to OFF
int8_t demoPin = 15;                              // Short this pin to GND on boot up to force demo mode
int8_t demoPinValue;


void setup() {

  pinMode(demoPin, INPUT_PULLUP);

  demoPinValue = digitalRead(demoPin);                   // read the demo pin once to see if shorted

  delay(3000); // 3 second delay for recovery

  Serial.begin(115200);                                    // Serial for debugging if required


  if (demoPinValue == 0) {                                       // after delay check pin is still shorted
    demoPinValue = digitalRead(demoPin);
    if (demoPinValue == 0) {                                         // if false i.e (Shorted), set demoMode
      demoMode = true;
      Serial.println("Demo Mode Active");
    }
  } else {
    Serial.println("Countdown Clock Initialised");
  }


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


int8_t seconds = -3;    // initialised at -60 for testing
int8_t minutes = -1;
int8_t hours = 0;           // These just set the starting time on boot up, could be set to -23:59:59 in final implementation
// for now set lower for easier testing transition state


int8_t secondsLSF;     //seconds least significant figure
int8_t secondsMSF;      // seconds most singnificant figure



unsigned long countdownTiming;
unsigned long lastCountdownEvent = 0;
unsigned long countdownDelay = 1000;  // Counts down 1 every second


int8_t activeDigit = 0;  // this value used to control the digit that is currently being written to



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main Loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void loop() {


  // clock timing functions to take over when live data not available:
  countdownClock();


  // Function here to split up clock into individual digits to feed to display algorithms
  clocktodigits();




  displayedDigits[0] = alldigits[secondsLSF];                                          // current display data handed a digit from alldigits array, number set by the countdown placeholder
  // This line can be put into a for loop, and the pre seperated timing values passed to displayed digits array, then to the setDigit function


  if (!demoMode) {                                                                                                                             // if demo mode is inactive > Write the clock to the display

    setDigit(displayedDigits[0], activeDigit, currentColour.r, currentColour.g, currentColour.b);                                      // Turns LED segments on and off. Brightness is already set in setup loop,.

    //     Colour for "ON" LEDs passed as seperate RGB values an argument to the function
    //      currentColour variable is passed as seperate .r .b .g values, so these can be replaced directly with RGB values 0-255
  }

  else {                                                                                                 // else run the demo program

    runDemo();

  }

  FastLED.show();                          // print the data to all the LEDs


  //Serial.println(countdown);             // print for testing only
  // delay(1000);                          // delay for testing purposes only




}




void countdownClock() {                                           // Free Running Countdown clock to take over timing if data is lost (and testing)

  if (millis() - lastCountdownEvent >= countdownDelay) {               // if (the current elapsed time in milliseconds) - (the last time recorded) >= 1second

    lastCountdownEvent = millis();                                      // Record the new elapsed time

    seconds++;                                                         // add a second


    Serial.printf("H: %i M: %i S: %i", hours, minutes, seconds);          // serial print for testing
    Serial.println(" ");

    if (hours == 0 && minutes == 0) {

      if (seconds >= -20) {
        Serial.printf("T  %i seconds...", seconds);
        Serial.println(" ");
      }

      if (seconds == 0) {
        Serial.println("T = 0!");                                                      // when t=0 is reached, Print t=0
        Serial.println(" ");
        Serial.println(" ");
      }
    }


    if (hours < 0) {

      if (seconds >= 0) {                                                    // If seconds greater than 0
        minutes++;                                                              // Increment minuites
        seconds = -60;
        cycleColour();                                                               // Cycles colours once a minuite
        if (minutes >= 0) {
          hours++;
          minutes = -60;
        }
      }
    }

    if (hours == 0 && minutes < 0) {
      if (seconds >= 0) {                                                    // If seconds greater than 0
        minutes++;                                                              // Increment minuites
        seconds = -60;
        cycleColour();                                                               // Cycles colours once a minuite
      }
    }

    if (hours == 0 && minutes == 0) {
      if (seconds >= 10) {
      //  Serial.printf("T - %i seconds...", seconds);
      //  Serial.println(" ");
      }
      if (seconds >= 60) {                                                   // If seconds greater than 0
        minutes++;
        seconds = 0;
        cycleColour();
      }
    }

    if (hours >= 0 && minutes > 0) {
      if (seconds >= 60) {                                                    // If seconds greater than 0
        minutes++;
        seconds = 0;
        cycleColour();
        if (minutes >= 60) {
          hours++;
          minutes = 0;
        }
      }
    }
  }




  if (hours >= 24) {                                                               // if +24 hours is reached from t=0 clock resets to -23:59:50 from t=0
    hours = -23;
    minutes = -59;
    seconds = -59;
    countingDown = true;
    Serial.println("Clock Reset");
  }


}




void clocktodigits() {  // Function to split each clock value into seperate digits


  secondsLSF = seconds % 10;                                    // Splits up seconds into most significant and least significant figure
  secondsMSF = seconds / 10;

  if (secondsLSF < 0) {                                          // If negative, inverts digit to work with our array
    secondsLSF = secondsLSF * -1;                                 // character selection.
  }
  if (secondsMSF < 0) {
    secondsMSF = secondsMSF * -1;
  }

  // Do the same with minutes and Hours:


  // Serial.printf("MSF: %i  LSF:  %i", secondsMSF, secondsLSF);
  //Serial.println(" ");

}
