#pragma once
#include <Box2D/Box2D.h>
#include <vector>

class HitTest : public b2ContactListener {
public:
    HitTest();
    ~HitTest() {}
    /// called when 2 fixtures start touching.
    void BeginContact(b2Contact* contact);

    /// called when 2 fixtures stop touching.
    void EndContact(b2Contact* contact);

    /// get a vector of all the b2Body pairs that collided.
    std::vector< std::pair<b2Body*, b2Body*> > getHits() const;

    /// get a vector of all the b2Body pairs that stopped colliding.
    std::vector< std::pair<b2Body*, b2Body*> > getEndingHits() const;

    /// empty the hitTest vectors of the collision vector and the stopped-colliding vector
    /// if parameters are true respectively. If both parameters are false this function does nothing.
    /// Both by default true.
    void clearHits(bool clearContacts = true, bool clearEndContacts = true);

private:
    std::vector< std::pair<b2Body*, b2Body*> > contactPairList;
    std::vector< std::pair<b2Body*, b2Body*> > endContactPairList;
};
