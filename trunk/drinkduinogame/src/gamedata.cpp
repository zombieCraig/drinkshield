/*
 GameData - Main game class
 */

#include "gamedata.h"

GameData::GameData()
{
	srand(time(NULL));
	gui = new Gui(this);
	sql = new Sql();
	emulate_device = 0;
	verbose = 0;
	gameState = GAME_STATE_INIT;
	activePlayer = NULL;
	Winner = NULL;
	HighScore = 0;
	playerReady = NO;
	duiMode = 0;
	serialPorts.push_back(DEFAULT_SERIALPORT);
	addPlayersBox = NULL;
	allPlayersBox = NULL;
}

GameData::~GameData()
{
	delete sql;
	delete gui;
}

void GameData::setRCdir(string dir)
{
  struct stat st;
  string scoredb;
  rcdir = dir + "/.drinkduino/";
  gui->setRCdir(rcdir);
  // create directory if it doesn't exist
  if(stat(rcdir.c_str(), &st) != 0) {
	mkdir(rcdir.c_str(), 0755);
	if(verbose)
		cout << "Created " << rcdir << endl;
  }
  scoredb = rcdir + DB_FILE;
  sql->setDBFile(scoredb);
}

string GameData::getRCdir()
{
   return rcdir;
}

// Initializes attached drink shields
// Returns: number of shields initialized
int GameData::init_drinkShields()
{
   int detected = 0;
   string serialPort;
   DrinkShield *shield;
   Serial *dev = new Serial();
   int c;

   if(emulate_device) 
	dev->emulated = 1;

   for(c=0; c < serialPorts.size(); c++) {
	   serialPort = serialPorts.at(c);
	   if(verbose)
		cout << "Setting serial port to " << serialPort << endl;
	   dev->setPort(serialPort.c_str());
	   if(dev->init() > 0) {
		shield = new DrinkShield(dev);
		shield->setVerbose(verbose);
		if(shield->pollVersion() > 0) {
			if(verbose) {
				cout << "Device Detected: DrinkShield v" << shield->version;
				if(shield->dev->emulated)
					cout << " (emulated)";
				cout << endl;
			}
			shield->emulated = shield->dev->emulated;
			detected++;
			gui->splashShields(detected);
			addShield(shield);
		} else {
			if(verbose)
				cout << "Serial port did not contain a drinkShield." << endl;
		}
	   } else if(verbose) {
		cerr << "Could not initialize serial port" << endl;
   	}
   }
   return detected;		
}

// Adds an initialized shield object to the vector list
void GameData::addShield(DrinkShield *newshield)
{
    shields.push_back(newshield);
}

// Removes a shield from the list
void GameData::delShield(DrinkShield *badshield)
{
   bool found = 0;
   vector<DrinkShield *>::iterator target;

   for(vector<DrinkShield *>::iterator it = shields.begin(); it != shields.end(); ++it) {
	if(badshield == *it) {
		found = 1;
		target = it;
	}
   }
   if(found)
	shields.erase(target);
}

// Returns number of shields initialized
int GameData::numShields()
{
   return shields.size();
}

// Adds 1 to verbose level and syncs other objects
void GameData::addVerbose()
{
    verbose++;
    sql->verbose = verbose;
    gui->verbose = verbose;
}

// Abort Play.  Cancels a player ready attempt
void GameData::abortPlay()
{
    DrinkShield *s;

    if(!playerReady)
	return;
    setPlayerReady(OFF);
    gui->abortState = ABORT_DOWN;
    for(vector<DrinkShield *>::iterator it=shields.begin(); it != shields.end() && (s=*it); ++it) 
	s->dev->write_str(DEV_ABORT);
    gui->redraw();
}

// Choose the previous player
void GameData::prevPlayer()
{
	if(activePlayerLeft1())
		activePlayer = activePlayerLeft1();
}

// Choose Next player
void GameData::nextPlayer()
{
	if(activePlayerRight1())
		activePlayer = activePlayerRight1();
}

// Returns the player to the immediate left of the active player
// These functions are used by the statusbar
Player *GameData::activePlayerLeft1()
{
	Player *p;
	int c=0;
	for(vector<Player *>::iterator it = activePlayers.begin(); it != activePlayers.end() && (p=*it); ++it) {
		if(p == activePlayer && c > 0) 
			return activePlayers.at(c-1);
		c++;
	}
	return NULL;
}

// Returns the player 2nd to the left of the active player
// Method used by statusbar
Player *GameData::activePlayerLeft2()
{
	Player *p;
	int c=0;
	for(vector<Player *>::iterator it = activePlayers.begin(); it != activePlayers.end() && (p=*it); ++it) {
		if(p == activePlayer && c > 1) 
			return activePlayers.at(c-2);
		c++;
	}
	return NULL;
}

// Returns a player to the right of the active player
Player *GameData::activePlayerRight1()
{
	Player *p;
	int c=0;
	for(vector<Player *>::iterator it = activePlayers.begin(); it != activePlayers.end() && (p=*it); ++it) {
		if(p == activePlayer && c+1 < activePlayers.size()) 
			return activePlayers.at(c+1);
		c++;
	}
	return NULL;
}

// Returns a player object 2nd to the right of the active player
Player *GameData::activePlayerRight2()
{
	Player *p;
	int c=0;
	for(vector<Player *>::iterator it = activePlayers.begin(); it != activePlayers.end() && (p=*it); ++it) {
		if(p == activePlayer && c+2 < activePlayers.size()) 
			return activePlayers.at(c+2);
		c++;
	}
	return NULL;
}

// Recalibrate the DrinkShield
void GameData::recalibrateShields()
{
   DrinkShield *s;
   for(vector<DrinkShield *>::iterator it=shields.begin(); it != shields.end() && (s=*it); ++it)
	s->dev->write_str(DEV_RECAL);
}

// Tell drinkshield we are ready to record results
void GameData::recordResults()
{
    DrinkShield *s;

    if(!players.size()) {
	cerr << "No players added yet." << endl;
	return;
    } else if (!shields.size()) {
	cerr << "No identified drinkShields connected." << endl;
	return;
    }
    // Telling DrinkShields we are ready for a reading
    for(vector<DrinkShield *>::iterator it = shields.begin(); it != shields.end() && (s=*it); ++it) {
	s->dev->write_str(DEV_READY);
	s->dev->read_until('\n');
	if(s->dev->readBuf().compare(0,5,"Blow!") == 0) {
		setPlayerReady(ON);
	} else {
		cerr << "Err: DEV_Ready did not recieve normal response: " << s->dev->readBuf() << endl;
		return;
	}
    }
}

// Requeries the SQL DB to update information
void GameData::refreshPlayerData()
{
  Player *p;
  SDL_Surface *pic;
  char sqlcmd[255];

  // Wipe players
  players.clear();

  // Load player info
  if(!sql->exec("SELECT * FROM players", &addPlayerFromDB, 0))
	cerr << " Couldn't refresh player info" << endl;

  // Load scores and pix
  for(vector<Player *>::iterator it = players.begin(); it != players.end() && (p = *it); ++it) {
        snprintf(sqlcmd, 255, "SELECT * FROM scores WHERE id=%d ORDER BY score DESC LIMIT 10", p->id);

	if(!sql->exec(sqlcmd, &addScoreDataFromDB, p))
		cerr << " Could not refresh scores for player id " << p->id << endl;
	if(p->picpath.size() > 0)  {
		pic = gui->loadImage(p->picpath.c_str());
		if(pic)
			p->pic->addPic(pic);
		else // Error occured loading pic
			p->pic->addPic(gui->getAnonPic());
	} else 
		p->pic->addPic(gui->getAnonPic());
  }

  gui->splashFriends(players.size());

  if(verbose)
	dumpPlayerInfo();
}

// Retrieves a Player structure based on the id
Player *GameData::getPlayerByID(int id)
{
  Player *p;
  for(vector<Player *>::iterator it = players.begin(); it != players.end() && (p = *it); ++it) 
	if(p->id == id)
		return p;
  return 0;
}

void GameData::addPlayer(Player *p)
{
  players.push_back(p);
}

// Initializes the add players screen with data
void GameData::initAddPlayersDlg()
{
	Player *dbplayer, *gameplayer;
	int alreadyPlaying;

	allPlayersBox = new Scrollbar(gui, gui->playerscreen, 65, 150, 257, 403);
	addPlayersBox = new Scrollbar(gui, gui->playerscreen, 452, 150, 265, 206);

	for(vector<Player *>::iterator it = players.begin(); it != players.end() && (dbplayer = *it); ++it) {
		alreadyPlaying = 0;
		for(vector<Player *>::iterator it2 = activePlayers.begin(); it2 != activePlayers.end() && (gameplayer = *it2); ++it2) {
			if(dbplayer->id == gameplayer->id)
				alreadyPlaying = 1;
		}
		if(alreadyPlaying)
			addPlayersBox->add(dbplayer);
		else
			allPlayersBox->add(dbplayer);
	}
}

// Frees up some resources from the add players dialog
void GameData::destroyAddPlayersDlg()
{
	if(allPlayersBox) delete allPlayersBox;
	if(addPlayersBox) delete addPlayersBox;
}

// Move players from allPlayersBox to addPlayersBox
void GameData::moveAlltoAddBox()
{
	ScrollbarItem *entry;
	vector<ScrollbarItem *>allPlayers = allPlayersBox->getEntries();
	for(vector<ScrollbarItem *>::iterator it=allPlayers.begin(); it != allPlayers.end() && (entry = *it); ++it) {
		if(entry->enabled) {
			addPlayersBox->add(entry->player);
			allPlayersBox->del(entry->player);
		}
	}
	allPlayersBox->redraw();
	addPlayersBox->redraw();
}

// Move players from addPlayersBox to allPlayersBox
void GameData::moveAddtoAllBox()
{
	ScrollbarItem *entry;
	vector<ScrollbarItem *>addPlayers = addPlayersBox->getEntries();
	for(vector<ScrollbarItem *>::iterator it=addPlayers.begin(); it != addPlayers.end() && (entry = *it); ++it) {
		if(entry->enabled) {
			allPlayersBox->add(entry->player);
			addPlayersBox->del(entry->player);
		}
	}
	addPlayersBox->redraw();
	allPlayersBox->redraw();
}
// Debugging routine to dump player vector contents
void GameData::dumpPlayerInfo()
{
  Player *p;
  Score *s;
  int count;

  for(vector<Player *>::iterator it = players.begin(); it != players.end() && (p = *it); ++it) {
	count = 0;
	cout << "Player: " << p->name << " (" << p->picpath << ")" << endl;
	for(vector<Score *>::iterator it2 = p->scores.begin(); it2 != p->scores.end() && (s = *it2); ++it2) {
		count++;
		cout << "  " << count << ") " << s->points << endl;
	} 
  }
}

// Switches game states
void GameData::switchState(int newstate) {
   gameState = newstate;
   switch(newstate) {
   case GAME_STATE_INIT:
	cerr << "STATE_INIT directly called.  Currently unhandled." << endl;
	break;
   case GAME_STATE_ADD_PLAYERS:
	initAddPlayersDlg();
	gui->state = STATE_ADD_PLAYERS_SCREEN;
	gui->redraw();
	break;
   case GAME_STATE_ADD_NEW:
	gui->state = STATE_ADD_NEW_SCREEN;
	gui->addNewName.clear();
	gui->redraw();
	break;
   case GAME_STATE_MAIN:
	destroyAddPlayersDlg();
	if(activePlayers.size() > 0)
		activePlayer = activePlayers.at((int)activePlayers.size()/2);
	gui->state = STATE_MAIN_SCREEN;
	gui->redraw();
	break;
   default:
	cerr << "Unkown STATE called in switchState: " << newstate << endl;
   }
}

// Simple function to get a floating point equiv to the score and return it
float GameData::getPPM(int score)
{
	return ((float)score/1024) * 0.4;
}

// Updates ranking to determine winner and higest score
// Returns the winning player
Player *GameData::updateRanks()
{
   Player *p, *currentWinner;
   Score *sc;
   int highest;

   for(vector<Player *>::iterator it = players.begin(); it != players.end() && (p=*it); ++it) {
	sc = p->HighScore(0);
	if(sc) {
		if( sc->points > highest) {
			highest = sc->points;
			currentWinner = p;
		}
	}
   }
   Winner = currentWinner;
   HighScore = highest;
   return Winner;
}

// Get Players Ranking
int GameData::getRank(Player *p, int score)
{
   Score *sc;
   float ppm = getPPM(score);

   if(ppm < 0.02)
	return RANK_SOBER;

   updateRanks();
   
   if(score > HighScore) {
	HighScore = score;
	Winner = p;
	return RANK_WINRAR;
   }
   sc = p->HighScore(0);
   if(sc) { // If the player has a high score
	if(score > sc->points)
		return RANK_WINRAR;
	sc = p->ThirdScore(0);
	if(sc) {  // If the player has a 3 scores
	   if(score > sc->points)
		return RANK_DRUNK;
	}
   }
   if(score < 0.06)
	return RANK_HAPPY;
   return RANK_TIPSY;
}

// Sets the player the Ready Mode
void GameData::setPlayerReady(int state)
{
    playerReady = state;
    gui->togglePlayerLights(state);
}

// Parses the results from a DrinkShield
void GameData::parseResults(DrinkShield *s)
{
	int score;

	s->dev->read_until('\n');
	if(verbose)
		cout << "Got results.  Processing..." << endl;
	score = strtol(s->dev->readBuf().c_str(), NULL, 10);
	if(verbose)
		cout << "Score: " << score << endl;
	if(score > 10) { // You need to at least score a 10 to count
		sql->insertScore(activePlayer->id, score, time(0));
		gui->setGuyRank(getRank(activePlayer, score));
		gui->newScore(score);
		activePlayer->newScore(score);
		setPlayerReady(NO);
	} else {
		if(verbose)
			cout << "Results were too low, not recording" << endl;
	}
	gui->redraw();
}

// Poll the drinkshields for information
void GameData::pollDrinkShields()
{
	struct timeval timeout;
	int maxfd = 0;
	int serialfd = 0;
	int n = 0;
	DrinkShield *s;

	FD_ZERO(&serialset);

	// We only do this type of polling when waiting results
	if(playerReady == YES) {
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;
		for(vector<DrinkShield *>::iterator it = shields.begin(); it != shields.end() && (s = *it); ++ it) {
			if(s->emulated) {
				s->dev->randomScore();
				parseResults(s);
			} else {
				serialfd = s->dev->getfd();
				if(serialfd > maxfd) maxfd = serialfd;
				FD_SET(serialfd, &serialset);
			}
		}
		if(maxfd > 0) {  // We have active shields listening
			maxfd++;
			n = select(maxfd, &serialset, NULL, NULL, &timeout);
			if(n < 0) {
				cerr << "ERROR while polling drinkshield" <<endl;
			} else if (n > 0) {
				for(vector<DrinkShield *>::iterator it = shields.begin(); it != shields.end() && (s = *it); ++it)
					if(FD_ISSET(s->dev->getfd(), &serialset))
						parseResults(s);
			}
		}
	}
}

// DUI Mode simply changes thes core to floating point math
void GameData::enableDUImode()
{
	if(verbose)
		cout << "Enabled DUI Mode" << endl;
	duiMode = 1;
	gui->duiMode = 1;
}

// Adds players from the Add scrollbar to activeplayers
void GameData::addBoxtoActivePlayers()
{
	ScrollbarItem *entry;
	vector<ScrollbarItem *>addplayers = addPlayersBox->getEntries();

	activePlayers.clear();

	for(vector<ScrollbarItem *>::iterator it=addplayers.begin(); it != addplayers.end() && (entry = *it); ++it) 
		activePlayers.push_back(entry->player);
}

// Adds a new player to the DB
void GameData::addNewPlayer(string playerName, string picPath)
{
	SDL_Surface *pic;
	Player *p = new Player();
	p->name = playerName;
	p->picpath = picPath;
	if(p->picpath.size() > 0)  {
		pic = gui->loadImage(p->picpath.c_str());
		if(pic)
			p->pic->addPic(pic);
		else // Error occured loading pic
			p->pic->addPic(gui->getAnonPic());
	} else 
		p->pic->addPic(gui->getAnonPic());
	p->id=sql->insertPlayer(playerName, picPath);
	addPlayer(p);
}

// Adds a serial port
void GameData::addSerialPort(const char *port)
{
	serialPorts.push_back(string(port));
}

// Clears the serialPorts vector
void GameData::clearSerialPorts()
{
	serialPorts.clear();
}

// Main GameLoop
// Returns 0 when player exits
int GameData::mainLoop()
{
  int running = 1;
  int result;
  gui->updateTicks();

  while(running) {
	switch(gameState) {
	case GAME_STATE_ADD_NEW:
		result = gui->addNewEvents();
		if(result == -1) { // Done
			addNewPlayer(gui->addNewName, gui->addNewPicPath);
		}
		if(result < 0) { // Cancel or Done
			switchState(GAME_STATE_ADD_PLAYERS);
		}
		break;
	case GAME_STATE_ADD_PLAYERS:
		result = gui->addPlayerEvents();
		if(result == -1) { // Done
			addBoxtoActivePlayers();
			switchState(GAME_STATE_MAIN);
		} else if(result == -2) { // Add Player
			switchState(GAME_STATE_ADD_NEW);
		}
		break;
	case GAME_STATE_MAIN:
		gui->updateAnimations();
		pollDrinkShields();
		running = gui->pollEvents();
		break;
	default:
		cerr << "Unkown game state in mainLoop() " << gameState << endl;
	}
  }
  return running;
}
