#include "surfing.h"
#include <string>
#include <sstream>
#include <cmath>

// SDL2 needs main() to be main(int argc, char * argv[]) and not the simple main()
// https://wiki.libsdl.org/FAQWindows#I_get_.22Undefined_reference_to_.27SDL_main.27.22_...
int main(int argc, char * argv[]) {
	// to prevent spiral of death, set max number of steps.
	const int MAX_STEPS = 8;
	// initialize accumulator and it's ratio.
	accumulator = 0.0f;
	accumulatorRatio = 0.0f;

	// Init
	int x = SDL_WINDOWPOS_CENTERED;
	int y = SDL_WINDOWPOS_CENTERED;
	w = 800;
	h = 600;

    scr = Screen();
    in = Input();
    snd = Sound();
    phy = Physics(GRAVITY, 0.0f, 10.0f, FIXED_TIMESTEP);
    fpsTimer = Timer();
    Timer timer = Timer();
    float renderTime = 0.0f;
    objMan = ObjectManager();

	scr.makeWindow(x,y,w,h,"Surf!");
	//SDL_SetWindowBordered(window.getWindow(),SDL_FALSE);
	std::string winName = "Surf!";

	// load the test sprites
	loadMySprites();

	// load sounds n music
	loadMySounds();

	// load the physics
	theBody = loadMyPhysics();
	loadMyParticles();
	loadMyJelly();

	running = true;

	// fixed time step from http://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d
	while(running) {
        timer.stop();

        renderTime += timer.getSeconds();
        accumulator += timer.getSeconds();
        timer.start();
        const int nSteps = static_cast<int>(std::floor(accumulator / FIXED_TIMESTEP));

        //avoiding rounding error, touches accumulator only if needed.
        if(nSteps > 0)
            accumulator -= nSteps * FIXED_TIMESTEP;

        accumulatorRatio = accumulator / FIXED_TIMESTEP;

        // similar to clamp "dt" but keeps above calculations of accumulator and accumulatorRatio unchaged.
        const int nStepsClamped = std::min(nSteps, MAX_STEPS);
        for(int i = 0; i < nStepsClamped; ++i) {
            phy.resetSmoothStates();
            updateLogic();
        }
        phy.getWorld()->ClearForces();
        phy.smoothStates(accumulatorRatio, FIXED_TIMESTEP);

        if(renderTime >= CAPPED_FRAMERATE) {
            renderTime = 0;
            // update screen.
            scr.update();

            float fpsVal = getFps();

            std::cout << "FPS:" << fpsVal << " Frame took " << timer.getSeconds() << " seconds.       \r";
            std::ostringstream winNameFps;
            winNameFps << "FPS:" << fpsVal;
            std::string winName = "Surf! - " + winNameFps.str();
            SDL_SetWindowTitle(scr.getWindow(), winName.c_str());
        }
	}
	SDL_Quit();
	return 0;
}

void updateLogic() {
    in.updateInput();

    // Test input
    running = testInput();

    // update physics.
    phy.update();

    // physics and particle testing
    testPhysics(theBody);
    testParticles();
}

float getFps() {
    float give = 1000.0f / fpsTimer.stop();
    fpsTimer.start();
    return give;
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

    // test tileset
    scr.loadImg("tileSetImg", tileLoc);
	SDL_Rect tileSize;
	tileSize.x = tileSize.y = 0;
	tileSize.w = 16;
	tileSize.h = 16;
    scr.makeSprite("tileSet", "tileSetImg", 5, &tileSize, 1, false, true);
    scr.spriteData("tileSet")->setFrame(56);

    scr.duplicateSprite("tileSet2", "tileSet", true);
    scr.spriteData("tileSet2")->setFrame(113);
	scr.spriteData("tileSet2")->setPos(0,16);

    // animated tileset
	scr.makeSprite("tileSetAni","tileSetImg", 0, &tileSize, 1, true, true);
	scr.spriteData("tileSetAni")->setPos(16,16);
	std::vector<int>framez({470,471});
	scr.spriteData("tileSetAni")->customFrameOrder(framez);
    scr.spriteData("tileSetAni")->delayFrames(30);

    // test font
    TTF_Font* font = TTF_OpenFontIndex(fontLoc, 18, 0);
    TTF_Font* font2 = TTF_OpenFontIndex(fontLoc, 11, 0);
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
	scr.spriteData("meteorThe2nd")->setPos(100,50);
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
	fixtureDef.density = 0.8f;
	fixtureDef.restitution = 0.3f;
	fixtureDef.friction = 1.0f;
	body->CreateFixture(&fixtureDef);

    objMan.createObject("boxObj",0, 0, 0.0f, scr.spriteData("box"));
	body->SetUserData(objMan.objData("boxObj"));

	phy.getVelocityIterations() = 3;
	phy.getPositionsIterations() = 5;
	return body;
}

void testPhysics(b2Body* body) {
	// up force
	if(in.keyStatus(SDLK_UP))
		body->ApplyLinearImpulse(b2Vec2(0.0f,-20.0f), body->GetPosition(), true);
	// down force
    if(in.keyStatus(SDLK_DOWN))
        body->ApplyLinearImpulse(b2Vec2(0.0f,20.0f), body->GetPosition(), true);
	// right force
	if(in.keyStatus(SDLK_LEFT))
        body->ApplyLinearImpulse(b2Vec2(-20.0f,0.0f), body->GetPosition(), true);
	// left force
	if(in.keyStatus(SDLK_RIGHT))
        body->ApplyLinearImpulse(b2Vec2(20.0f,0.0f), body->GetPosition(), true);
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
	phy.getParticles()->SetGravityScale(static_cast<float32>(3.0f));
	phy.getParticles()->SetRadius(0.6f);
	phy.getParticles()->SetDensity(1.0f);
	// radius 1 default, the smaller radius, the more particles needed to fill something -> performance hit.
	phy.getParticleIterations() = b2CalculateParticleIterations(GRAVITY, 0.6f, FIXED_TIMESTEP);
}

void loadMyJelly() {
	//create particle group
	b2ParticleGroupDef pd;
	b2PolygonShape shape;
	shape.SetAsBox(10,5);
	pd.shape = &shape;

	// for jelly-ness
	pd.flags = b2_elasticParticle;//b2_elasticParticle;//b2_springParticle;//b2_viscousParticle;

	pd.strength = 0.95;
	pd.position.Set(40,50);
	phy.getParticles()->CreateParticleGroup(pd);
}

void testParticles() {
    std::pair<Sint32, Sint32> coords = in.getMouseValues(MouseVals::COORDS);
    scr.spriteData("mouseTxt")->setPos(coords.first, coords.second);
    int32 bucketSize = static_cast<int32>(bucket.size());
    int32 particleCounts = phy.getParticles()->GetParticleCount();
    if(bucketSize != particleCounts) {
        if(bucketSize < phy.getParticles()->GetParticleCount()) {
            int32 toAdd = particleCounts - bucketSize;
            for(int32 i = 0; i < toAdd; ++i) {
                std::ostringstream stream;
                stream << "drop" << bucket.size()+1;
                std::string name = stream.str();
                scr.duplicateSprite(name.c_str(),"drop", true);
                bucket.push_back(name);
                scr.addSpriteToRender(name.c_str());
            }
		}else if(bucketSize > particleCounts) {
			scr.removeSprite(scr.spriteData(bucket.back().c_str())->getName());
			bucket.pop_back();
		}
    }
	for(int32 j=0; j< particleCounts; ++j) {
		float radius = phy.getParticles()->GetRadius();
		double xLoc = round(MTOPX * (phy.getParticles()->GetPositionBuffer()[j].x - radius));
		double yLoc = round(MTOPX * (phy.getParticles()->GetPositionBuffer()[j].y - radius));
		if(scr.spriteData(bucket[j].c_str()) != nullptr) {
			scr.spriteData(bucket[j].c_str())->setPos(static_cast<int>(xLoc), static_cast<int>(yLoc));
		}
	}
	// make water
	if(in.mouseKeyStatus(SDL_BUTTON_LEFT)) {
		// play water sound if it is not already playing
		if(!Mix_Playing(1))
			Mix_PlayChannel(1, snd.getSFX("waterSnd"), 0);
		// create them at the mouse cursor
		b2ParticleDef pd;
		pd.lifetime = 80;
		pd.flags = b2_springParticle;
		pd.velocity = b2Vec2(0.0f, 5.0f);
		pd.position.Set(coords.first / MTOPX, coords.second / MTOPX);
		phy.getParticles()->CreateParticle(pd);
	}else if(!in.mouseKeyStatus(SDL_BUTTON_LEFT)) {
		// stop water sound if not holding left mouse button.
		Mix_HaltChannel(1);
	}
}
