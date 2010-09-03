/*
 * Countdown Timer Class
 */

#include "countdowntimer.h"

/* Init with the value in seconds to countdown */
CountdownTimer::CountdownTimer(int seconds)
{
  startCount = seconds * 1000;
  currentCount = startCount;
  activated = 0;
  lastticks = SDL_GetTicks();
}

CountdownTimer::~CountdownTimer()
{

}

/* Sets the countdown seconds */
void CountdownTimer::setSeconds(int seconds)
{
   startCount = seconds * 1000;
}

/* Start the countdown */
void CountdownTimer::start()
{
   lastticks = SDL_GetTicks();
   activated = 1;
   currentCount = startCount;
}

/* Resets the countdown timer */
void CountdownTimer::reset()
{
   currentCount = startCount;
}

/* Deactivates & Resets Timer */
void CountdownTimer::stop()
{
   activated = 0;
   reset();
}

/* Pauses the timer */
void CountdownTimer::pause()
{
   activated = 0;
}

/* Resume from a paused state */
void CountdownTimer::resume()
{
   activated = 1;
   lastticks = SDL_GetTicks();
}

/* Updates and returns the current count */
int CountdownTimer::getCount()
{
   int ticks = SDL_GetTicks();
   currentCount -= (ticks - lastticks);
   lastticks = ticks;
   if(currentCount <= 0) {
	currentCount = 0;
	return 0;
   }
   return currentCount / 1000;
}

bool CountdownTimer::isActive()
{
   return activated;
}
