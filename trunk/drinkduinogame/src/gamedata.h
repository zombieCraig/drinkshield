/*
 GameData class

 Main class for game data
 */

#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <string>

#include "gui.h"
#include "sql.h"
#include "drinkshield.h"
#include "player.h"
#include "scrollbar.h"
#include "countdowntimer.h"

#define NO  0
#define YES 1
#define DEFAULT_SERIALPORT "/dev/ttyUSB0"
#define BOOTH_START_TIME 5
#define GAME_STATE_INIT -1
#define GAME_STATE_MAIN 0
#define GAME_STATE_ADD_PLAYERS 1
#define GAME_STATE_ADD_NEW 2

class Gui;
class Sql;
class Player;
class GameData;
class Scrollbar;
class CountdownTimer;

extern GameData *g;

using namespace std;

class Player;

class GameData
{
public:
  GameData();
  ~GameData();
  Gui *gui;
  Sql *sql;
  int emulate_device;
  int verbose;
  string rcdir;
  Player *activePlayer;
  Player *Winner;
  int HighScore;
  vector<Player *>players;
  vector<Player *>activePlayers;
  int gameState;
  int playerReady;
  int duiMode;

  int init_drinkShields();
  void addShield(DrinkShield *);
  void delShield(DrinkShield *);
  int numShields();
  void addVerbose();
  void addPlayer(Player *);
  Player *getPlayerByID(int);
  void refreshPlayerData();
  void setRCdir(string);
  void dumpPlayerInfo();
  string getRCdir();
  void switchState(int);
  int gameLoop();
  void parseResults(DrinkShield *);
  int getRank(Player *, int);
  float getPPM(int);
  Player *updateRanks();
  int mainLoop();
  void pollDrinkShields();
  void setPlayerReady(int);
  void recordResults();
  void abortPlay();
  void prevPlayer();
  void nextPlayer();
  void recalibrateShields();
  void enableDUImode();
  void addSerialPort(const char *);
  void clearSerialPorts();
  void moveAlltoAddBox();
  void moveAddtoAllBox();
  void addBoxtoActivePlayers();
  void addNewPlayer(string);
  void takePlayerPictures();
  void savePictures(string);
  void loadPlayerPics(Player *);
  Player *activePlayerLeft1();
  Player *activePlayerLeft2();
  Player *activePlayerRight1();
  Player *activePlayerRight2();
  Scrollbar *allPlayersBox;
  Scrollbar *addPlayersBox;
  CountdownTimer *boothTimer;

  /* Callbacks */
  static int addPlayerFromDB(void *blah, int argc, char *argv[], char *col[]) {
   int id = strtol(argv[0], NULL, 10);
   Player *p = g->getPlayerByID(id);

   // Player data is already present
   if(p) {
	p->name = string(argv[1]);
	return 0;
   }

   p = new Player();
   p->id = id;
   p->name = string(argv[1]);

   g->addPlayer(p);
   return 0;
   }
  static int addScoreDataFromDB(void *p, int argc, char *argv[], char *col[]) {
	Player *tmp;
	tmp = (Player *)p;
	//(Player *)p->addScoreFromDB(strtod(argv[1], NULL));
	tmp->addScoreFromDB(strtod(argv[1], NULL));
	return 0;
   }
private:
  vector<DrinkShield *> shields;
  vector<string>serialPorts;
  fd_set serialset;
  int gameSate;

  void initAddPlayersDlg();
  void destroyAddPlayersDlg();
};

#endif /* __GAMEDATA_H__ */
