#pragma once

//it is the object manager that links physics and sprites.
//idea is pure aggregation from http://cowboyprogramming.com/2007/01/05/evolve-your-heirachy/
// used to combine box2d and spriteData.

class Sprite;

class Object {
public:
    Object() : name(""), x(0), y(0), rot(0), spr(nullptr), sprH(0), sprW(0) {}

    Object(const char* objName, int xLocation, int yLocation, float rotation) :
        name(objName), x(xLocation), y(yLocation), rot(rotation), spr(nullptr), sprH(0), sprW(0) {}

    // careful, it copies pointer and sprite name and sizes... only one object for one sprite.
    Object(const Object &rhs) : name(rhs.name), x(rhs.x), y(rhs.y), rot(rhs.rot), spr(rhs.spr),
    sprH(rhs.sprH), sprW(rhs.sprW) {}

    // careful, it assigns pointer and sprite name and sizes... only one object for one sprite.
    Object& operator=(const Object &rhs);

    ~Object() {/* Sprite that it points to exists in the screen unordered_map and will be cleared there if needed*/}

    // set the position in pixels center origin, and updates it's sprite (if exists) with top-left origin.
    void setPos(int newX, int newY);

    // set the rotation in degrees, and updates it's sprite's rotation, if sprite exists.
    void setRot(float rotationDegrees);

    // get the position in pixels with origin center
    int getXmid() const;
    int getYmid() const;

    // get the position in pixels with origin top-left corner of the object'ssprite
    int getX() const;
    int getY() const;

    // get the rotation degrees.
    float getRot() const;

    // sets the sprite assigned to this object
    void assignSprite(Sprite* sprite);

    // returns the name of the object.
    const char* getName() const;

private:
    int x;
    int y;
    float rot;
    Sprite* spr;
    int sprH;
    int sprW;
    const char* name;
};
