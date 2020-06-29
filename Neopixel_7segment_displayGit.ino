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






// ~~~~~~~~~~~~ Global Timing Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// These Hold the current displayed time in Seconds Minuites and Hours.
// Values can be negative or positive.

struct countDownTime {                                          //This is a data structure that contains the time as hours minutes and seconds
  int8_t h;
  int8_t m;
  int8_t s;
};

countDownTime countdownTime;     // Create an instance of the structure above to hold our master clock value

int32_t countDownMillis;   //an alternative master clock that only counts in milliseconds.


// Initial Timing Settings // This acts as a constant for the inital setup in the case of no other external start time input.
// These variables are not updated on every loop,
countDownTime initalCountdownTime{0, -2, -32};
//





// These variables hold the values printed to each digit of the display
int8_t secondsLSF;     //seconds least significant figure
int8_t secondsMSF;      // seconds most singnificant figure

int8_t minutesLSF;     //minutes least significant figure
int8_t minutesMSF;      // minites  most singnificant figure

int8_t hoursLSF;     //hours least significant figure
int8_t hoursMSF;      // hours  most singnificant figure

int8_t sixDigits[6] = {};   // array to hold variables above, allows us to iterate through them in a loop, writing each one to the displayed digit variable in order.


int8_t activeDigit = 0;  // this value used to control the digit that is currently being written to






int32_t millisOffset;     // This variable is subtracked from millis() reading in order to allow the clock to be updated periodically from database reads, then run off its own clock between updates.







// Set up inital millis, this sets up the start poiint for the clock. Called once at the end of setup.       


int32_t initalMillis;

void clockSet() {

  initalMillis = timeToMillis(countdownTime);

}



countDownTime outputTime;   

void millisToTime(int32_t inputMillis) {  // function to convert a total time in millis to  returns countdownTime






  outputTime.s = inputMillis / 1000;

  outputTime.m =  inputMillis / 60000;

  outputTime.h =  inputMillis  / 3600000;

  // Serial.printf("Total:   eH: %i   |   eM: %i   |   eS: %i   |", elapsedHours, elapsedMinutes, elapsedSeconds);  // This shows the total

  // Serial.println(" ");

  // This section subtracts the elapsed hours from the total minuites, then the total hours and total minuites from total seconds, leaving a "bucket" with each value seperated.
  // this works as the functions above round down by cutting off any decimal points

  outputTime.m =  outputTime.m  - ( outputTime.h * 60);

  outputTime.s =  outputTime.s  - (( outputTime.m + ( outputTime.h  * 60) ) * 60 );


  // return outputTime;           // this function does not return anything because C wont let you return structures for some reason. Instead use outputTime structure which is now declared as a global variable.

}









// int32_t outputMillis;   // Now made global variable because fuck you thats why

int32_t timeToMillis(countDownTime inputTime) {


  int32_t outputMillis = ((inputTime.s * 1000) + (inputTime.m * 60 * 1000) + (inputTime.h * 60 * 60 * 1000));             // Convert our current clock back into milliseconds


  return outputMillis;

}



void internalClock() {  // this function updates the countdownclock by using the functions above to convert the current countdown value to milliseconds. adding millis(). The converting the value back to a real time

  // int32_t millisAddr;

  // millisAddr = timeToMillis(countdownTime);


 int32_t millisPassed = millis() + initalMillis;


  millisToTime(millisPassed);


  countdownTime = outputTime;

}









// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Setup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void setup() {


  // Set up and get controller ready
  pinMode(demoPin, INPUT_PULLUP);

  demoPinValue = digitalRead(demoPin);                   // read the demo pin once to see if shorted

  delay(3000); // 3 second delay for recovery

  Serial.begin(115200);                                    // Serial for debugging if required

  // Pre load starting time

  countdownTime = initalCountdownTime;

  Serial.printf("Countdown MATHS TEST Starts @: %+2i:%2i:%2i", countdownTime.h, countdownTime.m, countdownTime.s);
  Serial.println(" ");
  delay(2000);

  int32_t millisAddr;

  millisAddr = timeToMillis(countdownTime);

  // Serial.println(countdownTime);

  Serial.printf("Time to Millis: %+i", millisAddr);
  Serial.println(" ");
  delay(2000);

  millisToTime(millisAddr);

  countdownTime = outputTime;  // output time is passed from millisToTime as a global variable


  Serial.printf("Countdown MATHS TEST Return: %+2i:%2i:%2i", countdownTime.h, countdownTime.m, countdownTime.s);
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" Above countdown times should match. If not Your Program is WRONG! ");

  delay(2000);

  // Check demopin for short to GND

  if (demoPinValue == 0) {                                       // after delay check pin is still shorted
    demoPinValue = digitalRead(demoPin);
    if (demoPinValue == 0) {                                         // if false i.e (Shorted), set demoMode
      demoMode = true;
      Serial.println("Demo Mode Active");
    }
  } else {
    Serial.println("Countdown Clock Initialised");
    Serial.printf("Countdown Starts @: %+2i:%2i:%2i", countdownTime.h, countdownTime.m, countdownTime.s);
    Serial.println(" ");
    Serial.println(" ");
  }


  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(ledString, NUM_LEDS).setCorrection(TypicalLEDStrip);            // This sets up the fist digit of the display


  // set master brightness control
  FastLED.setBrightness(maxBrightness);                                                                        // Sets the brightness for the entire string


  currentColour = skyroraBlue;   // Preset the LED colour, can be changed later in program

  for (int i = 0; i < NUM_LEDS; i++) {                                                                          // loop could be used to reset all LEDs to black on startup. Currently depreciated
    //  leds[i] = CRGB::Black;
  }

  clockSet();
  // internalClockReset() ;                                       // resets internal clock offset at zero < Last thing before ending setup
}














unsigned long updateDisplayDelay = 1000;  // delay between writing data to LEDs// Now only controls serialprint.  (will also be used to slow down printing to the serial monitor during testing)
unsigned long lastDisplayUpdate;        // save the time of the last update


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main Loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void loop() {



  // clock timing functions to take over when live data not available:

  //   countdownClock();                                                                  // Pick one function or the other

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


    // Serial.printf("Elapsed Millis: %l ",  elapsedMillis);

    Serial.printf("  %i : %i : %i ", countdownTime.h , countdownTime.m, countdownTime.s);
    Serial.println(" ");


    // Some If Statements for controlling events around T=0 mark

    if (countdownTime.h == 0 && countdownTime.m == 0) {

      if (countdownTime.s >= -20 && countdownTime.s <= 20) {

        Serial.printf("| T %+i  seconds |", countdownTime.s);  //    %+i means integer variable is passed to character string. + forces sign (+-)
        Serial.println(" ");

      }


    }



    // FastLED.show();                          // print the data to all the LEDs
    lastDisplayUpdate = millis();                                          // save the time for the next loop
  }

  FastLED.show();    // trying this on every loop to see if it works more smoothly


  if (countdownTime.s == 60 || countdownTime.s == -60 || countdownTime.s == 0) {                     // if seconds reach 0 (every minuite) change colour. Just for fun while testing.

    cycleColour();                                                               // Cycles colours once a minuite

  }
  //delay(500);

}






void clocktodigits() {  // Function to split each clock value into seperate digits bfore printing to display


  secondsLSF = countdownTime.s % 10;                                    // Splits up seconds into most significant and least significant figure
  secondsMSF = countdownTime.s / 10;

  if (secondsLSF < 0) {                                          // If negative, inverts digit to work with our array
    secondsLSF = secondsLSF * -1;                                 // character selection.
  }
  if (secondsMSF < 0) {
    secondsMSF = secondsMSF * -1;
  }


  minutesLSF = countdownTime.m % 10;                                    // Splits up minuites into most significant and least significant figure
  minutesMSF =  countdownTime.m / 10;

  if (minutesLSF < 0) {                                          // If negative, inverts digit to work with our array
    minutesLSF =  minutesLSF * -1;                                 // character selection.
  }
  if (minutesMSF < 0) {
    minutesMSF =  minutesMSF * -1;
  }


  hoursLSF = countdownTime.h % 10;                                    // Splits up Hours into most significant and least significant figure
  hoursMSF =  countdownTime.h / 10;

  if (hoursLSF < 0) {                                          // If negative, inverts digit to work with our array
    hoursLSF =  hoursLSF * -1;                                 // character selection.
  }
  if (hoursMSF < 0) {
    hoursMSF =  hoursMSF * -1;
  }







  // Serial.printf("MSF: %i  LSF:  %i", secondsMSF, secondsLSF);    //For Testing not required
  //Serial.println(" ");

}
