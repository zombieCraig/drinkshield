/*
 * Scrollbar Class
 *
 * Primarily used with the add players screen
 *
 */

#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__

#include "gui.h"
#include "player.h"

#define DIR_UP   1
#define DIR_DOWN 2
#define PIC_H 50
#define PIC_W 50
#define SCROLL_ARROW_W 23
#define SCROLL_ARROW_H 23
#define SCROLL_ARROW_BORDER 4

class Player;
class Gui;

using namespace std;

class ScrollbarItem
{
public:
   ScrollbarItem(TTF_Font *, Player *);
   ~ScrollbarItem();
   SDL_Surface *pic;
   string name;
   int id;
   SDL_Surface *playerName;
   bool enabled;
   Player *player;
};

class Scrollbar
{
public:
  Scrollbar(Gui *, SDL_Surface *, int, int, int, int);
  ~Scrollbar();
   SDL_Surface *base;
  SDL_Rect scrollbox;
  SDL_Rect upArrow, downArrow;
  int index;
  int maxh;

  void add(Player *);
  void del(Player *);
  vector<ScrollbarItem *>getEntries();
  void redraw();
  int click(SDL_Event);
  void toggle(ScrollbarItem *);
  void scroll(int,int);
private:
  vector<ScrollbarItem *>entries;
  Gui *graphics;
};
#endif /* __SCROLLBAR_H__ */
