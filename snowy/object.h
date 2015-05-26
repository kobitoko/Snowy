#pragma once
#include <string>

//it is the object manager that links physics and sprites.
//idea is pure aggregation from http:///cowboyprogramming.com/2007/01/05/evolve-your-heirachy/
// used to combine box2d and spriteData.

class Sprite;
class b2Body;
class Object {
public:
    Object() : name(""), typ(0), x(0), y(0), rot(0), theBody(nullptr), spr(nullptr) {}

    Object(std::string objName, int type, int xLocation, int yLocation, float rotation, b2Body* body) :
            name(objName), typ(type), x(xLocation), y(yLocation), rot(rotation), theBody(body), spr(nullptr),
            prevPos(std::make_pair(-1, -1)), prevAng(-1) {}

    /// Copy Construct will ALSO copies the sprite's pointer and the body pointer.
    Object(const Object &rhs) : name(rhs.name), typ(rhs.typ), x(rhs.x), y(rhs.y), rot(rhs.rot), theBody(rhs.theBody),
            spr(rhs.spr), prevPos(rhs.prevPos), prevAng(rhs.prevAng) {}

    /// Assignment Operator ALSO copies the sprite's pointer and the body pointer.
    Object& operator=(const Object &rhs);

    ~Object();

    /// set the position in pixels center origin.
    void setPos(float newXpx, float newYpx);

    /// set the rotation in degrees
    void setRot(float rotationDegrees);

    /// set the previous position in pixels center origin.
    void setPrevPos(float newX, float newY);

    /// set the previous rotation in degrees
    void setPrevRot(float rotationDegrees);

    /// get the position in pixels with origin center
    std::pair<float, float> getCenterPos() const;

    /// get the rotation degrees.
    float getRot() const;

    /// set the previous position in pixels center origin.
    std::pair<float, float> getPrevPos() const;

    /// set the previous rotation in degrees
    float getPrevRot() const;

    /// sets the sprite assigned to this object by storing the sprite's pointer
    /// returns 0 if sprite passed is nullptr
    int assignSprite(Sprite* sprite);

    /// gets the sprite assigned to this object, returns nullptr if there is none.
    Sprite* getSprite();

    /// returns the name of the object.
    std::string getName() const;

    /// returns the typeid int
    int getType() const;

    /// sets the typeid int
    void setType(int type);

    /// get the body assigned to this object, nullptr if there is none.
    b2Body* getBody();

    /// assign a body to this object by storing it's pointer, returns 0 if pointer given is nullptr.
    int assignBody(b2Body* body);

    /// destroys the body assigned to this object via b2World::DestroyBody and sets it to nullptr
    /// returns 0 if body is nullptr.
    int destroyBody();

private:
    std::string name;
    int typ;
    float x;
    float y;
    float rot;
    b2Body* theBody;
    Sprite* spr;
    // for interpolation
    std::pair<float, float> prevPos;
    float prevAng;
};
