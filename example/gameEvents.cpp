#include "game.h"

void Game::collisionTesting() {
    std::vector< std::pair<b2Body*, b2Body*> > all = phy->hitTest->getHits();
    for(size_t i = 0; i < all.size(); ++i) {
        Object* objA = static_cast<Object*>(all[i].first->GetUserData());
        Object* objB = static_cast<Object*>(all[i].second->GetUserData());
        if(objA != nullptr) {
            //do stuff on hit
        } else if(objB != nullptr) {
            //do stuff on hit
        }
    }
    phy->hitTest->clearHits();
}
