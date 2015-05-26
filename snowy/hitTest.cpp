#include "hitTest.h"
#include "object.h"

#include <iostream>

HitTest::HitTest() {}

void HitTest::BeginContact(b2Contact* contact) {
    b2Body* a = contact->GetFixtureA()->GetBody();
    b2Body* b = contact->GetFixtureB()->GetBody();
    contactPairList.push_back(std::make_pair(a, b));
}

void HitTest::EndContact(b2Contact* contact) {
    b2Body* a = contact->GetFixtureA()->GetBody();
    b2Body* b = contact->GetFixtureB()->GetBody();
    endContactPairList.push_back(std::make_pair(a, b));
}

std::vector< std::pair<b2Body*, b2Body*> > HitTest::getHits() const{
    return contactPairList;
}

std::vector< std::pair<b2Body*, b2Body*> > HitTest::getEndingHits() const{
    return endContactPairList;
}

void HitTest::clearHits(bool clearContacts, bool clearEndContacts) {
    if(clearContacts)
        contactPairList.clear();
    if(clearEndContacts)
        endContactPairList.clear();
}
