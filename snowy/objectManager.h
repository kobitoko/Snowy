#pragma once
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>

class Sprite;
class b2Body;
class Object;

class ObjectManager {
public:
    ObjectManager() {}
    ~ObjectManager();

    /// creates an object and stores it in an unordered map. Returns 0 if an object with the same name already exists.
    int createObject(std::string objName, int type, int x, int y, float rotation, b2Body* body, Sprite* spriteToAssign);

    bool objectExists(std::string name) const;

    /// retrieve a certain object's data so one can modify it. Returns nullpointer if object with given name doesn't exist.
    Object* getObj(std::string objectName);

    /// returns a vector containing all object names.
    std::vector<std::string> getAllObjectNames() const;

    /// returns a vector containing all objects
    std::vector<Object*> getAllObjects();

    /// destroys the object returns 1 if successful. Returns 0 if object with such name doesn't exist.
	int destroyObject(std::string name);


private:
    std::unordered_map<std::string, Object*> allObj;
    std::vector<Object*> allObjVec;
};
