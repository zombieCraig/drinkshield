/*
 * DrinkShield Object
 */

#ifndef __DRINKSHIELD_H__
#define __DRINKSHIELD_H__

#include <iostream>
#include <stdlib.h>

#include "serial.h"

class Serial;

using namespace std;

class DrinkShield
{
public:
  DrinkShield(Serial *);
  ~DrinkShield();
  Serial *dev;
  int emulated;
  int verbose;

  void setVerbose(int);
  float pollVersion();
  void stillAlive();
  float version;
};

#endif  /* __DRINKSHIELD_H__ */
