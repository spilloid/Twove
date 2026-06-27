#include "ForceVisitor.h"
void ForceVisitor::visit(Sprite* s) {
    //for every object with a force on it
    s->setXY(
            s->getX() + s->getDX(),
            s->getY() + s->getDY()
    );
}

void ForceVisitor::applyForce(const std::shared_ptr<Sprite> &s, double magnitude, float angle) {
    double dx = s->getDX(), dy = s->getDY();
    //convert to radians because we're math people
    double radianAngle = (angle - 90.f) * M_PI / 180.f;
    //calc dx & dy like our lord and savior Pythagoras taught us
    dx += magnitude * cos(radianAngle);
    dy += magnitude * sin(radianAngle);
    s->setDXY(dx, dy);
}

void ForceVisitor::stop(const std::shared_ptr<Sprite> &s) {
    s->setDXY(0, 0);
}