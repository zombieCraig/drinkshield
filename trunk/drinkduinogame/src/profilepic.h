/*
 * Profile Picture class for each player
 *
 */

#ifndef __PROFILE_PIC_H__
#define __PROFILE_PIC_H__

#include <vector>
#include <SDL/SDL.h>

#define PIC_FRAMERATE 512

using namespace std;

class ProfilePic
{
public:
   ProfilePic();
   ~ProfilePic();
   void addPic(SDL_Surface *);
   SDL_Surface *getPic();
   SDL_Surface *firstPic();
   SDL_Surface *nextPic();
private:
   int oldticks;
   int frame;
   vector<SDL_Surface*>pics;

   void nextFrame();
};

#endif /* __PROFILE_PIC_H__ */
