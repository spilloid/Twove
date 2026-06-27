#include "GridDrawingVisitor.h"

#include <utility>

GridDrawingVisitor::GridDrawingVisitor(int maxX, int maxY, std::shared_ptr<AbstractRenderer> ar)
        : maxX(maxX), maxY(maxY) {
    this->renderer = std::move(ar);
}

void GridDrawingVisitor::draw() {
    this->renderer->clear();

    //draw the board grid lines first so sprites land on top of them. A faint
    //grey keeps the grid visible without competing with the pieces.
    unsigned int pxW = this->renderer->getWidth();
    unsigned int pxH = this->renderer->getHeight();
    unsigned int blockW = pxW / this->maxX;
    unsigned int blockH = pxH / this->maxY;
    for (int c = 0; c <= this->maxX; ++c)
        this->renderer->drawLine(c * blockW, 0, c * blockW, pxH, 200, 200, 200);
    for (int r = 0; r <= this->maxY; ++r)
        this->renderer->drawLine(0, r * blockH, pxW, r * blockH, 200, 200, 200);

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
  this->renderer->present();
  this->renderList.clear();
}

bool GridDrawingVisitor::isOpen() {
    return this->renderer->isOpen();
}

void GridDrawingVisitor::visit(Sprite* s) {
    this->renderList.push_back(s);
}