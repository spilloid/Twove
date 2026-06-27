
#ifndef RAYCAST_COLLISIONVISITOR_H
#define RAYCAST_COLLISIONVISITOR_H

#include "AbstractCollisionVisitor.h"

class RayCastCollisionVisitor : public AbstractCollisionVisitor {
    void visit(Sprite* s) override;
};

#endif