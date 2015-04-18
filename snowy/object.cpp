#include <cmath>
#include "object.h"
#include "errorHandler.h"
#include "spriteData.h"

Object& Object::operator=(const Object &rhs) {
    if(this != &rhs) {
        x = rhs.x;
        y = rhs.y;
        rot = rhs.rot;
        spr = rhs.spr;
        sprH = rhs.sprH;
        sprW = rhs.sprW;
        name = rhs.name;
    }
    return *this;
}

void Object::setPos(int newX, int newY) {
    x = newX;
    y = newY;
    // change sprite's position too if the object has a sprite assigned to it.
    if(spr != nullptr)
        spr->setPos(newX - sprH, newY - sprW);
}

void Object::setRot(float rotationDegrees) {
    rot = rotationDegrees;
    if(spr != nullptr)
        spr->rotation(static_cast<double>(rot));
}

int Object::getXmid() const {
    return x;
}

int Object::getYmid() const {
    return y;
}

int Object::getX() const {
    return x - sprH;
}

int Object::getY() const {
    return y - sprW;
}

float Object::getRot() const {
    return rot;
}

void Object::assignSprite(Sprite* sprite) {
    // all sprites are saved in the unordered_map in the Screen object.
    spr = sprite;
    sprH = static_cast<int>(round(sprite->getFrameSizeH() * 0.5f));
    sprW = static_cast<int>(round(sprite->getFrameSizeW() * 0.5f));
}

const char* Object::getName() const {
    return name;
}
