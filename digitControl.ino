// Functions Relating to the control of the digits and data to the LEDs










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


void cycleColour(){    // Test Function to Cycle Colour of countdown clock periodically

  

    colourSelect++;                                     // increment current colour - cycle through colours
    if (colourSelect > 2) {                              // reset when reach the end of array
      colourSelect = 0;
    }


     // Just a way of testing the colour changing function through each cycle
    currentColour =  colourArray[colourSelect];            // pass currentColour the array containing data structure containing
    // colour data. Can be used in response to other events
    // in response to other events. e.g. when value turns negative
    // to positive.

  
}
