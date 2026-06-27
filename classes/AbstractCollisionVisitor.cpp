#include "AbstractCollisionVisitor.h"

std::list<Sprite*> AbstractCollisionVisitor::getCollisions() {
    std::list<Sprite*> list = this->collisions;
    this->collisions.clear();
    return list;

}

void AbstractCollisionVisitor::setWatched(Sprite* s) {
    this->watched = s;
}