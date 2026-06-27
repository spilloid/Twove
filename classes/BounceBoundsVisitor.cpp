#include "BounceBoundsVisitor.h"

BounceBoundsVisitor::BounceBoundsVisitor(int minX, int maxX, int minY, int maxY)
        : minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}

void BounceBoundsVisitor::visit(Sprite* sp) {
    double ox = sp->getX();
    double oy = sp->getY();
    double nx = ox;
    double ny = oy;
    //flip on boundary hit
    double ix = 1, iy = 1;
    //check bounds of x and y coords, setting to max/min of breaking threshold
    //TODO: make the dx / dy bounce!
    if (ox > maxX) {
        nx = this->maxX;
        ix = -1;
    } else if (ox < minX) {
        nx = this->minX;
            ix = -1;
        }
        if(oy > maxY){
            ny = this->maxY;
            iy = -1;
        }else if(oy < minY){
            ny = this->minY;
            iy = -1;
        }
        sp->setXY(nx,ny);
        sp->setDXY(ix*sp->getDX(),iy*sp->getDY());
    }