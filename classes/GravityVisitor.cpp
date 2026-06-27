#include "GravityVisitor.h"
GravityVisitor::GravityVisitor(double intensity)
        : intensity(intensity) {}

void GravityVisitor::visit(Sprite* s) {
    s->setXY(
            s->getX(),
            s->getY() - this->intensity
    );
}