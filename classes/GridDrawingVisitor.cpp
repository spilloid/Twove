#include "GridDrawingVisitor.h"

#include "VectorDrawing.h"

#include <algorithm>
#include <cmath>
#include <utility>

GridDrawingVisitor::GridDrawingVisitor(int maxX, int maxY, std::shared_ptr<AbstractRenderer> ar)
        : maxX(maxX), maxY(maxY) {
    this->renderer = std::move(ar);
}

void GridDrawingVisitor::setStatusRail(unsigned char r, unsigned char g, unsigned char b) {
    this->statusRailVisible = true;
    this->statusR = r;
    this->statusG = g;
    this->statusB = b;
}

void GridDrawingVisitor::clearStatusRail() {
    this->statusRailVisible = false;
}

void GridDrawingVisitor::setViewport(int x, int y, int width, int height) {
    this->viewportX = x;
    this->viewportY = y;
    this->viewportWidth = width;
    this->viewportHeight = height;
}

void GridDrawingVisitor::setOverlay(std::function<void(AbstractRenderer&)> overlay) {
    this->overlay = std::move(overlay);
}

void GridDrawingVisitor::clearOverlay() {
    this->overlay = nullptr;
}

void GridDrawingVisitor::draw() {
    this->renderer->clear();

    // Draw a quiet board surface first so sprites land on a cleaner stage.
    int originX = this->viewportX;
    int originY = this->viewportY;
    unsigned int pxW = this->viewportWidth > 0 ? this->viewportWidth : this->renderer->getWidth();
    unsigned int pxH = this->viewportHeight > 0 ? this->viewportHeight : this->renderer->getHeight();
    double blockW = pxW / (double) this->maxX;
    double blockH = pxH / (double) this->maxY;

    this->renderer->fillRect(originX, originY, (int) pxW, (int) pxH, 248, 250, 252);
    for (int r = 0; r < this->maxY; ++r) {
        for (int c = 0; c < this->maxX; ++c) {
            int x1 = originX + (int) std::round(c * blockW);
            int y1 = originY + (int) std::round(r * blockH);
            int x2 = originX + (int) std::round((c + 1) * blockW);
            int y2 = originY + (int) std::round((r + 1) * blockH);
            bool alternate = ((r + c) % 2) != 0;
            this->renderer->fillRect(
                    x1,
                    y1,
                    std::max(1, x2 - x1),
                    std::max(1, y2 - y1),
                    alternate ? 244 : 252,
                    alternate ? 247 : 253,
                    alternate ? 250 : 255
            );
        }
    }

    auto gridX = [&](int c) {
        return originX + std::min((int) pxW - 1, (int) std::round(c * blockW));
    };
    auto gridY = [&](int r) {
        return originY + std::min((int) pxH - 1, (int) std::round(r * blockH));
    };

    for (int c = 0; c <= this->maxX; ++c)
        this->renderer->drawLine(gridX(c), originY, gridX(c), originY + (int) pxH - 1, 188, 194, 202);
    for (int r = 0; r <= this->maxY; ++r)
        this->renderer->drawLine(originX, gridY(r), originX + (int) pxW - 1, gridY(r), 188, 194, 202);

    this->renderer->fillRect(originX, originY, (int) pxW, 2, 112, 119, 129);
    this->renderer->fillRect(originX, originY + (int) pxH - 2, (int) pxW, 2, 112, 119, 129);
    this->renderer->fillRect(originX, originY, 2, (int) pxH, 112, 119, 129);
    this->renderer->fillRect(originX + (int) pxW - 2, originY, 2, (int) pxH, 112, 119, 129);

    if (this->statusRailVisible)
        this->renderer->fillRect(originX, originY, (int) pxW, 8, this->statusR, this->statusG, this->statusB);

    //convert x / y coordinates from grid to pixel  ; done every refresh.
    //each rescaled sprite is drawn immediately, preserving scene order while
    //allowing built-in vector pieces and texture-backed sprites to mix freely.
    std::vector<Sprite*> single(1);
    for (auto *ptr : this->renderList) {
        //convert grid coord --> pixel coord
        double xcoord = originX + ptr->getX() * blockW;
        double ycoord = originY + ptr->getY() * blockH;
        //make a new sprite with correct coordinates
        Sprite scaled(
                ptr->getTextureLocation(),
                xcoord,
                ycoord,
                blockW * ptr->getWidth(),
                blockH * ptr->getHeight()
        );
        if (VectorDrawing::drawSprite(*this->renderer, &scaled))
            continue;
        single[0] = &scaled;
        this->renderer->draw(single);
    }
  if (this->overlay)
    this->overlay(*this->renderer);
  this->renderer->present();
  this->renderList.clear();
}

bool GridDrawingVisitor::isOpen() {
    return this->renderer->isOpen();
}

void GridDrawingVisitor::visit(Sprite* s) {
    this->renderList.push_back(s);
}
