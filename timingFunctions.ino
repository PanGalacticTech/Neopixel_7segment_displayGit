
/*
    Functions here for timing and testing timing




*/












// These variables directly relate to the countdownClock function They are not required for any real time keeping functions

unsigned long countdownTiming;
unsigned long lastCountdownEvent = 0;
unsigned long countdownDelay = 1000;  // @ 1000 Counts down 1 every second  // reduce this number to make clock run faster for testing




void countdownClock() {                                           // Free Running Countdown clock to take over timing if data is lost (and testing) // This is a bad way of implementing this as any drift in the time a loop takes will lead to the clock drifting over time.
  // A better clock will count the total elapsed time and use this to count down from a starting value
  // Left in:
  //1. for testing, This clock can run faster up by reducing the interval between counts (countdownDelay);
  //2. to show the process I went through in developing the program.
  //3. Its good to remind myself of why ideas are bad.

  if (millis() - lastCountdownEvent >= countdownDelay) {               // if (the current elapsed time in milliseconds) - (the last time recorded) >= 1second

    lastCountdownEvent = millis();                                      // Record the new elapsed time

    seconds++;                                                         // add a second


    Serial.printf("H: %i M: %i S: %i", hours, minutes, seconds);          // serial print for testing
    Serial.println(" ");

    if (hours == 0 && minutes == 0) {

      if (seconds >= -20 && seconds <= 20) {                             // other animations could be applied during the time around T = 0
        Serial.printf("T  %i seconds...", seconds);
        Serial.println(" ");
      }

      if (seconds == 0) {
        Serial.println("T = 0!");                                                      // when t=0 is reached, Print t=0 to serial
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

    /*
        if (hours == 0 && minutes >= 0) {
          if (seconds >= 60) {                                                   // If seconds greater than 0
            minutes++;                                                            // This section is redundant
            seconds = 0;
            cycleColour();
          }
        }
    */
    if (hours >= 0 && minutes >= 0) {
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





// Really Really Simple Countdown timer used for testing a single digit. No required for final function


int8_t countdown = 0;            // Countdown timer placeholder for testing.



void testCountdown() {   // Simple test countdown timer. Just for testing:

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
