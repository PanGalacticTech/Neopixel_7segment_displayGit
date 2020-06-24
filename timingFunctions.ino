
/*
 *  Functions here for timing and testing timing
 * 
 * 
 * 
 * 
 */





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
