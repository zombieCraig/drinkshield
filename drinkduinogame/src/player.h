/*
 * Player Class
 */
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include "score.h"
#include "profilepic.h"

class Score;
class ProfilePic;

using namespace std;

class Player
{
public:
  Player();
  ~Player();
  int id;
  string name;
  ProfilePic *pic;
  vector<Score *>scores;
  void addScoreFromDB(int);
  Score *HighScore(int);
  Score *FirstScore(int);
  Score *SecondScore(int);
  Score *ThirdScore(int);
  void newScore(int);
};

#endif /* __PLAYER_H__ */
