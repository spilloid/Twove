#ifndef WRAPBOUNDSVISITOR_H
#define WRAPBOUNDSVISITOR_H
#include "Visitor.h"
class WrapBoundsVisitor : public Visitor{
    private:
    /**
     * @brief bounds of scene
     * 
     */
    int minX,maxX,minY,maxY;
    public:
    /**
     * @brief Construct a new Wrap Bounds Visitor object
     * 
     * @param minX minimum X of sprite
     * @param maxX maximum X of sprite
     * @param minY minimum Y of sprite
     * @param maxY maximum Y of sprite
     */
    WrapBoundsVisitor(int minX, int maxX, int minY, int maxY);

    /**
     * @brief if sprite is outside of bounds, move it into bounds then have it WRAP
     * 
     * @param s 
     */
    void visit(Sprite* s) override;
};
#endif