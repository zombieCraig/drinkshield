/*
 * Player Class
 */

#include "player.h"

Player::Player()
{
   id = 0;
   pic = new ProfilePic();
}

Player::~Player()
{
  scores.clear();
}

void Player::addScoreFromDB(int points)
{
	Score *s = new Score(points);
	scores.push_back(s);
}

// Returns the players top score since a set time
// TODO: Implement 'since'
Score *Player::HighScore(int since)
{
	Score *s, *hs = NULL;
	int highest = 0;

	for(vector<Score *>::iterator it = scores.begin(); it != scores.end() && (s = *it); ++it) {
		if(s->points > highest) {
			highest = s->points;
			hs = s;
		}
	}
	return hs;
}

// Wrapper for HighScore
Score *Player::FirstScore(int since)
{
	return HighScore(since);
}

// Returns the 2nd highest score for the player
Score *Player::SecondScore(int since)
{
	Score *fs = HighScore(since);
	Score *s, *hs = NULL;
	int first = 0;
	int second = 0;

	if(!fs) return NULL;

	first = fs->points;

	for(vector<Score *>::iterator it = scores.begin(); it != scores.end() && (s = *it); ++it) {
		if(s->points > second && s->points < first) {
			second = s->points;
			hs = s;
		}
	}
	return hs;
}

// Returns the 3rd highest score for the player
Score *Player::ThirdScore(int since)
{
	Score *ss = SecondScore(since);
	Score *s, *hs = NULL;
	int third = 0;
	int second = 0;

	if(!ss) return NULL;

	second = ss->points;

	for(vector<Score *>::iterator it = scores.begin(); it != scores.end() && (s = *it); ++it) {
		if(s->points > third && s->points < second) {
			third = s->points;
			hs = s;
		}
	}
	return hs;
}

// Inserts a score into the scores vector
void Player::newScore(int points) 
{
	Score *s = new Score(points);
	scores.push_back(s);
}
