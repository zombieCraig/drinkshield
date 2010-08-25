/*
 DrinkDuino C++ Version

 Written for use with the Drink Shield

 (c) 2010 Craig Smith <craig@gfxhax.com>

*/

#include "drinkduino.h"

GameData *g = new GameData();

void usage(char *msg) {
   if(msg) cerr << "ERROR: " << msg << endl;
   cerr << "Usage: drinkduino <options>" << endl << endl;
   cerr << "   -h|--help          This usage message" << endl;
   cerr << "   -p|--port <serial> Serial port. Default: /dev/ttyUSB0" << endl;
   cerr << "   -e|--emulate       Emulate a drinkShield (debug mode)" << endl;
   cerr << "   -f|--fullscreen    Initialize in fullscreen mode" << endl;
   cerr << endl;

   exit(1);
}

int main(int argc, char *argv[]) {
    int cleared = 0;

     /* parse options */
    int option_index = 0, opt;
    static struct option loptions[] = {
        {"help",       no_argument,       0, 'h'},
        {"port",       required_argument, 0, 'p'},
        {"emulate",    no_argument,       0, 'e'},
        {"fullscreen", no_argument,       0, 'f'},
	{"dui",        no_argument,       0,  0 },
	{"verbose",    no_argument,       0, 'v'}
    };

    // Getopts
    while(1) {
        opt = getopt_long (argc, argv, "hp:fev",
                           loptions, &option_index);
        if (opt==-1) break;
        switch (opt) {
        case 0: 
	  if(!strncmp(loptions[option_index].name, "dui", 3)) 
		g->enableDUImode();
	  break;
        case 'p':
	  if(!cleared)
		g->clearSerialPorts();
	  g->addSerialPort(optarg);
	  cleared = 1;
          break;
        case 'f':
          g->gui->fullscreen = 1;
          break;
        case 'e':
          g->emulate_device = 1;
          break;
	case 'v':
	  g->addVerbose();
	  break;
        case 'h':
        default:
           usage(0);
        }
    }

    // Get Homedir info
    g->setRCdir(string(getenv("HOME")));

    if(!g->gui->splashScreen()) {
	cerr << "Could not initialize video. Aborting..." << endl;
	exit(1);
    }

    if(g->init_drinkShields() <= 0) {
	cerr << "Could not connect to drink shield.  Check port settings." << endl;
	exit(2);
    }
    if(g->verbose)
	cout << "Found " << g->numShields() << " shield(s)" << endl;

    if(!g->sql->init()) {
	cerr << "Could not initialize Scores." << endl;
	exit(3);
    }

    // Load up player vector info
    g->refreshPlayerData();

    // Pause for keypress (or mousebutton)
    g->gui->anyKey();

    // Done with Splash Screen and initialization.  It's game time.
    g->switchState(GAME_STATE_ADD_PLAYERS);

    // Main game handling loop
    g->mainLoop();

    delete g;
    SDL_Quit();
    return 0;
}
