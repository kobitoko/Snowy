#include "surfing.h"
#include <string>
#include <sstream>

// SDL2 needs main() to be main(int argc, char * argv[]) and not the simple main()
// https://wiki.libsdl.org/FAQWindows#I_get_.22Undefined_reference_to_.27SDL_main.27.22_...
int main(int argc, char * argv[]) {

	std::cout << "Hi there!" << std::endl;

	// writes an error log, if 2nd parameter is false it will not crash the game.
	callError("Hello World!", false);

	Uint32 gameTime = SDL_GetTicks();
	// the core game updates will be capped at 60fps
	Uint32 msPerFrame = 1000/60;

	// Init
	int x = SDL_WINDOWPOS_CENTERED;
	int y = SDL_WINDOWPOS_CENTERED;
	w = 800;
	h = 600;

    scr = Screen();
    in = Input();
    snd = Sound();
    phy = Physics();

	scr.makeWindow(x,y,w,h,"Surf!");
	//SDL_SetWindowBordered(window.getWindow(),SDL_FALSE);
	std::string winName = "Surf!";

	// load the test sprites
	loadMySprites();

	// load sounds n music
	loadMySounds();

	// load the physics
	b2Body* theBody = loadMyPhysics();
	loadMyParticles();
	bool running = true;

	// main loop http://gameprogrammingpatterns.com/game-loop.html
	while(running) {

		if(SDL_TICKS_PASSED(SDL_GetTicks(), gameTime)) {
			//std::cout << "FPS:" << scr.getFps() << "\r";
			//MinGW has an issue with std::to_string -> http://stackoverflow.com/questions/22571838/gcc-4-8-1-stdto-string-error
			//winName = "Surf! - FPS:" + std::to_string(scr.getFps());
			//std::ostringstream winNameFps;
			//winNameFps << "FPS:" << scr.getFps();
			//std::string winName = "Surf!" + winNameFps.str();
			//SDL_SetWindowTitle(scr.getWindow(), winName.c_str());

			//Later just perhaps make events seperately on which Input class works on????
			// or custom events class to handle user events.
			in.updateInput();

			// update physics, 2 times to prevent slowmotion water but requires more computation.
			phy.update();
			phy.update();

			// physics and particle testing
			testPhysics(theBody);
			testParticles();

			// Test input
			running = testInput();

            // update screen.
            scr.update();

			gameTime = SDL_GetTicks() + msPerFrame;
		}
	}

	SDL_Quit();
	return 0;
}

// Tests' Function definitions --------------------------------------------------

bool testInput() {
	// Test keyboard keys
	if(in.keyStatus(SDLK_q)) {
		std::cout << SDL_GetKeyName(SDLK_q) << std::endl << "Bye" << std::endl;
		//running = false;
		return false;
	}

	// Test window's buttons
	if(in.windowStatus(SDL_WINDOWEVENT_CLOSE)) {
		std::cout << "Window Closed." << std::endl;
		//running = false;
		return false;
	}
	return true;
}

void loadMySprites() {
	// the background image
	scr.loadImg("bgImg", bgSrc);
	scr.makeSprite("bg", "bgImg", -1);
	scr.spriteData("bg")->setStretch(800, 600);
    scr.addSpriteToRender("bg");

    // test font
    TTF_Font* font = TTF_OpenFontIndex("example/kenpixel_mini_square.ttf", 18, 0);
    TTF_Font* font2 = TTF_OpenFontIndex("example/kenpixel_mini_square.ttf", 11, 0);
    SDL_Color colr;
    colr.a = 255;
    colr.r = 0;
    colr.g = 0;
    colr.b = 0;
    scr.text("testTxt1", "Hello there!", font, colr, 200, 50, 0, 0, true);
    scr.text("mouseTxt", "Click & hold for \"water\"", font2, colr, 0, 0, 10, 0, true);
    TTF_CloseFont(font);
    TTF_CloseFont(font2);

	// the box
	scr.loadImg("boxImg", boxSrc);
	scr.makeSprite("box","boxImg", 1);
	scr.spriteData("box")->setPos(20,100);
    scr.addSpriteToRender("box");

	// the water mother sprite to duplicate from.
	scr.loadImg("blob", blobSrc);
	scr.makeSprite("drop","blob", 0);

	// the meteor
	scr.loadImg("meteorImg", meteorSrc);
	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = 232;
	rect.h = 400;
	scr.makeSprite("meteor","meteorImg", 0, &rect);
	std::vector<int>myFrameOrder({0,1,2,3,4,5,6,7,8,9,10});
	scr.spriteData("meteor")->customFrameOrder(myFrameOrder);
	scr.spriteData("meteor")->setStretch(116, 200);
	scr.spriteData("meteor")->setAlpha(222);
	scr.addSpriteToRender("meteor");

	scr.duplicateSprite("meteorThe2nd", "meteor", true);
	scr.spriteData("meteorThe2nd")->setStretch(80, 150);
	scr.spriteData("meteorThe2nd")->setAlpha(100);
	scr.spriteData("meteorThe2nd")->setPos(20,100);
	scr.changeLayer("meteorThe2nd", 3);

    // print all the images and sprites.
    std::vector<std::string> a = scr.getAllSpriteNames();
    std::cout << "-----------allSprites at startup-----------" << std::endl;
    for(auto& aa : a)
        std::cout << aa << std::endl;

    std::vector<std::string> b = scr.getAllImageNames();
    std::cout << "-----------all Images at startup-----------" << std::endl;
    for(auto& bb : b)
        std::cout << bb << std::endl;

    std::vector<std::string> c = scr.getRenderSpriteNames();
	std::cout << "-----------RenderedSprites at startup-----------" << std::endl;
    for(auto& cc : c)
        std::cout << cc << std::endl;
}

b2Body* loadMyPhysics() {
	// from http://google.github.io/liquidfun/Programmers-Guide/html/md__chapter02__hello__box2_d.html
	phy.createWorld(b2Vec2(0.0f, 9.80665f));
	// GROUND BOX
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(40.0f, -0.5f);
	b2Body* groundBody = phy.getWorld()->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	// units is from midcenter. Box is actually 100m*20m.
	groundBox.SetAsBox(40.0f, 0.5f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	// top wall
	groundBodyDef.position.Set(40.0f, 60.5f);
	groundBody = phy.getWorld()->CreateBody(&groundBodyDef);
	groundBox.SetAsBox(40.0f, 0.5f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	// Left wall
	groundBodyDef.position.Set(-0.5f, 30.0f);
	groundBody = phy.getWorld()->CreateBody(&groundBodyDef);
	groundBox.SetAsBox(0.5f, 30.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	// Right wall
	groundBodyDef.position.Set(80.5f, 30.0f);
	groundBody = phy.getWorld()->CreateBody(&groundBodyDef);
	groundBox.SetAsBox(0.5f, 30.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	// DYNAMIC BODY
	b2BodyDef bodyDef;
	// body type must be set to b2_dynamicBody if the body should move in response to forces.
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(20.0f, 10.0f);
	b2Body* body = phy.getWorld()->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(2.5f, 2.5f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 0.7f;
	fixtureDef.restitution = 0.3f;
	fixtureDef.friction = 1.0f;
	body->CreateFixture(&fixtureDef);

	//phy.getTimeStep() = 1.0f / 30.0f;
	phy.getVelocityIterations() = 3;
	phy.getPositionsIterations() = 5;

	return body;
}

void testPhysics(b2Body* body) {
	// it is in meters
	b2Vec2 position = body->GetPosition();
	b2Vec2 center = b2Vec2(2.5f, 2.5f);

	// it is in radians
	float32 angle = body->GetAngle();

	// converting into screen coordinates 10m = 100px.
	scr.spriteData("box")->rotation(angle*(180/3.141592));
	scr.spriteData("box")->setPos(MTOPX*(position.x - center.x), MTOPX*(position.y - center.y));
	// up force
	if(in.keyStatus(SDLK_UP))
		body->ApplyLinearImpulse(b2Vec2(0.0f,-10.0f), body->GetPosition(), true);
	// down force
		if(in.keyStatus(SDLK_DOWN))
			body->ApplyLinearImpulse(b2Vec2(0.0f,10.0f), body->GetPosition(), true);
	// right force
	if(in.keyStatus(SDLK_LEFT))
			body->ApplyLinearImpulse(b2Vec2(-10.0f,0.0f), body->GetPosition(), true);
	// left force
	if(in.keyStatus(SDLK_RIGHT))
			body->ApplyLinearImpulse(b2Vec2(10.0f,0.0f), body->GetPosition(), true);

}


void loadMySounds() {
	// Recommend to use OGG. Mp3 had some weird things like some dont work & froze since probably different mp3 encodings.
	// Flac and Wav can be used too, downside is that they're big in filesize.
	snd.loadMusic("music", bgm);
	snd.loadSFX("waterSnd", sfx);

	// start playing the music and set volume of music to 50%
	Mix_FadeInMusic(snd.getMusic("music"), -1, 2000);
	Mix_VolumeMusic(MIX_MAX_VOLUME/2);
}

void loadMyParticles() {
	phy.createParticles();
	phy.getParticles()->SetGravityScale(static_cast<float32>(2.0f));
	phy.getParticles()->SetRadius(0.6f);
	phy.getParticles()->SetDensity(1.0f);
	// radius 1 default, the smaller radius, the more particles needed to fill something -> performance hit.
	phy.getParticleIterations() = b2CalculateParticleIterations(9.80556f, 0.6f, 1.0f/60.0f);
}

void testParticles() {
	// BUG: This doesnt remove sprites propperly. Sometimes no sprites come due to some already existing so we get invisible water.
	for(size_t j=0; j< bucket.size(); ++j) {
		float radius = phy.getParticles()->GetRadius();
		double xLoc = round(MTOPX * (phy.getParticles()->GetPositionBuffer()[j].x - radius));
		double yLoc = round(MTOPX * (phy.getParticles()->GetPositionBuffer()[j].y - radius));

        // GetExpirationTimeBuff just gets expiration time, doesn't tell you if it's about to be destroyed or not, use the callback b2DestructionListener::SayGoodbye
		if(phy.getParticles()->GetExpirationTimeBuffer()[j] <= 1
				&& scr.spriteData(bucket[j].c_str()) != nullptr) {
			scr.removeSprite(scr.spriteData(bucket[j].c_str())->getName());
			bucket[j] = bucket.back();
			bucket.back() = "";
			bucket.pop_back();
			--j;
		}
		if(scr.spriteData(bucket[j].c_str()) != nullptr) {
			scr.spriteData(bucket[j].c_str())->setPos(static_cast<int>(xLoc), static_cast<int>(yLoc));
		}
	}
	// get mouse coordinates and make text follow mouse
	std::pair<Sint32, Sint32> coords = in.getMouseValues(MouseVals::COORDS);
	scr.spriteData("mouseTxt")->setPos(coords.first, coords.second);

	// make water
	if(in.mouseKeyStatus(SDL_BUTTON_LEFT)) {
		// play water sound if it is not already playing
		if(!Mix_Playing(1))
			Mix_PlayChannel(1, snd.getSFX("waterSnd"), 0);
		// create them at the mouse cursor
		b2ParticleDef pd;
		pd.lifetime = 120;
		pd.flags = b2_springParticle;
		pd.velocity = b2Vec2(0.0f, 5.0f);
		pd.position.Set(coords.first / MTOPX, coords.second / MTOPX);

		phy.getParticles()->CreateParticle(pd);

		//std::string name = "drop" + std::to_string(bucket.size()+1);
		std::ostringstream stream;
		stream << "drop" << bucket.size()+1;
		std::string name = stream.str();
		scr.duplicateSprite(name.c_str(),"drop", true);
		scr.spriteData(name.c_str())->setPos(coords.first, coords.second);
		bucket.push_back(name);

		//print the names of all the water
        /*std::vector<std::string> c = scr.getRenderSpriteNames();
        std::cout << "-----------RenderedSprites-----------" << std::endl;
        for(auto& cc : c)
            std::cout << cc << std::endl;
        */
	}else if(!in.mouseKeyStatus(SDL_BUTTON_LEFT)) {
		// stop water sound if not holding left mouse button.
		Mix_HaltChannel(1);
	}
}
