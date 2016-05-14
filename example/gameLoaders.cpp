#include "game.h"

void Game::loadMySprites() {
	// the background image
	scr->loadImg("bgImg", bgSrc);
	scr->makeSprite("bg", "bgImg", -1);
	scr->getSprite("bg")->setPos(-100, -100);
	scr->getSprite("bg")->setStretch(1000, 800);
    scr->addSpriteToRender("bg");

    // test tileset
    scr->loadImg("tileSetImg", tileLoc);
	SDL_Rect tileSize;
	tileSize.x = tileSize.y = 0;
	tileSize.w = 16;
	tileSize.h = 16;
    scr->makeSprite("tileSet", "tileSetImg", 5, &tileSize, 1, false, true);
    scr->getSprite("tileSet")->setFrame(56);

    scr->duplicateSprite("tileSet2", "tileSet", true);
    scr->getSprite("tileSet2")->setFrame(113);
	scr->getSprite("tileSet2")->setPos(0,16);

    // animated tileset
	scr->makeSprite("tileSetAni","tileSetImg", 0, &tileSize, 1, true, true);
	scr->getSprite("tileSetAni")->setPos(16,16);
	std::vector<int>framez({470,471});
	scr->getSprite("tileSetAni")->customFrameOrder(framez);
    scr->getSprite("tileSetAni")->delayFrames(30);

	// the box
	scr->loadImg("boxImg", boxSrc);
	scr->makeSprite("box","boxImg", 1);
	scr->getSprite("box")->setPos(20,100);
    scr->addSpriteToRender("box");

	// the water mother sprite to duplicate from.
	scr->loadImg("blob", blobSrc);
	scr->makeSprite("drop","blob", 0);

	// the meteor
	scr->loadImg("meteorImg", meteorSrc);
	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = 232;
	rect.h = 400;
	scr->makeSprite("meteor","meteorImg", 0, &rect);
	std::vector<int>myFrameOrder({0,1,2,3,4,5,6,7,8,9,10});
	scr->getSprite("meteor")->customFrameOrder(myFrameOrder);
	//scr->getSprite("meteor")->delayFrames(10);
	scr->getSprite("meteor")->setStretch(116, 200);
	scr->getSprite("meteor")->setAlpha(200);
	scr->addSpriteToRender("meteor");
    // meteor duplicate
    scr->duplicateSprite("meteorThe2nd", "meteor", true);
	scr->getSprite("meteorThe2nd")->setStretch(80, 150);
	scr->getSprite("meteorThe2nd")->setAlpha(100);
	scr->getSprite("meteorThe2nd")->setPos(100,50);
	scr->changeLayer("meteorThe2nd", 3);

	// Batch them up.
	std::unordered_map<std::string, SDL_Texture*> toBatch;
    for(auto &&it : scr->getAllImageNames()) {
        toBatch[it] = scr->getTexture(it);
    }

    // not good atm, screen resize = everything black, also no alpha for some reason.
	//scr->createSpriteBatch(toBatch);

    // test font
    scr->getFonts()->createFont("font1", fontLoc, 18,0);
    scr->getFonts()->createFont("font2", fontLoc, 11,0);
    // R, G, B, alpha.
    SDL_Color colr = {0,0,0,255};
    scr->text("testTxt1", "Hello there!", "font1", colr, 200, 50, 0, 0, true);
    scr->text("mouseTxt", "Click & hold for \"water\"", "font2", colr, 0, 0, 10, 0, true);


    // print all the images and sprites.
    std::vector<std::string> a = scr->getAllSpriteNames();
    std::cout << "-----------allSprites at startup-----------" << std::endl;
    for(auto& aa : a)
        std::cout << aa << std::endl;

    std::vector<std::string> b = scr->getAllImageNames();
    std::cout << "-----------all Images at startup-----------" << std::endl;
    for(auto& bb : b)
        std::cout << bb << std::endl;

    std::vector<std::string> c = scr->getRenderSpriteNames();
	std::cout << "-----------RenderedSprites at startup-----------" << std::endl;
    for(auto& cc : c)
        std::cout << cc << std::endl;

	std::cout << "-----------Batched images-----------" << std::endl;
    for(auto& bathd : toBatch)
        std::cout << bathd.first << std::endl;
}

void Game::loadMyPhysics() {
	// from http://google.github.io/liquidfun/Programmers-Guide/html/md__chapter02__hello__box2_d.html
	// GROUND BOX
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(40.0f, -0.5f);
	b2Body* groundBody = phy->getWorld()->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	// units is from midcenter. Box is actually 100m*20m.
	groundBox.SetAsBox(40.0f, 0.5f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	// top wall
	groundBodyDef.position.Set(40.0f, 60.5f);
	groundBody = phy->getWorld()->CreateBody(&groundBodyDef);
	groundBox.SetAsBox(40.0f, 0.5f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	// Left wall
	groundBodyDef.position.Set(-0.5f, 30.0f);
	groundBody = phy->getWorld()->CreateBody(&groundBodyDef);
	groundBox.SetAsBox(0.5f, 30.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	// Right wall
	groundBodyDef.position.Set(80.5f, 30.0f);
	groundBody = phy->getWorld()->CreateBody(&groundBodyDef);
	groundBox.SetAsBox(0.5f, 30.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

		// DYNAMIC BODY
	b2BodyDef bodyDef;
	// body type must be set to b2_dynamicBody if the body should move in response to forces.
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(20.0f, 10.0f);
	b2Body* body = phy->getWorld()->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(2.5f, 2.5f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 0.8f;
	fixtureDef.restitution = 0.3f;
	fixtureDef.friction = 1.0f;
	body->CreateFixture(&fixtureDef);

    objMan->createObject("boxObj", NULL,0, 0, 0.0f, body, scr->getSprite("box"));
	body->SetUserData(objMan->getObj("boxObj"));
    theBody = body;

    // iteration values
	phy->getVelocityIterations() = 3;
	phy->getPositionsIterations() = 5;
}

void Game::loadMyParticles() {
	phy->createParticles();
	phy->getParticles()->SetGravityScale(static_cast<float32>(3.0f));
	phy->getParticles()->SetRadius(0.6f);
	phy->getParticles()->SetDensity(1.0f);
	// radius 1 default, the smaller radius, the more particles needed to fill something -> performance hit.
	phy->getParticleIterations() = b2CalculateParticleIterations(GRAVITY, 0.6f, fixedTimez);
}

void Game::loadMyJelly() {
	//create particle group
	b2ParticleGroupDef pd;
	b2PolygonShape shape;
	shape.SetAsBox(10,5);
	pd.shape = &shape;

	// for jelly-ness
	pd.flags = b2_elasticParticle;//b2_elasticParticle;//b2_springParticle;//b2_viscousParticle;

	pd.strength = 0.95;
	pd.position.Set(40,50);
	phy->getParticles()->CreateParticleGroup(pd);
}

void Game::loadMySounds() {
	// Recommend to use OGG. Mp3 had some weird things like some dont work & froze since probably different mp3 encodings.
	// Flac and Wav can be used too, downside is that they're big in filesize.
	snd->loadMusic("music", bgm);
	snd->loadSFX("waterSnd", sfx);

	//start playing the music and set volume of music to 50%
	Mix_FadeInMusic(snd->getMusic("music"), -1, 2000);
	Mix_VolumeMusic(MIX_MAX_VOLUME/2);

}
