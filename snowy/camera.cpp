#include "camera.h"
#include "screen.h"
#include "spriteData.h"
#include "object.h"

std::pair<int, int> Camera::getCamOffset() const {
    return std::make_pair(camPxOffsetX, camPxOffsetY);
}

void Camera::setCamOffset(int x, int y) {
    camPxOffsetX = x;
    camPxOffsetY = y;
}

void Camera::camUpdate(std::vector<Object*> objList, Screen* scr) {
    int camDx = camPxOffsetX - lastCamX;
    int camDy = camPxOffsetY - lastCamY;
    lastCamX = camPxOffsetX;
    lastCamY = camPxOffsetY;
    sprList = scr->getRenderSpriteNames();
    // Render sprites offsets.
    for(auto& i : sprList) {
        Sprite* spr = scr->getSprite(i.c_str());
        spr->setPos(spr->getPos()->x + camDx, spr->getPos()->y + camDy);
    }
    // Object sprites offsets. Should come after updating all sprites positions.
    // as objects update their sprites with values that come from the physics simulator.
    for(size_t i = 0; i < objList.size(); ++i) {
        Sprite* spr = objList[i]->getSprite();
        pos = objList[i]->getCenterPos();
        org = spr->getOrigin();
        // no need for camera offset difference as physics simulator resets values to original values.
        spr->setPos((pos.first - org.first) + camPxOffsetX, (pos.second - org.second) + camPxOffsetY);
        spr->rotation(objList[i]->getRot());
    }
}
