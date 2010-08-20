#include "WProgram.h"
#include "DrinkShield.h"

DrinkShield::DrinkShield(int major, int minor)
{
   switch(major){
   case 0:
   default:
	switch(minor) {
	case 1:
	defaut:  // We assume the first gen boards if unknown vesrion
		_mq3pin = 0;
		_SHpin = 6;
		_STpin = 7;
		_DSpin = 8;
		_blinkCnt = 3;
		_numLights = 11;

		_highscore = 0;
		_playerReady = 0;
		_baseline = 70;
		_startTime = 0;

		_currentLevel = 0;
		_greenLightState = 0;
		_redLightState = 0;
		sprintf(version, "drinkShield v%0.2f", 0.3);

		pinMode(_STpin, OUTPUT);
		pinMode(_SHpin, OUTPUT);
		pinMode(_DSpin, OUTPUT);
		break;
	}
        break;
    }
}

// Updates all the lights
void DrinkShield::refreshLights()
{
	int cnt;

	digitalWrite(_SHpin, LOW);
	digitalWrite(_STpin, LOW);
	// First set the red and green lights
	if(_greenLightState) {
		digitalWrite(_DSpin, HIGH);
	} else {
		digitalWrite(_DSpin, LOW);
	}
	digitalWrite(_SHpin, HIGH);
	digitalWrite(_SHpin, LOW);
	if(_redLightState) {
		digitalWrite(_DSpin, HIGH);
	} else {
		digitalWrite(_DSpin, LOW);
	}
	digitalWrite(_SHpin, HIGH);
	digitalWrite(_SHpin, LOW);

	// Set light bar
	for(cnt = _numLights; cnt > 0; cnt--) {
		if(cnt <= _currentLevel || cnt == _highscoreLevel) {
			digitalWrite(_DSpin, HIGH);
		} else {
			digitalWrite(_DSpin, LOW);
		}
		digitalWrite(_SHpin, HIGH);
		digitalWrite(_SHpin, LOW);
	}
	// Throw Latch
	digitalWrite(_STpin, HIGH);
	digitalWrite(_STpin, LOW);
}

// Sets both player lights at the same time
void DrinkShield::setPlayerLights(int red, int green) {
	_redLightState = red;
	_greenLightState = green;
	refreshLights();  
}

void DrinkShield::redLight(int val) {
	_redLightState = val;
	refreshLights(); 
}

void DrinkShield::greenLight(int val) {
	_greenLightState = val;
	refreshLights(); 
}

// Turns up all lights up to level
void DrinkShield::lightBarLevel(int level, int hs) {
	_currentLevel = level;
	_highscoreLevel = hs;
	refreshLights();
}

// Test the lights
void DrinkShield::lightTest() {
	int c1; 

	lightBarLevel(0, 0);
	for(c1 =0; c1 < _numLights; c1++) {
		lightBarLevel(c1+1, 0);
		delay(200);
	}
	delay(500);
	for(c1=_numLights; c1 > 0; c1--) {
		lightBarLevel(c1, 0);
		delay(200);
	}
	lightBarLevel(0, 0);
}

// Uses the DrinkShield protocol to talk to SW
void DrinkShield::pollSerial()
{
	int inByte;
	if(Serial.available() > 0) {
		inByte = Serial.read();
		switch(inByte) {
		case 'i':	// SW Init message
			setPlayerLights(ON, OFF);
			Serial.println(version);
			break;
		case 'r':	// Player Ready
			setPlayerLights(OFF, ON);
			Serial.println("Blow!");
			_playerReady = 1;
			_highscore = 0;
			_startTime = 0;
			break;
		case 'a':	// Auto-Calibrate
			setPlayerLights(ON, ON);
			autocalibrate(20);
			break;
		case 'l':	// Light test
			lightTest();
			break;
		case 'A':	// Abort player ready
			_playerReady = 0;
			break;
		}
	}
}

// Auto-calibrates the device
void DrinkShield::autocalibrate(int samples)
{
	int val, i, firstcal, lastnum;
	firstcal = 1;
	for(i = 0; i < samples; i++) {
		val = analogRead(_mq3pin);
		if(firstcal) {
			firstcal = 0;
			lastnum = val;
		} else {
			lastnum = (lastnum + val) / 2;
		}
		if(i % 2) 
			setPlayerLights(OFF, OFF);
		else
			setPlayerLights(ON, ON);
		delay(100);
	}
	_baseline = lastnum + 20;
}

// Returns the value from the MQ-3 Sensor minus the baseline
// Returns the number greater than the baseline or 0
int DrinkShield::getReading()
{
	int val = analogRead(_mq3pin);
	val -= _baseline;
	if(val > 0)
		return val;
	return 0;
}

// Returns true if the MQ-3 Sensor is cleared enough for another reading
bool DrinkShield::isClear()
{
	int val = getReading();
	if(val)
		return 1;
	return 0;
}

// Map the current reading to a light level
int DrinkShield::getLightLevel(int val)
{
	return map(val, 0, 256-_baseline, 0, _numLights);
}
