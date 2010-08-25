/*
 * Scrollbar Class
 */

#include "scrollbar.h"

ScrollbarItem::ScrollbarItem(TTF_Font *font, Player *addPlayer)
{
	SDL_Color listColor = { 0, 0, 0, 0 };
	SDL_Surface *ppic = addPlayer->pic->getPic();
	double xscalefactor = (double)PIC_W / (double)ppic->w;
	double yscalefactor = (double)PIC_H / (double)ppic->h;

	player = addPlayer;
	pic = zoomSurface(ppic, xscalefactor, yscalefactor, SMOOTHING_ON);
	name = player->name;
	id = player->id;
	
	playerName = TTF_RenderText_Blended(font, name.c_str(), listColor);

	enabled = 0;
}

ScrollbarItem::~ScrollbarItem() 
{
	if(pic) SDL_FreeSurface(pic);
	if(playerName) SDL_FreeSurface(playerName);
}

/* ScrollBar */
Scrollbar::Scrollbar(Gui *parent, SDL_Surface *basescreen, int posx, int posy, int posw, int posh)
{
	graphics = parent;
	base = basescreen;
	scrollbox.x = posx;
	scrollbox.y = posy;
	scrollbox.w = posw;
	scrollbox.h = posh;
	upArrow.x = scrollbox.x + scrollbox.w + SCROLL_ARROW_BORDER;
	upArrow.y = scrollbox.y + 2;
	upArrow.w = SCROLL_ARROW_W;
	upArrow.h = SCROLL_ARROW_H;
	downArrow.x = upArrow.x;
	downArrow.y = scrollbox.y + scrollbox.h - SCROLL_ARROW_H - 2;
	downArrow.w = SCROLL_ARROW_W;
	downArrow.h = SCROLL_ARROW_H;

	index = 0;
	maxh = (int)(scrollbox.h / PIC_H);
}

Scrollbar::~Scrollbar()
{
	entries.clear();
}

// Adds a players pic/name to the scrollbar
void Scrollbar::add(Player *player)
{
	ScrollbarItem *entry;

	entry = new ScrollbarItem(graphics->getListFont(), player);
	entries.push_back(entry);
}

// Deletes a players entry
void Scrollbar::del(Player *player)
{
	ScrollbarItem *entry;
	vector<ScrollbarItem *>::iterator target;
	int found = 0;

	for(vector<ScrollbarItem *>::iterator it = entries.begin(); it != entries.end() && (entry = *it); ++it) {
		if(entry->id == player->id) {
			target = it;
			found = 1;
		}
	}
	if(found)
		entries.erase(target);
}

// Retrieves the current entries
vector<ScrollbarItem *>Scrollbar::getEntries()
{
	return entries;
}

// Redraws the contents of the scrollbar
void Scrollbar::redraw()
{
	ScrollbarItem *entry;
	SDL_Rect box, namebox;
	int c, pos = 0;
	
	namebox.x = 0;
	namebox.y = 0;
	SDL_BlitSurface(base, &scrollbox, graphics->screen, &scrollbox);
	for(c = index; c < index + maxh; c++) {
		if(c < entries.size()) {
			entry = entries.at(c);
			box.x = scrollbox.x+5;
			box.y = scrollbox.y+5;
			box.w = PIC_W;
			box.h = PIC_H;
			box.y += pos * PIC_H;
			SDL_BlitSurface(entry->pic, NULL, graphics->screen, &box);
			box.x += entry->pic->w + 8;
			box.w = scrollbox.w - entry->pic->w - 15;
			if(entry->enabled) 
				SDL_FillRect(graphics->screen, &box, SDL_MapRGB(graphics->screen->format, 0x74, 0x74, 0x74));
			box.y += (box.h / 2) - (entry->playerName->h / 2);
			box.h = entry->playerName->h;
			namebox.h = box.h;
			namebox.w = box.w;
			SDL_BlitSurface(entry->playerName, &namebox, graphics->screen, &box);
			pos++;
		}
	}
	SDL_UpdateRect(graphics->screen, scrollbox.x, scrollbox.y, scrollbox.w, scrollbox.h);
}

// Toggles the selected state of an entry
void Scrollbar::toggle(ScrollbarItem *entry)
{
	if(entry->enabled == 1)
		entry->enabled = 0;
	else
		entry->enabled = 1;
}

// Determine what was clicked on and toggles the entry
// Returns the index number or -1 if none
int Scrollbar::click(SDL_Event event)
{
	int clickx = event.motion.x;
	int clicky = event.motion.y;
	int itemnum = index;

	itemnum += (clicky - scrollbox.y) / PIC_H;
	
	if(itemnum < entries.size())
		toggle(entries.at(itemnum));
	else
		return -1;
	redraw();
	return itemnum;
}

// Scroll up/down a set amount of items
void Scrollbar::scroll(int dir, int amount)
{
	switch(dir) {
	case DIR_UP:
		if(index - amount >= 0)
			index -= amount;
		break;
	case DIR_DOWN:
		if(index + amount < entries.size())
			index += amount;
		break;
	default:
		cerr << "Unkown scroll direction: " << dir << endl;
		return;
	}
	redraw();
}
