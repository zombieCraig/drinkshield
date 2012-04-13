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

int highscore = 0;     // Highscore value
int highscoreLed = 0;  // The highest LED
int lightLevel;        // Current LED level
int gotTopResult = 0;  // Used for instant response to system

void setup()
{
 // Initial Serial Port
 Serial.begin(9600);
 // Take 20 air samples when we first turn on the system
 ds.autocalibrate(20);
 // We initialize all the lights, reset them to ff
 ds.lightBarLevel(0, 0);
 ds.greenLight(OFF);
 // Turn player ready light to off (RED)
 ds.redLight(ON);

 // Tell the system what firmware version we support
 Serial.println(ds.version);
}

void loop()
{
  // Process any commands from the connected system
  ds.pollSerial();
  if(ds.playerReady) {
    // Player is Ready, check for reading...
    int val = ds.getReading();
    /* gotTopResult is used to respond to the system when
     the sensor is going down (got a lower than highscore
     reading 20 times)  This prevents the connectec machine
     from having to wait for the lightbar to go back down
     */
    if(val && gotTopResult < 20) {
      // We have a reading from the sensor, process it
      lightLevel = ds.getLightLevel(val);
      // We will not start recording unless there is a basic
      // level of alcohal detected (20)
      if(val > highscore && val >= 20) {
         highscore = val;
         highscoreLed = lightLevel; 
      } else if(lightLevel < highscoreLed) {
         // Bar is going back down, record it
         gotTopResult++;
      }
      // Display light level
      ds.lightBarLevel(lightLevel, highscoreLed);
      // If constant streams are enable then print results
      if (ds.streamResults) {
        Serial.print(val, DEC);
        Serial.println;
      }
    } else if(highscore) {
     // A highscore has been set so we had a reading, we are done
     ds.lightBarLevel(0, highscoreLed);
     // Report the Highscore value to the system
     Serial.print(highscore, DEC);
     Serial.println();
     // Blink highscore LED and Ready Light
     for(int cnt=0; cnt < 4; cnt++) {
       ds.greenLight(ON);
       ds.lightBarLevel(0, highscoreLed);
       delay(100);
       ds.greenLight(OFF);
       ds.lightBarLevel(0, 0);
       delay(100); 
     }
     // Mark player not ready, reset
     ds.playerReady = 0;
     ds.redLight(ON);
     ds.lightBarLevel(0, 0);
     highscore = 0;
     highscoreLed = 0;
     gotTopResult = 0;
    }
  }
}
