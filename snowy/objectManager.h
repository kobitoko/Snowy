#pragma once
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "object.h"

class Sprite;

class ObjectManager {
public:
    ObjectManager() {}
    ~ObjectManager() {}

    // creates an object and stores it in an unordered map. Returns 0 if an object with the same name already exists.
    int createObject(const char* objName, int x, int y, float rotation, Sprite* spriteToAssign);

    bool objectExists(const char* name) const;

    // retrieve a certain object's data so one can modify it. Returns nullpointer if object with given name doesn't exist.
    Object* objData(const char* objectName);

    // returns a vector containing all object names.
    std::vector<std::string> getAllObjectNames() const;

    // destroys the object returns 1 if successful. Returns 0 if object with such name doesn't exist.
	int destroyObject(const char* name);

private:
    std::unordered_map<const char*, Object> allObj;
};
