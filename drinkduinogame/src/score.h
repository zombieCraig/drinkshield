/*
 * Score Class
 */
#ifndef __SCORE_H__
#define __SCORE_H__

#include <time.h>

using namespace std;

class Score
{
public:
  Score(int);
  ~Score();
  int points;
  time_t recordedOn;

  float getPPM();
};

#endif /* __SCORE_H__ */
