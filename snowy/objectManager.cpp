#include "objectManager.h"
#include "object.h"

ObjectManager::~ObjectManager() {
    for(auto&& it : allObj) {
        delete it.second;
        it.second = nullptr;
    }
    allObj.clear();
}

int ObjectManager::createObject(std::string objName, int type, int x, int y, float rotation, b2Body* body, Sprite* spriteToAssign) {
    if(allObj.count(objName) > 0)
        return 0;
    allObj[objName] = new Object(objName, type, x, y, rotation, body);
    allObjVec.push_back(allObj[objName]);
    if(spriteToAssign != nullptr)
        allObj[objName]->assignSprite(spriteToAssign);
    return 1;
}

bool ObjectManager::objectExists(std::string name) const {
	return allObj.count(name) > 0;
}

Object* ObjectManager::getObj(std::string objectName) {
	if(allObj.count(objectName) == 0)
        return nullptr;
    return allObj[objectName];
}

std::vector<std::string> ObjectManager::getAllObjectNames() const{
    std::vector<std::string> ret;
    for(auto& a : allObj)
        ret.push_back(a.first);
    return ret;
}

std::vector<Object*> ObjectManager::getAllObjects() {
    return allObjVec;
}

int ObjectManager::destroyObject(std::string name) {
	if(allObj.count(name) > 0) {
		for(size_t i = 0; i < allObjVec.size(); ++i) {
            if(allObjVec[i]->getName() == name) {
                allObjVec[i] = allObjVec.back();
                allObjVec.pop_back();
            }
		}
		delete allObj[name];
        allObj.erase(name);
		return 1;
	}
	return 0;
}
