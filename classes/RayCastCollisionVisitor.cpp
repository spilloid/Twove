#include "RayCastCollisionVisitor.h"

#include <algorithm>
#include <limits>

// Ray-vs-AABB by the slab method. The ray starts at the centre of the watched
// sprite and travels along its velocity (dx, dy); every *other* sprite that ray
// passes through (forward only) is recorded as a collision. A watched sprite
// with no velocity casts no ray. This answers "what's in my path?" -- Pong's AI
// uses it to tell when the ball is heading toward its goal.
void RayCastCollisionVisitor::visit(Sprite* s) {
    if (this->watched == nullptr || s == this->watched)
        return;

    double dx = this->watched->getDX();
    double dy = this->watched->getDY();
    if (dx == 0 && dy == 0)
        return; // no direction => no ray

    // ray origin: centre of the watched sprite
    double ox = this->watched->getX() + this->watched->getWidth() / 2.0;
    double oy = this->watched->getY() + this->watched->getHeight() / 2.0;

    // target axis-aligned bounding box
    double minX = s->getX();
    double minY = s->getY();
    double maxX = minX + s->getWidth();
    double maxY = minY + s->getHeight();

    // tmin/tmax bound the ray parameter range that stays inside the box on
    // every axis. Clamp tmin to 0 so only the forward half of the ray counts.
    double tmin = 0.0;
    double tmax = std::numeric_limits<double>::infinity();

    if (dx != 0) {
        double t1 = (minX - ox) / dx;
        double t2 = (maxX - ox) / dx;
        if (t1 > t2) std::swap(t1, t2);
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
    } else if (ox < minX || ox > maxX) {
        return; // ray parallel to the X slab and outside it
    }

    if (dy != 0) {
        double t1 = (minY - oy) / dy;
        double t2 = (maxY - oy) / dy;
        if (t1 > t2) std::swap(t1, t2);
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
    } else if (oy < minY || oy > maxY) {
        return; // ray parallel to the Y slab and outside it
    }

    if (tmax >= tmin) {
        this->collisions.push_back(s);
        this->collisions.unique();
    }
}
