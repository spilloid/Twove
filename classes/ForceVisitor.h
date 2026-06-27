#ifndef FORCE_VISITOR_H
#define FORCE_VISITOR_H

#include "Visitor.h"
#include <map>
#include <cmath>

class ForceVisitor : public Visitor {
private:
public:
    /**
     * @brief update force data on sprite (move it dx/dy)
     * 
     * @param s 
     */
    void visit(Sprite* s) override;

    /**
     * @brief apply a force to the sprite
     * 
     * @param s sprite to be operated on
     * @param magnitude force magnitude
     * @param angle angle of operation, in degrees
     */
    static void applyForce(const std::shared_ptr<Sprite> &s, double magnitude, float angle);

    /**
     * @brief Stop the sprite from moving
     * 
     * @param s 
     */
    static void stop(const std::shared_ptr<Sprite> &s);
};
#endif