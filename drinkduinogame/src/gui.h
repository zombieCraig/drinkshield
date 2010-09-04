/*
  GUI Class

  Handles the SDL and screen routines

 */

#ifndef __GUI_H__
#define __GUI_H__

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#include "../config.h"

#include "gamedata.h"
#include "camera.h"

#define PIC_DIR "pics"
#define FONT_DIR "fonts"
#define IMG_DIR "images"
#define ANONPIC "anon.jpg"

#define DRINK_MSG "Drink Byatch!"

#define VID_W 800
#define VID_H 600

#define GUY_FRAMES 4
#define GUY_FRAME_W 75
#define GUY_FRAME_H 100
#define GEAR_FRAME_W 42
#define GEAR_FRAME_H 40
#define GEAR_FRAMES 4
#define ABORT_FRAME_W 42
#define ABORT_FRAME_H 40
#define FRIEND_FRAME_W 42
#define FRIEND_FRAME_H 40
#define RANK_HAPPY 0
#define RANK_TIPSY 1
#define RANK_DRUNK 2
#define RANK_WINRAR 3
#define RANK_SOBER 4
#define STATE_SPLASH_SCREEN -1
#define STATE_MAIN_SCREEN 0
#define STATE_SCORE_UP    1
#define STATE_SCORE_DOWN  2
#define STATE_SCORE_HOVER 3
#define STATE_ADD_PLAYERS_SCREEN 4
#define STATE_ADD_NEW_SCREEN 5
#define SCORE_FRAMERATE 12
#define HOVER_FRAMERATE 2000
#define GUY_FRAMERATE   62
#define GEAR_FRAMERATE  74
#define ABORT_UP 0
#define ABORT_OVER 1
#define ABORT_DOWN 2
#define MAX_PLAYER_NAME_LEN 15

#define OFF 0
#define ON 1

class GameData;
class Camera;

using namespace std;

class Gui
{
public:
  Gui(GameData *);
  ~Gui();
  int fullscreen;
  int verbose;
  string data_path;
  string pic_path;
  string font_path;
  string img_path;
  string rcpath;
  string addNewName, addNewPicPath;
  GameData *game;
  SDL_Surface *screen, *playerscreen, *addnewscreen;
  int state, gearState, abortState,;
  int duiMode;

  int splashScreen();
  int initGraphics();
  void initImages();
  void initVideoMode();
  void loadFonts();
  void setRCdir(string);
  void redraw();
  void drawGear();
  void drawAbort();
  void drawFriendIcon();
  void drawPlayerInfo();
  void newScore(int);
  void setGuyRank(int);
  void updateTicks();
  void updateAnimations();
  void light(SDL_Rect, int);
  void togglePlayerLights(int);
  int pollEvents();
  int addPlayerEvents();
  int addNewEvents();
  void mouseover();
  bool isover(SDL_Rect);
  void updateProfileBar();
  void anyKey();
  void splashShields(int);
  void splashFriends(int);
  void splashMsg(const char *);
  void updateAddNewNamebox();
  bool enableCamera();
  void disableCamera();
  void updateNewPlayerAnimations();
  void updateSnapshots();
  TTF_Font *getListFont();
  SDL_Surface *createBlankSurface(int,int);
  SDL_Surface *loadImage(const char *);
  SDL_Surface *getAnonPic();
  Camera *cam;
private:
  SDL_Surface *splashscreen;
  SDL_Surface *anonpic;
  SDL_Surface *scrap;
  SDL_Surface *maindlg, *maindlgon, *statusbar;
  SDL_Surface *scoredlg, *newscore;
  SDL_Surface *guyanim;
  SDL_Surface *configgear, *abortbtn, *friendbtn;
  SDL_Rect leftArrow;
  SDL_Rect rightArrow;
  SDL_Rect readyBtn;
  SDL_Rect redLight, greenLight;
  SDL_Rect mainpic, l1pic, l2pic, r1pic, r2pic;
  SDL_Rect scoredst, scorenow;
  SDL_Rect gearLoc, abortLoc, friendLoc;
  SDL_Rect donebox, addArrow, removeArrow, addNewbtn;
  SDL_Rect addDonebox, addCancelbox, addSmilebtn, addNamebox;
  SDL_Rect addWebcam, addPic1, addPic2, addPic3, addPic4;
  SDL_Event event;
  TTF_Font *NameFont;
  TTF_Font *ScoreFont;
  TTF_Font *SplashMsgFont;
  TTF_Font *NumberFont;
  TTF_Font *ListFont;
  TTF_Font *NewNameFont;
  TTF_Font *CountFont;
  int oldticks, oldguyticks, gearticks;
  int scoreidx;
  int currentGuyFrame, guyRank;
};

#endif /* __GUI_H__ */
