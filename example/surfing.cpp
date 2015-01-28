#include "surfing.h"

// SDL2 needs main() to be main(int argc, char * argv[]) and not the simple main()
// https://wiki.libsdl.org/FAQWindows#I_get_.22Undefined_reference_to_.27SDL_main.27.22_...
int main(int argc, char * argv[]) {

	std::cout << "Hi there!" << std::endl;

	// writes an error log, if 2nd parameter is false it will not crash the game.
	callError("Hello World!",false);

	Uint32 gameTime = SDL_GetTicks();
	// the core game updates will be capped at 60fps
	Uint32 msPerFrame = 1000/60;

	// Init
	int x = SDL_WINDOWPOS_CENTERED;
	int y = SDL_WINDOWPOS_CENTERED;
	w = 800;
	h = 600;

	Screen::get().makeWindow(x,y,w,h,"Surf!", 30);
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
			//std::cout << "FPS:" << Screen::get().getFps() << std::endl;
			winName = "Surf! - FPS:" + boost::lexical_cast<std::string>(Screen::get().getFps());
			SDL_SetWindowTitle(Screen::get().getWindow(), winName.c_str());

			//Later just perhaps make events seperately on which Input class works on????
			// or custom events class to handle user events.
			Input::get().updateInput();

			// update physics, 2 times to prevent slowmotion water but requires more computation.
			Physics::get().update();
			Physics::get().update();

			// physics and particle testing
			testPhysics(theBody);
			testParticles();

			// Test input
			running = testInput(Input::get());

			gameTime = SDL_GetTicks() + msPerFrame;
		}
		// update graphics  Graphics has its own ticks that go with your given desired FPS to render. In this example it is capped at 30fps.
		Screen::get().update();
	}
	SDL_Quit();
	return 0;
}

// Tests' Function definitions --------------------------------------------------

bool testInput(Input input) {
	// Test keyboard keys
	if(input.keyStatus(SDLK_q)) {
		std::cout << SDL_GetKeyName(SDLK_q) << std::endl << "Bye" << std::endl;
		//running = false;
		return false;
	}

	// Test window's buttons
	if(input.windowStatus(SDL_WINDOWEVENT_CLOSE)) {
		std::cout << "Window Closed." << std::endl;
		//running = false;
		return false;
	}
	return true;
}

void loadMySprites() {
	// the background image
	Screen::get().loadImg("bgImg", bgSrc);
	Screen::get().makeSprite("bg", "bgImg", -1);
	Screen::get().spriteData("bg")->setStretch(800, 600);

	// the box
	Screen::get().loadImg("boxImg", boxSrc);
	Screen::get().makeSprite("box","boxImg", 1);
	Screen::get().spriteData("box")->setPos(20,100);

	// the water mother sprite to duplicate from
	Screen::get().loadImg("blob", blobSrc);
	Screen::get().makeSprite("drop","blob", 0);
	Screen::get().spriteData("drop")->setPos(-10, -10);

	// the meteor
	Screen::get().loadImg("meteorImg", meteorSrc);
	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = 232;
	rect.h = 400;
	Screen::get().makeSprite("meteor","meteorImg", 3, &rect);
	std::vector<int>myFrameOrder({0,1,2,3,4,5,6,7,8,9,10});
	Screen::get().spriteData("meteor")->customFrameOrder(myFrameOrder);
	Screen::get().spriteData("meteor")->setStretch(116, 200);
	Screen::get().spriteData("meteor")->setAlpha(222);
}

b2Body* loadMyPhysics() {
	// from http://google.github.io/liquidfun/Programmers-Guide/html/md__chapter02__hello__box2_d.html
	Physics::get().createWorld(b2Vec2(0.0f, 9.80665f));
	// GROUND BOX
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(40.0f, -0.5f);
	b2Body* groundBody = Physics::get().getWorld()->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	// units is from midcenter. Box is actually 100m*20m.
	groundBox.SetAsBox(40.0f, 0.5f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	// top wall
	groundBodyDef.position.Set(40.0f, 60.5f);
	groundBody = Physics::get().getWorld()->CreateBody(&groundBodyDef);
	groundBox.SetAsBox(40.0f, 0.5f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	// Left wall
	groundBodyDef.position.Set(-0.5f, 30.0f);
	groundBody = Physics::get().getWorld()->CreateBody(&groundBodyDef);
	groundBox.SetAsBox(0.5f, 30.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	// Right wall
	groundBodyDef.position.Set(80.5f, 30.0f);
	groundBody = Physics::get().getWorld()->CreateBody(&groundBodyDef);
	groundBox.SetAsBox(0.5f, 30.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	// DYNAMIC BODY
	b2BodyDef bodyDef;
	// body type must be set to b2_dynamicBody if the body should move in response to forces.
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(20.0f, 10.0f);
	b2Body* body = Physics::get().getWorld()->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(2.5f, 2.5f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 0.7f;
	fixtureDef.restitution = 0.3f;
	fixtureDef.friction = 1.0f;
	body->CreateFixture(&fixtureDef);

	//Physics::get().getTimeStep() = 1.0f / 30.0f;
	Physics::get().getVelocityIterations() = 3;
	Physics::get().getPositionsIterations() = 5;

	return body;
}

void testPhysics(b2Body* body) {
	// it is in meters
	b2Vec2 position = body->GetPosition();
	b2Vec2 center = b2Vec2(2.5f, 2.5f);

	// it is in radians
	float32 angle = body->GetAngle();

	// converting into screen coordinates 10m = 100px.
	Screen::get().spriteData("box")->rotation(angle*(180/3.141592));
	Screen::get().spriteData("box")->setPos(MTOPX*(position.x - center.x), MTOPX*(position.y - center.y));
	// up force
	if(Input::get().keyStatus(SDLK_UP))
		body->ApplyLinearImpulse(b2Vec2(0.0f,-10.0f), body->GetPosition(), true);
	// down force
		if(Input::get().keyStatus(SDLK_DOWN))
			body->ApplyLinearImpulse(b2Vec2(0.0f,10.0f), body->GetPosition(), true);
	// right force
	if(Input::get().keyStatus(SDLK_LEFT))
			body->ApplyLinearImpulse(b2Vec2(-10.0f,0.0f), body->GetPosition(), true);
	// left force
	if(Input::get().keyStatus(SDLK_RIGHT))
			body->ApplyLinearImpulse(b2Vec2(10.0f,0.0f), body->GetPosition(), true);

}


void loadMySounds() {
	// Recommend to use OGG. Mp3 had some weird things like some dont work & froze since probably different mp3 encodings.
	// Flac and Wav can be used too, downside is that they're big in filesize.
	Sound::get().loadMusic("music", bgm);
	Sound::get().loadSFX("waterSnd", sfx);

	// start playing the music and set volume of music to 50%
	Mix_FadeInMusic(Sound::get().getMusic("music"), -1, 2000);
	Mix_VolumeMusic(MIX_MAX_VOLUME/2);
}

void loadMyParticles() {
	Physics::get().createParticles();
	Physics::get().getParticles()->SetGravityScale(static_cast<float32>(2.0f));
	Physics::get().getParticles()->SetRadius(0.6f);
	Physics::get().getParticles()->SetDensity(1.0f);
	// radius 1 default, the smaller radius, the more particles needed to fill something -> performance hit.
	Physics::get().getParticleIterations() = b2CalculateParticleIterations(9.80556f, 0.6f, 1.0f/60.0f);
}

void testParticles() {
	// BUG: This doesnt remove sprites propperly. Sometimes no sprites come due to some already existing so we get invisible water.
	for(size_t j=0; j< bucket.size(); ++j) {
		float radius = Physics::get().getParticles()->GetRadius();
		double xLoc = round(MTOPX * (Physics::get().getParticles()->GetPositionBuffer()[j].x - radius));
		double yLoc = round(MTOPX * (Physics::get().getParticles()->GetPositionBuffer()[j].y - radius));

		if(Physics::get().getParticles()->GetExpirationTimeBuffer()[j] <= 1
				&& Screen::get().spriteData(bucket[j].c_str()) != nullptr) {
			//Screen::get().removeSprite(Screen::get().spriteData(bucket[j].c_str())->getName());
			//temp fix for not deleting sprite. Nope doesnt work either...
			bucket[j] = bucket.back();
			bucket.back() = "";
			bucket.pop_back();
			--j;
		}
		if(Screen::get().spriteData(bucket[j].c_str()) != nullptr) {
			Screen::get().spriteData(bucket[j].c_str())->setPos(static_cast<int>(xLoc), static_cast<int>(yLoc));
		}
	}
	// make water
	if(Input::get().mouseKeyStatus(SDL_BUTTON_LEFT)) {
		// play water sound if it is not already playing
		if(!Mix_Playing(1))
			Mix_PlayChannel(1, Sound::get().getSFX("waterSnd"), 0);
		// create them at the mouse cursor
		std::pair<Sint32, Sint32> coords = Input::get().getMouseValues(MouseVals::COORDS);
		b2ParticleDef pd;
		pd.lifetime = 60;
		pd.flags = b2_springParticle;
		pd.velocity = b2Vec2(0.0f, 5.0f);
		pd.position.Set(coords.first / MTOPX, coords.second / MTOPX);

		Physics::get().getParticles()->CreateParticle(pd);

		std::string name = "drop" + boost::lexical_cast<std::string>(bucket.size()+1);
		Screen::get().duplicateSprite(name.c_str(),"drop");
		bucket.push_back(name);
	}else if(!Input::get().mouseKeyStatus(SDL_BUTTON_LEFT)) {
		// stop water sound if not holding left mouse button.
		Mix_HaltChannel(1);
	}
}
