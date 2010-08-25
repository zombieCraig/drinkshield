/*
 * Score Class
 */

#include "score.h"

Score::Score(int pts)
{
	points = pts;
	recordedOn = 0;
}

Score::~Score()
{

}

// Converts points to floating point
float Score::getPPM()
{
    return ((float)points/1024) *0.4;
}

