#ifndef BOUNCEBOUNDSVISITOR_H
#define BOUNCEBOUNDSVISITOR_H
#include "Visitor.h"
class BounceBoundsVisitor : public Visitor{
    /**
     * @brief bounds of sprites
     * 
     */
    int minX, maxX, minY, maxY;
    public:
    /**
     * @brief Construct a new Bounce Bounds Visitor object
     * 
     * @param minX minimum X of sprite
     * @param maxX maximum X of sprite
     * @param minY minimum Y of sprite
     * @param maxY maximum Y of sprite
     */
    BounceBoundsVisitor(int minX, int maxX, int minY, int maxY);

    /**
     * @brief if sprite is outside of bounds, move it into bounds then have it BOUNCE
     * 
     * @param sp 
     */
    void visit(Sprite* sp) override;
};
#endif