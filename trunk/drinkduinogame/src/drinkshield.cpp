/*
 * DrinkShield Class
 */

#include "drinkshield.h"

DrinkShield::DrinkShield(Serial *serialdev)
{
  dev = NULL;
  emulated = 0;
  version = 0;
  verbose = 0;

  if(serialdev == NULL)
	emulated = 1;
  else 
  	dev = serialdev;
}

DrinkShield::~DrinkShield()
{

}

void DrinkShield::setVerbose(int v)
{
   verbose = v;
   if(dev)
	dev->verbose = v;
}
// Send an INIT to the serial and check response
// Returns version number on success
float DrinkShield::pollVersion()
{
	string buf;
	if(verbose)
		cout << "Polling device version info..." << endl;
	dev->write_str(DEV_INIT);
	if(dev->read_until('\n') == 0) {
		buf = dev->readBuf();
        if(buf.size() == 0) {
            cerr << "ERR: No data returned will polling version info" << endl;
            return 0;
        }
		if(verbose >= 2)
			    cout << "Buffer: " << buf << endl;
		string id(buf, 0, 11);
		string ver(buf, 13); 
		if(id.compare("drinkShield") == 0) {
			version = strtof(ver.c_str(), 0);
			return version;
		} else {
			cerr << "Not a drinkShield." << endl;
		}
	} else {
		cerr << "Error reading device. " << endl;
		return 0;
	}
	return 0;
}

// Send a command over serial to the arduino/drinkshield to inform it that it is still attached to a computer
void DrinkShield::stillAlive()
{
    dev->write_str(DEV_ALIVE);
    if(verbose)
        cout << "Sending Still Alive signal." << endl;
}
