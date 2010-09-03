/*
 * Countdown Timer Class
 *
 * Initialize with a # in seconds. When active it counts down.
 *
 */

#ifndef __COUNTDOWN_TIMER_H__
#define __COUNTDOWN_TIMER_H__

#include <SDL/SDL.h>

using namespace std;

class CountdownTimer
{
public:
   CountdownTimer(int);
   ~CountdownTimer();
   void start();
   void reset();
   void stop();
   void pause();
   void resume();
   void setSeconds(int);
   int getCount();
   bool isActive();
private:
   bool activated;
   int startCount;
   int currentCount;
   int lastticks;
};

#endif /* __COUNTDOWN_TIMER_H__ */
