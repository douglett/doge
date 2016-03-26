#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "libsrc/xcengine.h"
#include "globals.h"

using namespace std;


static int loopt();


// main.cpp globals
vector<string> gmap, fogofwar;
vector<pair<int, string> > combat_log;
vector<mob> gmobs, effects;
vector<gtext> gtexts;



int main() {
	cout << "hello world" << endl;
	if (game::init())
		return 1;

	// game init work
	display::sprites = texture::get("images")->tex;
	display::crownsprite = texture::get("crown")->tex;
	display::camera.w = ceil(game::width/12.0);
	display::camera.h = ceil(game::height/12.0);

	// start state
	titlemenu::reset();
	loopt();

	game::quit();
}


// helper - manage global stringstream
stringstream& ss(int reset) {
	static stringstream strm;
	if (reset)
		strm.str(""), strm.clear();
	return strm;
}


// main loop
static int loopt() {
	using namespace scene;

	while (true) {
		// cls
		SDL_SetRenderDrawColor(game::ren, 0, 0, 0, 255);
		SDL_RenderClear(game::ren);
		// draw
		for (int mscene : scene::scenestack)
			switch (mscene) {
			 case TITLEMENU:
				display::draw_titlemenu();
				break;
			 case FADEBLACK:
			 	fadeblack::draw();
				break;
			 case GAME:
			 	display::draw_gamescene();
			 	display::draw_spellmenu();
			 	break;
			 case FADEWHITE:
			 	fadewhite::draw();
			 	break;
			}
		// display
		display::flip();

		// handle input
		int rval = 0;
		string k = keys::getkey();
		if (k == "^q")
			break;
		switch (scene::current()) {
		 case TITLEMENU:
		 	rval = titlemenu::step(k);
			break;
		 case FADEBLACK:
		 	rval = fadeblack::step(k);
			break;
		 case GAME:
		 	rval = action::taketurn(k);
		 	break;
		 case SPELLMENU:
			rval = spellmenu::action(k);
			break;
		 case FADEWHITE:
		 	rval = fadewhite::step(k);
		 	break;
		}
		// handle return values
		// ...
	}

	return 0;
}


// change an 8 character word to an integer seed
int stringtoseed(string seedstr) {
	// sanitize seed a-z only
	string seed;
	for (auto c : seedstr) {
		c = tolower(c);
		if (seed.length() >= 8)
			;
		else if (c >= 'a' && c <= 'z')
			seed += c;
		else if (c >= '0' && c <= '9')
			seed += c;
	}
	// make prefix and suffix
	int s1 = 0;
	int s2 = 0;
	for (int i = 0; i < seed.length(); i++) {
		char c = seed[i];
		if (i < 3)
			s1 += c - 'a' + 1;
		else
			s2 += c - 'a' + 1;
	}
	int s = s1*100 + s2;  // make seed number
	printf("seed: %s  %d\n", seed.c_str(), s);  // debug
	return s;
}
