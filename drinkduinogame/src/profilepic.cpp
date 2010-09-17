/*
 * ProfilePic Class
 *
 */

#include "profilepic.h"

ProfilePic::ProfilePic()
{
	frame = 0;
}

ProfilePic::~ProfilePic()
{
	SDL_Surface *pp;

	for(vector<SDL_Surface *>::iterator it = pics.begin(); it != pics.end() && (pp = *it); ++it)
		SDL_FreeSurface(pp);
	pics.clear();
}

void ProfilePic::addPic(SDL_Surface *newpic)
{
	SDL_Surface *savepic;
	if(newpic) {
		savepic = SDL_CreateRGBSurface( newpic->flags, newpic->w, newpic->h,
			newpic->format->BitsPerPixel,
			newpic->format->Rmask,
			newpic->format->Gmask,
			newpic->format->Bmask,
			newpic->format->Amask);
		SDL_BlitSurface(newpic, NULL, savepic, NULL);

		pics.push_back(savepic);
	}
}

void ProfilePic::nextFrame()
{
	frame++;
	if(frame >= pics.size())
		frame = 0;
}

SDL_Surface *ProfilePic::getPic()
{
	if(pics.size() == 0)
		return NULL;
	return pics.at(frame);
}

SDL_Surface *ProfilePic::firstPic()
{
	if(pics.size() == 0)
		return NULL;
	return pics.front();
}

SDL_Surface *ProfilePic::nextPic()
{
	nextFrame();
	return pics.at(frame);
}
