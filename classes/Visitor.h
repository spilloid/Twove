#ifndef VISITOR_H
#define VISITOR_H
#include <iostream>
#include "Sprite.h"
#include <memory>
class Visitor{
    public:
    /**
     * @brief visit a sprite
     *
     * The sprite is a non-owning observer pointer: the scene owns the sprite,
     * the visitor only looks at / mutates it. Passing a raw pointer keeps
     * double dispatch lightweight — no shared_ptr refcount traffic per call.
     *
     * @param sp non-owning pointer to the sprite being visited
     */
    virtual void visit(Sprite* sp) = 0;
};
#endif