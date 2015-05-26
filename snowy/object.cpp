#include <Box2D/Box2D.h>
#include "object.h"
#include "tools.h"


Object& Object::operator=(const Object &rhs) {
    if(this != &rhs) {
        name = rhs.name;
        typ = rhs.typ;
        x = rhs.x;
        y = rhs.y;
        rot = rhs.rot;
        theBody = rhs.theBody;
        spr = rhs.spr;
        prevPos = rhs.prevPos;
        prevAng = rhs.prevAng;
    }
    return *this;
}

Object::~Object() {

}

void Object::setPos(float newXpx, float newYpx) {
    x = newXpx;
    y = newYpx;
}

void Object::setRot(float rotationDegrees) {
    rot = rotationDegrees;
}

void Object::setPrevPos(float newX, float newY) {
    prevPos = std::make_pair(newX, newY);
}

void Object::setPrevRot(float rotationDegrees) {
    prevAng = rotationDegrees;
}

std::pair<float, float> Object::getCenterPos() const {
    return std::make_pair(x, y);
}

float Object::getRot() const {
    return rot;
}

std::pair<float, float> Object::getPrevPos() const {
    return prevPos;
}

float Object::getPrevRot() const {
    return prevAng;
}

Sprite* Object::getSprite() {
    return spr;
}

int Object::assignSprite(Sprite* sprite) {
    if(sprite == nullptr)
        return 0;
    spr = sprite;
    return 1;
}

std::string Object::getName() const {
    return name;
}

int Object::getType() const {
    return typ;
}

void Object::setType(int type) {
    typ = type;
}

b2Body* Object::getBody() {
    return theBody;
}

int Object::assignBody(b2Body* body) {
    if(body == nullptr)
        return 0;
    theBody = body;
    return 1;
}

int Object::destroyBody() {
    if(theBody == nullptr)
        return 0;
    b2World* world = theBody->GetWorld();
    world->DestroyBody(theBody);
    theBody = nullptr;
    return 1;
}
