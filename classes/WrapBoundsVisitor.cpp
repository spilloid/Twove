#include "WrapBoundsVisitor.h"
    WrapBoundsVisitor::WrapBoundsVisitor(int minX, int maxX, int minY, int maxY)
            : minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}

void WrapBoundsVisitor::visit(Sprite* sp) {
    double ox = sp->getX();
    double oy = sp->getY();
    double nx = ox;
    double ny = oy;
    //check bounds of x and y coordinates, setting to max/min of breaking threshold
    if (ox > this->maxX)
        nx = this->minX;
    else if (ox < this->minX)
        nx = this->maxX;
    if (oy > maxY)
        ny = this->minY;
    else if (oy < minY)
        ny = this->maxY;
    sp->setXY(nx, ny);
}