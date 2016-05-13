#include "game.h"

void Game::testInput() {
    //int camMovVal = 5;
    //int horVal = 0;
	//int vertVal = 0;

	// Test keyboard keys
	if(in->keyStatus(SDLK_q)) {
		std::cout << SDL_GetKeyName(SDLK_q) << std::endl << "Bye." << std::endl;
		//running = false;
		running = false;
	}

	// Test window's buttons
	if(in->windowStatus(SDL_WINDOWEVENT_CLOSE)) {
		std::cout << "Window Closed.\nBye." << std::endl;
		running = false;
	}
    //camera moves
    /*
    if(in->keyStatus(SDLK_UP)){
        vertVal = camMovVal;
    }
    if(in->keyStatus(SDLK_DOWN)){
        vertVal = -camMovVal;
    }
    if(in->keyStatus(SDLK_LEFT)){
        horVal = camMovVal;
    }
    if(in->keyStatus(SDLK_RIGHT)){
        horVal = -camMovVal;
    }
	std::pair<int, int> oldCam = cam->getCamOffset();
	cam->setCamOffset(oldCam.first + horVal, oldCam.second + vertVal);
    */
}

void Game::movePlayer() {
		// up force
	if(in->keyStatus(SDLK_w))
		objMan->getObj("boxObj")->getBody()->ApplyLinearImpulse(b2Vec2(0.0f,-20.0f),
                                            objMan->getObj("boxObj")->getBody()->GetPosition(), true);
	// down force
    if(in->keyStatus(SDLK_s))
        objMan->getObj("boxObj")->getBody()->ApplyLinearImpulse(b2Vec2(0.0f,20.0f),
                                            objMan->getObj("boxObj")->getBody()->GetPosition(), true);
	// right force
	if(in->keyStatus(SDLK_a))
        objMan->getObj("boxObj")->getBody()->ApplyLinearImpulse(b2Vec2(-20.0f,0.0f),
                                            objMan->getObj("boxObj")->getBody()->GetPosition(), true);
	// left force
	if(in->keyStatus(SDLK_d))
        objMan->getObj("boxObj")->getBody()->ApplyLinearImpulse(b2Vec2(20.0f,0.0f),
                                            objMan->getObj("boxObj")->getBody()->GetPosition(), true);
}

void Game::testParticles() {
    SDL_Color colr = {0,0,0,255};
    std::pair<Sint32, Sint32> coords = in->getMouseValues(MouseVals::COORDS);
    int32 bucketSize = static_cast<int32>(bucket.size());
    int32 particleCounts = phy->getParticles()->GetParticleCount();
    if(bucketSize != particleCounts) {
        if(bucketSize < phy->getParticles()->GetParticleCount()) {
            int32 toAdd = particleCounts - bucketSize;
            for(int32 i = 0; i < toAdd; ++i) {
                std::ostringstream stream;
                stream << "drop" << bucket.size()+1;
                std::string name = stream.str();
                scr->duplicateSprite(name.c_str(),"drop", true);
                bucket.push_back(name);
                scr->addSpriteToRender(name.c_str());
            }
		}else if(bucketSize > particleCounts) {
			scr->removeSprite(scr->getSprite(bucket.back().c_str())->getName());
			bucket.pop_back();
		}
    }

	for(int32 j=0; j< particleCounts; ++j) {
		float radius = phy->getParticles()->GetRadius();
		double xLoc = round(MTOPX * (phy->getParticles()->GetPositionBuffer()[j].x - radius));
		double yLoc = round(MTOPX * (phy->getParticles()->GetPositionBuffer()[j].y - radius));
		if(scr->getSprite(bucket[j].c_str()) != nullptr) {
			scr->getSprite(bucket[j].c_str())->setPos(static_cast<int>(xLoc), static_cast<int>(yLoc));
		}
	}
	// make water
	if(in->mouseKeyStatus(SDL_BUTTON_LEFT)) {
        // Update text
        scr->text("mouseTxt", "Pouring water!", "font2", colr, 0, 0, 10, 0, true);

		// play water sound if it is not already playing
		if(!Mix_Playing(1))
			Mix_PlayChannel(1, snd->getSFX("waterSnd"), 0);
		// create them at the mouse cursor
		b2ParticleDef pd;
		pd.lifetime = 90;
		pd.flags = b2_springParticle;
		pd.velocity = b2Vec2(0.0f, 5.0f);
		pd.position.Set(coords.first * PXTOM, coords.second * PXTOM);
		phy->getParticles()->CreateParticle(pd);

	}else if(in->mouseKeyStatus(SDL_BUTTON_RIGHT)) {
	    scr->text("mouseTxt", "Not RMB, it is the LMB!", "font2", colr, 0, 0, 10, 0, true);
	}else if(!in->mouseKeyStatus(SDL_BUTTON_LEFT)) {
	    // Reset text
	    scr->text("mouseTxt", "Click & hold for \"water\"", "font2", colr, 0, 0, 10, 0, true);
		// stop water sound if not holding left mouse button.
		Mix_HaltChannel(1);
	}
	// Mouse text follows mouse.
	scr->getSprite("mouseTxt")->setPos(coords.first, coords.second);
}
