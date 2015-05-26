#pragma once
#include <string>
#include <vector>

class Object;
class Screen;

class Camera {
public:

Camera() : camPxOffsetX(0), camPxOffsetY(0), lastCamX(0), lastCamY(0) {}
~Camera() {}

/// returns x and y camera offset value respectively.
std::pair<int, int> getCamOffset() const;

/// set the camera offset value to a specific XY coordinate.
void setCamOffset(int x, int y);

/// updates and changes the sprite's coordinates in the
/// screen's sprites and objects according to the camera's offset values.
/// Also updates each object's sprite on the screen to the object's current
/// coordinates + camera offset value, and also updates  the object sprite's rotation.
void camUpdate(std::vector<Object*> objList, Screen* scr);

private:
    int camPxOffsetX;
    int camPxOffsetY;
    int lastCamX;
    int lastCamY;
    std::vector<std::string> sprList;
    std::pair<int, int> pos;
    std::pair<float,float> org;
};
