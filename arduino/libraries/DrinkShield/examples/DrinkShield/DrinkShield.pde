/*
 * DrinkShield - DrinkDuino Game Sketch
 *
 * Base firmware for use with the DrinkDuino game
 * 
 * NOTE: You will also need the DrinkDuino software that talks
 * to the drinkShield along with this firmware.
 *
 * Craig Smith <craig@gfxhax.com>
 */

#include "DrinkShield.h"

// 0.1 is the version written on the DrinkShield board
DrinkShield ds(0,1);

int mode = STANDALONE; // Either STANDALONE or if connected to computer DRINKDUINO.

void setup()
{
  // We initialize all the lights, reset them to ff
  ds.lightBarLevel(0, 0);
  ds.greenLight(OFF);
  // Turn player ready light to off (RED)
  ds.redLight(ON);
}

void loop()
{
  if((millis() - ds.lastAlive) > 3000 && mode == DRINKDUINO) // Check if in computer mode and it's been more than 3 seconds since Still Alive signal
  {
    // switch over to standalone and initialize that stuff
  } else if((millis() - ds.lastAlive) < 3000 && mode == STANDALONE) // Check if in stand alone and we have recently recieved a Still Alive signal
    // switch over to drinkduino/computer mode

  }
  
  if(mode == STANDALONE)
  {
    // standalone code here
  } else if(mode == DRINKDUINO) {
    // drinkduino code here
  } else {
    // error
  }
}
