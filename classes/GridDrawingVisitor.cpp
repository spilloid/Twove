#include "GridDrawingVisitor.h"

#include <utility>

GridDrawingVisitor::GridDrawingVisitor(int maxX, int maxY, std::shared_ptr<AbstractRenderer> ar)
        : maxX(maxX), maxY(maxY) {
    this->renderer = std::move(ar);
}

void GridDrawingVisitor::draw() {
    //convert x / y coordinates from grid to pixel  ; done every refresh.
    //the rescaled sprites are freshly built here, so 'owned' holds them alive
    //for the duration of the draw call while 'view' hands the renderer
    //non-owning pointers (matching AbstractRenderer::draw).
    std::vector<std::shared_ptr<Sprite>> owned;
    std::vector<Sprite*> view;
    for (auto *ptr : this->renderList) {
        //get height/width of each square
        unsigned int blockWidth = this->renderer->getWidth() / this->maxX;
        unsigned int blockHeight = this->renderer->getHeight() / this->maxY;
        //convert grid coord --> pixel coord
        double xcoord = ptr->getX() * blockWidth;
        double ycoord = ptr->getY() * blockHeight;
        //make a new sprite with correct coordinates
        owned.push_back(
                std::make_shared<Sprite>(
                        ptr->getTextureLocation(),
                        xcoord,
                        ycoord,
                        blockWidth * ptr->getWidth(),
                        blockHeight * ptr->getHeight()
                ));
        view.push_back(owned.back().get());
    }
  this->renderer->draw(view);
  this->renderList.clear();
}

bool GridDrawingVisitor::isOpen() {
    return this->renderer->isOpen();
}

void GridDrawingVisitor::visit(Sprite* s) {
    this->renderList.push_back(s);
}