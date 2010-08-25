/*
 * GUI Class
 *
 */

#include "gui.h"

Gui::Gui(GameData *engine)
{
  game = engine;
  verbose = 0;
  anonpic = NULL;
  NameFont = NULL;
  ScoreFont = NULL;
  screen = NULL;
  splashscreen = NULL;
  maindlg = NULL;
  maindlgon = NULL;
  statusbar = NULL;
  scoredlg = NULL;
  newscore = NULL;
  guyanim = NULL;
  configgear = NULL;
  abortbtn = NULL;
  currentGuyFrame = 0;
  guyRank = 0;
  state = STATE_SPLASH_SCREEN;
  gearState = 0;
  abortState = ABORT_UP;
  oldticks = 0;
  oldguyticks = 0;
  gearticks = 0;
  scoreidx = 0;
  fullscreen = 0;
  duiMode = 0;

  data_path = string(GAME_DATA_PATH);
  font_path = data_path + "/" + string(FONT_DIR);
  img_path = data_path + "/" + string(IMG_DIR);

  /* Box Regions */
  leftArrow.x = 20;
  leftArrow.y = 155;
  leftArrow.w = 90;
  leftArrow.h = 100;
  rightArrow.x = 700;
  rightArrow.y = 155;
  rightArrow.w = 90;
  rightArrow.h = 100;
  readyBtn.x = 299;
  readyBtn.y = 487;
  readyBtn.w = 182;
  readyBtn.h = 88;
  redLight.x = 511;
  redLight.y = 488;
  redLight.w = 57;
  redLight.h = 22;
  greenLight.x = 513;
  greenLight.y = 554;
  greenLight.w = 55;
  greenLight.h = 23;
  gearLoc.x = 760;
  gearLoc.y = 560;
  gearLoc.w = GEAR_FRAME_W;
  gearLoc.h = GEAR_FRAME_H;
  abortLoc.x = 720;
  abortLoc.y = 562;
  abortLoc.w = ABORT_FRAME_W;
  abortLoc.h = ABORT_FRAME_H;
  friendLoc.x = 680;
  friendLoc.y = 552;
  friendLoc.w = FRIEND_FRAME_W;
  friendLoc.h = FRIEND_FRAME_H;

  // Add Player Screen
  donebox.x = 600;
  donebox.y = 518;
  donebox.w = 142;
  donebox.h = 35;
  addArrow.x = 372;
  addArrow.y = 190;
  addArrow.w = 68;
  addArrow.h = 40;
  removeArrow.x = 372;
  removeArrow.y = 295;
  removeArrow.w = 142;
  removeArrow.h = 37;
  addNewbtn.x = 430;
  addNewbtn.y = 468;
  addNewbtn.w = 92;
  addNewbtn.h = 90;

  // Add New Players Screen
  addDonebox.x = 540;
  addDonebox.y = 535;
  addDonebox.w = 108;
  addDonebox.h = 44;
  addCancelbox.x = 240;
  addCancelbox.y = 535;
  addCancelbox.w = 108;
  addCancelbox.h = 44;
  addSmilebtn.x = 375;
  addSmilebtn.y = 445;
  addSmilebtn.w = 138;
  addSmilebtn.h = 132;
  addNamebox.x = 238;
  addNamebox.y = 364;
  addNamebox.w = 413;
  addNamebox.h = 51;
}

Gui::~Gui()
{
   if(screen) SDL_FreeSurface(screen);
   if(splashscreen) SDL_FreeSurface(splashscreen);
   if(anonpic) SDL_FreeSurface(anonpic);
   if(maindlg) SDL_FreeSurface(maindlg);
   if(maindlgon) SDL_FreeSurface(maindlgon);
   if(statusbar) SDL_FreeSurface(statusbar);
   if(scoredlg) SDL_FreeSurface(scoredlg);
   if(newscore) SDL_FreeSurface(newscore);
   if(guyanim) SDL_FreeSurface(guyanim);
   if(configgear) SDL_FreeSurface(configgear);
   if(abortbtn) SDL_FreeSurface(abortbtn);
}

// Sets directory to keep user files
void Gui::setRCdir(string rcdir)
{
  struct stat st;
  rcpath = rcpath;
  pic_path = rcpath + "/" + string(PIC_DIR);
  if(stat(pic_path.c_str(), &st) != 0)
	mkdir(pic_path.c_str(), 0755);
}

// Creates a blank SDL surface
SDL_Surface *Gui::createBlankSurface(int width, int height)
{
    // We acquire the settings in our return surface
    SDL_Surface *basis = SDL_GetVideoSurface();

    // create the new surface using the Right-To_Left principle
    basis = SDL_CreateRGBSurface ( basis->flags, width, height,
                                     basis->format->BitsPerPixel,
                                     basis->format->Rmask,
                                     basis->format->Gmask,
                                     basis->format->Bmask,
                                     basis->format->Amask);
    // Make the surface transparent
    SDL_FillRect(basis, NULL, SDL_MapRGBA(basis->format, 0, 0, 0, 0));
    return basis;
}

// Initialize all SDL components
int Gui::initGraphics()
{
   if(SDL_Init(SDL_INIT_VIDEO)<0) {
	cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
	SDL_Quit();
	return 0;
   } else if (TTF_Init() == -1) {
	cerr << "Unable to initialize TTF: " << TTF_GetError() << endl;
	SDL_Quit();
	return 0;
   }
   SDL_EnableUNICODE(true);
   return 1;
}

// Load fonts
void Gui::loadFonts()
{
	string fontfile = font_path + "/Mostwasted.ttf";
	NameFont = TTF_OpenFont(fontfile.c_str(), 99);
	if(!NameFont) {
		cerr << "Unable to load name font: " << fontfile << endl;
		SDL_Quit();
		return;
	}
	fontfile = font_path + "/defused.ttf";
	ScoreFont = TTF_OpenFont(fontfile.c_str(), 39);
	if(!ScoreFont) {
		cerr << "Unable to load name font: " << fontfile << endl;
		SDL_Quit();
		return;
	}
	fontfile = font_path + "/bluebold.ttf";
	AnyFont = TTF_OpenFont(fontfile.c_str(), 24);
	if(!AnyFont) {
		cerr << "Unable to load name font: " << fontfile << endl;
		SDL_Quit();
		return;
	}
	fontfile = font_path + "/Harabara.ttf";
	NumberFont = TTF_OpenFont(fontfile.c_str(), 18);
	if(!AnyFont) {
		cerr << "Unable to load name font: " << fontfile << endl;
		SDL_Quit();
		return;
	}
	// Scrollbar font
	ListFont = TTF_OpenFont(fontfile.c_str(), 18);
	NewNameFont = TTF_OpenFont(fontfile.c_str(), 38);
}

// Preloads all images
void Gui::initImages()
{
   scrap = createBlankSurface(VID_W, VID_H);
   anonpic = loadImage(string(img_path + "/" + ANONPIC).c_str());
   maindlg = loadImage(string(img_path + "/bg1.png").c_str());
   maindlgon = loadImage(string(img_path + "/bg1-on.png").c_str());
   scoredlg = loadImage(string(img_path + "/scoredlg.png").c_str());
   newscore = loadImage(string(img_path + "/scoredlg.png").c_str());
   configgear = loadImage(string(img_path + "/gears.png").c_str());
   abortbtn = loadImage(string(img_path + "/abortbtn.png").c_str());
   friendbtn = loadImage(string(img_path + "/friends-icon-sm.png").c_str());
   guyanim = loadImage(string(img_path + "/guy.png").c_str());
   playerscreen = loadImage(string(img_path + "/addPlayer1.png").c_str());
   addnewscreen = loadImage(string(img_path + "/addUser1.png").c_str());
}

// Initializes the video mode and sets fullscreen if enabled
void Gui::initVideoMode()
{
   string caption = "DrinkDuino v" + string(PACKAGE_VERSION);
   if(fullscreen) {
	screen = SDL_SetVideoMode(VID_W, VID_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
   } else {
	screen = SDL_SetVideoMode(VID_W, VID_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption(caption.c_str(), "DrinkDuino");
   }
}

SDL_Surface *Gui::loadImage(const char *path)
{
	SDL_Surface *pic = NULL;
	struct stat st;
	if(stat(path, &st) == 0) {
		pic = IMG_Load(path);
		if(!pic)
		  cerr << "Couldn't load image " << path << endl;
	} else if(verbose)
		cerr << "Couldn't find image " << path << endl;
	return pic;	
}

// Retrieves the default anon picture to use. 
SDL_Surface *Gui::getAnonPic()
{
	return anonpic;
}

// Returns the list font.  Used by scrollbar class
TTF_Font *Gui::getListFont()
{
	return ListFont;
}

// Initial splash screen
// Initializes and loads/caches all images,fonts,etc
int Gui::splashScreen()
{
	string splashimg = img_path + "/splashscreen.jpg";

	if(!initGraphics())
		return 0;

	loadFonts();

	initVideoMode();
	
	splashscreen = IMG_Load(splashimg.c_str());
	if(!splashscreen) {
		cerr << "Could not load splash image: " << splashimg << endl;
		SDL_Quit();
		return 0;
	}

	SDL_BlitSurface(splashscreen, NULL, screen, NULL);
	SDL_Flip(screen);

	initImages();
}

// Updates the splash screen count of DrinkShields
void Gui::splashShields(int count)
{
	char buf[10];
	SDL_Rect box;
	SDL_Color countColor = { 0x7f, 0x7f, 0x7f, 0 };
	SDL_Surface *countText;

	snprintf(buf, 10, "%d", count); 
	countText = TTF_RenderText_Blended(NumberFont, buf, countColor);

	box.x = 232;
	box.y = 474;
	box.w = countText->w;
	box.h = countText->h;

	SDL_BlitSurface(splashscreen, &box, screen, &box);
	SDL_UpdateRect(screen, box.x, box.y, box.w, box.h);

	SDL_BlitSurface(countText, NULL, screen, &box);
	SDL_UpdateRect(screen, box.x, box.y, box.w, box.h);
}

// Updates the splash screen count of Friends
void Gui::splashFriends(int count)
{
	char buf[10];
	SDL_Rect box;
	SDL_Color peopleColor = { 0x7f, 0x7f, 0x7f, 0 };
	SDL_Surface *peopleText;

	snprintf(buf, 10, "%d", count); 
	peopleText = TTF_RenderText_Blended(NumberFont, buf, peopleColor);

	box.x = 549;
	box.y = 474;
	box.w = peopleText->w;
	box.h = peopleText->h;

	SDL_BlitSurface(splashscreen, &box, screen, &box);
	SDL_UpdateRect(screen, box.x, box.y, box.w, box.h);

	SDL_BlitSurface(peopleText, NULL, screen, &box);
	SDL_UpdateRect(screen, box.x, box.y, box.w, box.h);
}

// Draws the configuration gear
void Gui::drawGear()
{
	SDL_Rect gear;

	gear.x = gearState * GEAR_FRAME_W;
	gear.y = 0;
	gear.w = GEAR_FRAME_W;
	gear.h = GEAR_FRAME_H;

	// Erase Gear
   	SDL_BlitSurface(maindlg, &gearLoc, screen, &gearLoc);
   	// Update Gear
   	SDL_BlitSurface(configgear, &gear, screen, &gearLoc);
   	SDL_UpdateRect(screen, gearLoc.x, gearLoc.y, gearLoc.w, gearLoc.h);
}

// Draws the Abort play button
void Gui::drawAbort()
{
	SDL_Rect abort;

	abort.x = abortState * ABORT_FRAME_W;
	abort.y = 0;
	abort.w = abortLoc.w;
	abort.h = abortLoc.h;

	SDL_BlitSurface(abortbtn, &abort, screen, &abortLoc);
	SDL_UpdateRect(screen, abortLoc.x, abortLoc.y, abortLoc.w, abortLoc.h);
}

// Draw add friend button
void Gui::drawFriendIcon()
{
	SDL_Rect friendbox;

	if(isover(friendLoc))
		friendbox.x = FRIEND_FRAME_W;
	else
		friendbox.x = 0;
	friendbox.y = 0;
	friendbox.w = FRIEND_FRAME_W;
	friendbox.h = FRIEND_FRAME_H;

	SDL_BlitSurface(maindlg, &friendLoc, screen, &friendLoc);
	SDL_BlitSurface(friendbtn, &friendbox, screen, &friendLoc);
	SDL_UpdateRect(screen, friendLoc.x, friendLoc.y, friendLoc.w, friendLoc.h);
}

// Sets dancing guys ranking
void Gui::setGuyRank(int r)
{
	guyRank = r;
}

// Recieved a new score.  Update score dialog and change state
void Gui::newScore(int score)
{
	char buf[25];
	SDL_Color scorecolor = { 0x33, 0xcf, 0x30, 0xff };
	SDL_Surface *scoretxt;
	SDL_Rect loc;

	SDL_BlitSurface(scoredlg, NULL, newscore, NULL);
	scoredst.x = (screen->w / 2) - (newscore->w / 2);
        scoredst.y = (screen->h / 2) - (newscore->h / 2);
        scorenow.x = scoredst.x;
        scorenow.y = screen->h;
        scorenow.w = newscore->w;
        scorenow.h = newscore->h;
	if(duiMode)
		snprintf(buf, 20, "%f", game->getPPM(score));
	else
		snprintf(buf, 20, "%d", score);
        scoretxt = TTF_RenderText_Blended(ScoreFont, buf, scorecolor);
        loc.x = 186;
        loc.y = (newscore->h / 2) - (scoretxt->h / 2);
        loc.w = scoretxt->w;
        loc.h = scoretxt->h;
        SDL_BlitSurface(scoretxt, NULL, newscore, &loc);
        SDL_BlitSurface(screen, NULL, scrap, NULL);
        state = STATE_SCORE_UP;
}

// Redraws the screen
void Gui::redraw()
{
   switch(state) {
   case STATE_ADD_NEW_SCREEN:
	SDL_BlitSurface(addnewscreen, NULL, screen, NULL);
	updateAddNewNamebox();
	SDL_Flip(screen);
	break;
   case STATE_ADD_PLAYERS_SCREEN:
	SDL_BlitSurface(playerscreen, NULL, screen, NULL);
	if(game->allPlayersBox) game->allPlayersBox->redraw();
	if(game->addPlayersBox) game->addPlayersBox->redraw();
	SDL_Flip(screen);
	break;
   case STATE_MAIN_SCREEN:
   case STATE_SCORE_UP:
   case STATE_SCORE_DOWN:
   case STATE_SCORE_HOVER:
	SDL_BlitSurface(maindlg, NULL, screen, NULL);
	drawGear();
	drawAbort();
	drawFriendIcon();
	drawPlayerInfo();
	SDL_Flip(screen);
	break;
   default:
	cerr << "redraw() unknown state: " << state << endl;
   }
}

// Update clock ticks
void Gui::updateTicks()
{
	oldticks = SDL_GetTicks();
	oldguyticks = SDL_GetTicks();
	gearticks = SDL_GetTicks();
}

// Update any animations on the screen
void Gui::updateAnimations()
{
   SDL_Rect guyloc, animloc;

   if(gearState > 0) {
        if(gearticks + GEAR_FRAMERATE > SDL_GetTicks()) {
                // Do Nothing
        } else {
                gearticks = SDL_GetTicks();
                gearState++;
                if(gearState > 3) gearState = 1;
                drawGear();
        }
   }

   if(state == STATE_SCORE_UP) {
        if(oldticks + SCORE_FRAMERATE > SDL_GetTicks()) {
                // Do Nothing
        } else {
                oldticks = SDL_GetTicks();
                // Erase old box
                SDL_BlitSurface(scrap, &scorenow, screen, &scorenow);
                scorenow.y-=3;
                if(scorenow.y <= scoredst.y)
                        state = STATE_SCORE_HOVER;
        }
  } else if (state == STATE_SCORE_HOVER) {
        if(oldticks + HOVER_FRAMERATE > SDL_GetTicks()) {
                // Do Nothing
        } else {
                oldticks = SDL_GetTicks();
                state = STATE_SCORE_DOWN;
        }
  }else if (state == STATE_SCORE_DOWN) {
        if(oldticks + SCORE_FRAMERATE > SDL_GetTicks()) {
                // Do Nothing
        } else {
                oldticks = SDL_GetTicks();
                SDL_BlitSurface(scrap, &scorenow, screen, &scorenow);
                scorenow.y+=3;
                if(scorenow.y >= screen->h) {
                        state = STATE_MAIN_SCREEN;
                        redraw();
                }
        }

  }

  // Score Guy Animiations
  if(state == STATE_SCORE_UP || state == STATE_SCORE_DOWN || state == STATE_SCORE_HOVER) {
        if(oldguyticks + GUY_FRAMERATE > SDL_GetTicks()) {
                // Do Nothing
        } else {
                oldguyticks = SDL_GetTicks();
                guyloc.x = scorenow.x+40;
                guyloc.y = scorenow.y+25;
                guyloc.w = GUY_FRAME_W;
                guyloc.h = GUY_FRAME_H;
                animloc.w = GUY_FRAME_W;
                animloc.h = GUY_FRAME_H;
                animloc.x = GUY_FRAME_W * currentGuyFrame;
                animloc.y = GUY_FRAME_H * guyRank;
                // Erase last guy pic
                SDL_BlitSurface(newscore, NULL, screen, &scorenow);
                //SDL_BlitSurface(scoredlg, &guyloc, screen, &guyloc);
                SDL_BlitSurface(guyanim, &animloc, screen, &guyloc);
                SDL_Flip(screen);

                currentGuyFrame++;
                if(currentGuyFrame >= GUY_FRAMES)
                        currentGuyFrame = 0;
        }
  }

}

// Toggle a light from the main dialog screen
void Gui::light(SDL_Rect b, int state)
{
  if(state == ON) {
   SDL_BlitSurface(maindlgon, &b, screen, &b);
  } else {
    SDL_BlitSurface(maindlg, &b, screen, &b);
  }
  SDL_UpdateRect(screen, b.x, b.y, b.w, b.h);
}

// Toggles the player lights
void Gui::togglePlayerLights(int state)
{
    if(state == ON) {
	light(greenLight, ON);
	light(redLight, OFF);
    } else {
	light(greenLight, OFF);
	light(redLight, ON);
    }
}

// Returns true if the current event is over a rectangle
bool Gui::isover(SDL_Rect b)
{
    if(event.motion.x > b.x && event.motion.x < b.x + b.w &&
       event.motion.y > b.y && event.motion.y < b.y + b.h)
      return 1;
    return 0;
}

// Lightup items when mouse over them
void Gui::mouseover()
{
  switch(state) {
  case STATE_ADD_PLAYERS_SCREEN:
	break;
  case STATE_MAIN_SCREEN:
	  if(isover(leftArrow)) {
	    light(leftArrow, ON);
	  } else if (isover(rightArrow)) {
	    light(rightArrow, ON);
	  } else if (isover(gearLoc)) {
	    gearState++;
	    if(gearState > 3) gearState = 1;
	    drawGear();
	  } else if (isover(abortLoc)) {
	    if(abortState != ABORT_DOWN) {
	            abortState = ABORT_OVER;
	            drawAbort();
	    }
	  } else { // Over nothing
	    light(leftArrow, OFF);
	    light(rightArrow, OFF);
	    gearState = 0;
	    drawGear();
	    drawFriendIcon();
	    if(abortState != ABORT_DOWN) {
	            abortState = ABORT_UP;
	            drawAbort();
	    }
	  }
	break;
   }
}

// Draws the profile bar
void Gui::updateProfileBar()
{
  Player *p;
  SDL_Rect profilebar, status;
  SDL_Surface *pic;

  if(game->players.size() == 0) return;

  if(game->activePlayer->pic->getPic() == NULL) {
	cerr << game->activePlayer->name << " does not have a pic loaded" << endl;
	return;
   }

  profilebar.x = 118;
  profilebar.y = 151;
  profilebar.w = 580;
  profilebar.h = 124;

  SDL_BlitSurface(maindlg, &profilebar, screen, &profilebar);

  pic = game->activePlayer->pic->getPic();
  mainpic.x = screen->w / 2 - pic->w / 2;
  mainpic.y = profilebar.y + 2;
  mainpic.w = pic->w;
  mainpic.h = pic->h;
  SDL_BlitSurface(pic, NULL, screen, &mainpic);

  p = game->activePlayerLeft1();
  if(p) {
	pic = p->pic->getPic();
	l1pic.x = mainpic.x - pic->w - 10;
	l1pic.y = mainpic.y;
	l1pic.w = pic->w;
	l1pic.h = pic->h;
	SDL_BlitSurface(pic, NULL, screen, &l1pic);
	p = game->activePlayerLeft2();
	if(p) {
		pic = p->pic->getPic();
		l2pic.x = l1pic.x - pic->w - 10;
		l2pic.y = mainpic.y;
		l2pic.w = pic->w;
		l2pic.h = pic->h;
		SDL_BlitSurface(pic, NULL, screen, &l2pic);
	}
  }

  p = game->activePlayerRight1();
  if(p) {
	pic = p->pic->getPic();
	r1pic.x = mainpic.x + pic->w + 10;
	r1pic.y = mainpic.y;
	r1pic.w = pic->w;
	r2pic.h = pic->h;
	SDL_BlitSurface(pic, NULL, screen, &r1pic);
	p = game->activePlayerRight2();
	if(p) {
		pic = p->pic->getPic();
		r2pic.x = r1pic.x + pic->w + 10;
		r2pic.y = mainpic.y;
		r2pic.w = pic->w;
		r2pic.h = pic->h;
		SDL_BlitSurface(pic, NULL, screen, &r2pic);
	}
  }

  // Gradiant
  status.x = 0;
  status.y = 0;
  status.w = profilebar.w;
  status.h = profilebar.h;
  SDL_BlitSurface(statusbar, &status, screen, &profilebar);
  // Redraw Arrows re-using the status rectangle
  status.y = profilebar.y;
  status.w = profilebar.x;
  status.h = profilebar.h;
  SDL_BlitSurface(maindlg, &status, screen, &status);
  status.x = profilebar.x + profilebar.w;
  status.w = screen->w - status.x;
  SDL_BlitSurface(maindlg, &status, screen, &status);

  SDL_Flip(screen);
}

// Updates the players statusbar and scores and any other info
void Gui::drawPlayerInfo()
{
    SDL_Color NameColor = { 201, 17, 17, 0 };
    SDL_Color ScoreColor = { 128, 127, 127, 0 };
    SDL_Surface *NameText, *ScoreText;
    SDL_Rect namebox, titlearea, scorearea, scorebox;
    Score *s;
    char scoremsg[30];

    titlearea.x = 0;
    titlearea.y = 0;
    titlearea.w = screen->w;
    titlearea.h = 130;

    if(!game->activePlayer)
	return;

    NameText = TTF_RenderText_Blended(NameFont, game->activePlayer->name.c_str(), NameColor);

    namebox.x = screen->w / 2 - NameText->w / 2;
    namebox.y = 21;
    namebox.w = NameText->w;
    namebox.h = NameText->h;

    SDL_BlitSurface(maindlg, &titlearea, screen, &titlearea);
    SDL_UpdateRect(screen, namebox.x, namebox.y, namebox.w, namebox.h);

    SDL_BlitSurface(NameText, NULL, screen, &namebox);
    SDL_UpdateRect(screen, namebox.x, namebox.y, namebox.w, namebox.h);

    // Score
    scorearea.x = 166;
    scorearea.y = 305;
    scorearea.w = 500;
    scorearea.h = 180;

    SDL_BlitSurface(maindlg, &scorearea, screen, &scorearea);
    SDL_UpdateRect(screen, scorearea.x, scorearea.y, scorearea.w, scorearea.h);

    // Top 3 scores
    if(game->activePlayer->scores.size() == 0) {
	ScoreText = TTF_RenderText_Blended(ScoreFont, DRINK_MSG, ScoreColor);
	scorebox.x = screen->w / 2 - ScoreText->w / 2;
	scorebox.y = scorearea.y + 40;
	scorebox.w = ScoreText->w;
	scorebox.h = ScoreText->h;
	SDL_BlitSurface(ScoreText, NULL, screen, &scorebox);
	SDL_UpdateRect(screen, scorebox.x, scorebox.y, scorebox.w, scorebox.h);
    } else {
	// Display the top 3 scores
	s = game->activePlayer->FirstScore(0);
	if(s) {
		if(duiMode)
			snprintf(scoremsg, 30, "1st %f", s->getPPM());
		else
			snprintf(scoremsg, 30, "1st %d", s->points);
		ScoreText = TTF_RenderText_Blended(ScoreFont, scoremsg, ScoreColor);
		scorebox.x = screen->w / 2 - ScoreText->w /2;
		scorebox.y = scorearea.y + 10;
		scorebox.w = ScoreText->w;
		scorebox.h = ScoreText->h;
		SDL_BlitSurface(ScoreText, NULL, screen, &scorebox);
		SDL_UpdateRect(screen, scorebox.x, scorebox.y, scorebox.w, scorebox.h);

	}
	s = game->activePlayer->SecondScore(0);
	if(s) {
		if(duiMode)
			snprintf(scoremsg, 30, "2nd %f", s->getPPM());
		else
			snprintf(scoremsg, 30, "2nd %d", s->points);
		ScoreText = TTF_RenderText_Blended(ScoreFont, scoremsg, ScoreColor);
		scorebox.x = screen->w / 2 - ScoreText->w /2;
		scorebox.y = scorearea.y + 50;
		scorebox.w = ScoreText->w;
		scorebox.h = ScoreText->h;
		SDL_BlitSurface(ScoreText, NULL, screen, &scorebox);
		SDL_UpdateRect(screen, scorebox.x, scorebox.y, scorebox.w, scorebox.h);

	}
	s = game->activePlayer->ThirdScore(0);
	if(s) {
		if(duiMode)
			snprintf(scoremsg, 30, "3rd %f", s->getPPM());
		else
			snprintf(scoremsg, 30, "3rd %d", s->points);
		ScoreText = TTF_RenderText_Blended(ScoreFont, scoremsg, ScoreColor);
		scorebox.x = screen->w / 2 - ScoreText->w /2;
		scorebox.y = scorearea.y + 90;
		scorebox.w = ScoreText->w;
		scorebox.h = ScoreText->h;
		SDL_BlitSurface(ScoreText, NULL, screen, &scorebox);
		SDL_UpdateRect(screen, scorebox.x, scorebox.y, scorebox.w, scorebox.h);
	}
    }
    updateProfileBar();
}

// Update add new player name box
void Gui::updateAddNewNamebox()
{
   SDL_Color NewNameColor = { 0, 0x4a, 0, 0 };
   SDL_Surface *newNameText;
   SDL_Rect b, maxText;

   b.x = addNamebox.x + 8;
   b.y = addNamebox.y + 8;
   b.w = addNamebox.w - 16;
   b.h = addNamebox.h - 8;
   maxText.x = 0;
   maxText.y = 0;
   maxText.h = b.h;
   maxText.w = b.w;

   newNameText = TTF_RenderText_Blended(NewNameFont, addNewName.c_str(), NewNameColor);
   SDL_BlitSurface(addnewscreen, &addNamebox, screen, &addNamebox);
   SDL_BlitSurface(newNameText, &maxText, screen, &b);
   SDL_UpdateRect(screen, addNamebox.x, addNamebox.y, addNamebox.w, addNamebox.h);
}

// Poll for any key to be pressed
void Gui::anyKey()
{
   int running = 1;
   SDL_Rect anybox;
   SDL_Surface *anytext;
   SDL_Color anycolor = { 0x4a, 0xba, 0x0d, 0 };

   anytext = TTF_RenderText_Blended(AnyFont, "INITIALIZED.  Press Any Key", anycolor);
   anybox.x = screen->w / 2 - anytext->w / 2;
   anybox.y = 536;
   anybox.w = anytext->w;
   anybox.h = anytext->h;

   SDL_BlitSurface(anytext, NULL, screen, &anybox);
   SDL_UpdateRect(screen, anybox.x, anybox.y, anybox.w, anybox.h);
	
   while(running) {
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_MOUSEBUTTONDOWN:
		case SDL_KEYDOWN:
			running = 0;
			break;
		}
	}
   } 
}

// Get events while in the Add New Players screen
int Gui::addNewEvents()
{
   int result = 0;
   int key;
   char byte[1];
   byte[1] = 0;
   while(SDL_PollEvent(&event)) {
	switch(event.type) {
	case SDL_MOUSEMOTION:
		mouseover();
		break;
        case SDL_MOUSEBUTTONDOWN:
                if(isover(addCancelbox)) {
			result = -2;
		} else if(isover(addDonebox)) {
			if(addNewName.size() > 0)
				result = -1;
		} else if(isover(addNamebox)) {
			addNewName.clear();
			updateAddNewNamebox();
		} else if(isover(addSmilebtn)) {
			cout << "TODO: Implement smile button" << endl;
		}
		break;
	case SDL_KEYDOWN:
		key = event.key.keysym.unicode;
		byte[0] = (char)key;
		if(key >= SDLK_SPACE && key <= 176) {
			if(addNewName.size() < MAX_PLAYER_NAME_LEN) {
				addNewName += string(byte);
				updateAddNewNamebox();
			}
		}
		if(key == SDLK_BACKSPACE) {
			if(addNewName.size() > 0) {
				addNewName.erase(addNewName.end()-1);
				updateAddNewNamebox();
			}
		}
		break;
	}
   }
   return result;
}

// Get events while in the Add Players screen
int Gui::addPlayerEvents()
{
   int result = 0;
   while(SDL_PollEvent(&event)) {
	switch(event.type) {
	case SDL_MOUSEMOTION:
		mouseover();
		break;
        case SDL_MOUSEBUTTONDOWN:
                if(isover(donebox)) {
			// Must have at least 1 active player
			if(game->addPlayersBox)
				if(game->addPlayersBox->getEntries().size() > 0)
					result = -1;
		} else if(isover(game->allPlayersBox->scrollbox)) {
			game->allPlayersBox->click(event);
		} else if(isover(game->addPlayersBox->scrollbox)) {
			game->addPlayersBox->click(event);
		} else if(isover(addArrow)) {
			game->moveAlltoAddBox();
		} else if(isover(removeArrow)) {
			game->moveAddtoAllBox();
		} else if(isover(game->allPlayersBox->upArrow)) {
			game->allPlayersBox->scroll(DIR_UP, 1);
		} else if(isover(game->allPlayersBox->downArrow)) {
			game->allPlayersBox->scroll(DIR_DOWN, 1);
		} else if(isover(game->addPlayersBox->upArrow)) {
			game->addPlayersBox->scroll(DIR_UP, 1);
		} else if(isover(game->addPlayersBox->downArrow)) {
			game->addPlayersBox->scroll(DIR_DOWN, 1);
		} else if(isover(addNewbtn)) {
			return -2;
		}
		break;
	}
   }
   return result;
}

// Poll keyboard and mouse events
// Return 1 to quit
int Gui::pollEvents()
{
   int running = 1;
   while(SDL_PollEvent(&event)) {
	switch(event.type) {
	case SDL_VIDEORESIZE:
	case SDL_VIDEOEXPOSE:
		redraw();
		break;
	case SDL_MOUSEMOTION:
		mouseover();
		break;
	case SDL_MOUSEBUTTONUP:
		if(abortState == ABORT_DOWN) 
			abortState = ABORT_UP;
		break;
	case SDL_MOUSEBUTTONDOWN:
                    if(isover(leftArrow)) {
                      game->prevPlayer();
                      drawPlayerInfo();
                    } else if(isover(rightArrow)) {
                      game->nextPlayer();
                      drawPlayerInfo();
                    } else if(isover(readyBtn)) {
                      game->recordResults();
                    } else if(isover(l1pic)) {
                      game->prevPlayer();
                      drawPlayerInfo();
                    } else if(isover(l2pic)) {
                      game->prevPlayer();
                      game->prevPlayer();
                      drawPlayerInfo();
                    } else if(isover(r1pic)) {
                      game->nextPlayer();
                      drawPlayerInfo();
                    } else if(isover(r2pic)) {
                      game->nextPlayer();
                      game->nextPlayer();
                      drawPlayerInfo();
                    } else if(isover(gearLoc)) {
                      game->recalibrateShields();
                    } else if(isover(abortLoc)) {
                      game->abortPlay();
                    } else if(isover(friendLoc)) {
		      game->switchState(GAME_STATE_ADD_PLAYERS);
		    }
                    break;
	case SDL_KEYDOWN:
		switch(event.key.keysym.sym) {
	                  case SDLK_LEFT:
        	            game->prevPlayer();
                	    drawPlayerInfo();
	                    break;
        	          case SDLK_RIGHT:
                	    game->nextPlayer();
	                    drawPlayerInfo();
        	            break;
	                  case SDLK_RETURN:
	                    game->recordResults();
        	            break;
                	  case SDLK_r:
				game->refreshPlayerData();
        	                redraw();
	                    	break;
	                  case SDLK_q:
       		          case SDLK_ESCAPE:
				running = 0;
				break;
			}
		break;
	case SDL_QUIT:
		running = 0;
		break;
	}
   }
   return running;
}
