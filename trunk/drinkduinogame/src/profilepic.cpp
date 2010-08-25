/*
 * ProfilePic Class
 *
 */

#include "profilepic.h"

ProfilePic::ProfilePic()
{
	oldticks = SDL_GetTicks();
	frame = 0;
}

ProfilePic::~ProfilePic()
{
	pics.clear();
}

void ProfilePic::addPic(SDL_Surface *newpic)
{
	if(newpic)
		pics.push_back(newpic);
}

void ProfilePic::nextFrame()
{
	if(oldticks + PIC_FRAMERATE > SDL_GetTicks()) {
		oldticks = SDL_GetTicks();
		frame++;
		if(frame >= pics.size())
			frame = 0;
	}
}

SDL_Surface *ProfilePic::getPic()
{
	int cnt = pics.size();
	if(cnt == 0)
		return NULL;
	if(cnt == 1)
		return pics.front();
	nextFrame();
	return pics.at(frame);
}

SDL_Surface *ProfilePic::firstPic()
{
	if(pics.size() == 0)
		return NULL;
	return pics.front();
}
