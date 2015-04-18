#include "objectManager.h"

int ObjectManager::createObject(const char* objName, int x, int y, float rotation, Sprite* spriteToAssign) {
    if(allObj.count(objName) > 0)
        return 0;
    allObj[objName] = Object(objName, x, y, rotation);
    if(spriteToAssign != nullptr)
        allObj[objName].assignSprite(spriteToAssign);
    return 1;
}

bool ObjectManager::objectExists(const char* name) const {
	return allObj.count(name) > 0;
}

Object* ObjectManager::objData(const char* objectName) {
	if(allObj.count(objectName) == 0)
        return nullptr;
    return &allObj[objectName];
}

std::vector<std::string> ObjectManager::getAllObjectNames() const{
    std::vector<std::string> ret;
    for(auto& a : allObj)
        ret.push_back(a.first);
    return ret;
}

int ObjectManager::destroyObject(const char* name) {
	if(allObj.count(name) > 0) {
		allObj.erase(name);
		return 1;
	}
	return 0;
}
