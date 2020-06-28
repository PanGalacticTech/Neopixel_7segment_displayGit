#include <FastLED.h>
#include "globals.h"
#include <DateTime.h>

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


  internalClockReset() ;                                       // resets internal clock offset at zero < Last thing before ending setup
}






// ~~~~~~~~~~~~ Global Timing Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// These Hold the current displayed time in Seconds Minuites and Hours.
// Values can be negative or positive.

int8_t seconds = -60;    // initialised at -60 for testing
int8_t minutes = -5;
int8_t hours = 0;           // These just set the starting time on boot up, could be set to -23:59:59 in final implementation
// for now set lower for easier testing transition state


// Initial Timing Settings // Due to a change in the maths, we now need a constant variable that holds the countdown clock start time, or the time at which it was last updated.
// These variables are not updated on every loop, unlike the variables above.

int8_t startSeconds = -34;
int8_t startMinutes = -1;
int8_t startHours = 0;

//



// These variables hold the values printed to each digit of the display
int8_t secondsLSF;     //seconds least significant figure
int8_t secondsMSF;      // seconds most singnificant figure

int8_t minutesLSF;     //minutes least significant figure
int8_t minutesMSF;      // minites  most singnificant figure

int8_t hoursLSF;     //hours least significant figure
int8_t hoursMSF;      // hours  most singnificant figure

int8_t activeDigit = 0;  // this value used to control the digit that is currently being written to


// These Variables are used for controlling the onboard (Real Time Clock (RTC) Function


uint32_t  currentTimeMillis;     // This variable will hold the start time + the elapsed time to = the real time in milliseconds

uint32_t  currentTimeSeconds;    // divides above value by 1000

uint32_t  elapsedMillis;     // Elapsed time since boot up  (millis()), but as a variable that can be reset

uint32_t  elapsedSeconds;

uint32_t  elapsedMinutes;

uint32_t  elapsedHours;

uint32_t millisOffset;     // This variable is subtracked from millis() reading in order to allow the clock to be updated periodically from database reads, then run off its own clock between updates.

bool clockReboot = true;   // triggers a total clock reset at seconds = 0; triggered once after startup


void internalClockReset() {  // call this function whenever the time is updated from a database read.  // May cause problems if millis() runs over and resets to zero, however this should only happen once every 49 days;
  // As the clock is not designed to run for that long this should work/ A function could be written to manually reset both millis() and millisOffset() at the same time to ensure robust function



  millisOffset = millis();

  elapsedTime();

  startSeconds = elapsedSeconds;
  startMinutes = elapsedMinutes;
  startHours  = elapsedHours;





  // Function here to update
  // startSeconds
  // startMinutes
  //startMHours
  // With values read from external server/database

}





void elapsedTime() {                                                    /// this only works if seconds always starts at -60 This will not do.

  // this section calculates the total elapsed time from millis()


  elapsedMillis = millis() - millisOffset;

  elapsedSeconds = elapsedMillis / 1000;                  //   100,000,000 > 100,000 Seconds

  elapsedMinutes = elapsedSeconds / 60;                      //// 100,000 > 1666 minuite

  elapsedHours = elapsedMinutes / 60;                         // 27 hours    >>> 27*60 = 1620    ||   1666 - 1620 =  46 mins

  // Serial.printf("Total:   eH: %i   |   eM: %i   |   eS: %i   |", elapsedHours, elapsedMinutes, elapsedSeconds);  // This shows the total

  // Serial.println(" ");

  // This section subtracts the elapsed hours from the total minuites, then the total hours and total minuites from total seconds, leaving a "bucket" with each value seperated.
  // this works as the functions above round down by cutting off any decimal points

  elapsedMinutes = elapsedMinutes - (elapsedHours * 60);

  elapsedSeconds = elapsedSeconds - ((elapsedMinutes * 60) + (elapsedHours * 60));


  //  Serial.printf("Recalc:  eH: %i   |   eM: %i   |   eS: %i   |", elapsedHours, elapsedMinutes, elapsedSeconds);  // This shows the total

  // Serial.println(" ");


}




bool tZeroTrigger = false;   // required to bug fix around t=0 transition


void internalClock() {                             //  more accurate than our previous clock function, this time using internal clock and a library to get the individual times.
  // Could be done by calling millis() then working out elapsed hours, mins & seconds manually, but the library will save some work. < Library was out of date and did not function
  // So I have re-written the elapsedTime function to do the job of the library.


  elapsedTime();     // Function to get elapsed times from internal clock


  seconds = startSeconds + elapsedSeconds;

  minutes = startMinutes + elapsedMinutes;

  hours = startHours + elapsedHours;


  // Bug fix, Not a fan of this but it should work: // Problem with transition through T=0 Reset clock to starting values 0:0:0

  if (!tZeroTrigger) {
    if (hours == 0 && minutes == 0 && seconds >= -1) {

      tZeroTrigger = true;
      Serial.println("T = 0");
      Serial.println(" ");
      Serial.println(" ");
      startSeconds = 0;
      startMinutes = 0;
      startHours = 0;

      internalClockReset();
    }
  }

  if (seconds == 0) {                   // another bug fix to allow for odd
    internalClockReset();
  }

  if (hours >= 100) {   // if hours roll over to t+ 100 hours, reset clock to -24 hours

    startSeconds = -60;
    startMinutes = -60;
    startHours = -23;
    internalClockReset();
    tZeroTrigger = false;   //reset this trigger


  }

  // Serial.printf("  %i : %i : %i ", hours, minutes, seconds);
  // Serial.println(" ");


}








unsigned long updateDisplayDelay = 1000;  // delay between writing data to LEDs// Now only controls serialprint.  (will also be used to slow down printing to the serial monitor during testing)
unsigned long lastDisplayUpdate;        // save the time of the last update


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main Loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void loop() {



  if (clockReboot) {
    if (seconds == 0) {
      internalClockReset();

      clockReboot = false;
    }
  }


  // clock timing functions to take over when live data not available:

  // countdownClock();                                                                  // Pick one function or the other

  internalClock();                                                                         // Pick one function or the other



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


  if ( (millis() - lastDisplayUpdate) >= updateDisplayDelay) {             // if the current time - the last time the LED display was updated.


    Serial.printf("Elapsed Millis: %l ",  elapsedMillis);

    Serial.printf("  %i : %i : %i ", hours, minutes, seconds);
    Serial.println(" ");


    // Some If Statements for controlling events around T=0 mark

    if (hours == 0 && minutes == 0) {

      if (seconds >= -20 && seconds <= 20) {

        Serial.printf("| T %+i  seconds |", seconds);  //    %+i means integer variable is passed to character string. + forces sign (+-)
        Serial.println(" ");

      }


    }



    // FastLED.show();                          // print the data to all the LEDs
    lastDisplayUpdate = millis();                                          // save the time for the next loop
  }

  FastLED.show();    // trying this on every loop to see if it works more smoothly


  if (seconds == 60 || seconds == -60) {                     // if seconds reach 0 (every minuite) change colour. Just for fun while testing.

    cycleColour();                                                               // Cycles colours once a minuite

  }
  //delay(500);

}






void clocktodigits() {  // Function to split each clock value into seperate digits bfore printing to display


  secondsLSF = seconds % 10;                                    // Splits up seconds into most significant and least significant figure
  secondsMSF = seconds / 10;

  if (secondsLSF < 0) {                                          // If negative, inverts digit to work with our array
    secondsLSF = secondsLSF * -1;                                 // character selection.
  }
  if (secondsMSF < 0) {
    secondsMSF = secondsMSF * -1;
  }


  minutesLSF = minutes % 10;                                    // Splits up minuites into most significant and least significant figure
  minutesMSF =  minutes / 10;

  if (minutesLSF < 0) {                                          // If negative, inverts digit to work with our array
    minutesLSF =  minutesLSF * -1;                                 // character selection.
  }
  if (minutesMSF < 0) {
    minutesMSF =  minutesMSF * -1;
  }


  hoursLSF = hours % 10;                                    // Splits up Hours into most significant and least significant figure
  hoursMSF =  hours / 10;

  if (hoursLSF < 0) {                                          // If negative, inverts digit to work with our array
    hoursLSF =  hoursLSF * -1;                                 // character selection.
  }
  if (hoursMSF < 0) {
    hoursMSF =  hoursMSF * -1;
  }







  // Serial.printf("MSF: %i  LSF:  %i", secondsMSF, secondsLSF);    //For Testing not required
  //Serial.println(" ");

}
