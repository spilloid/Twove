#include "SimpleDrawingVisitor.h"

#include "VectorDrawing.h"

#include <utility>

SimpleDrawingVisitor::SimpleDrawingVisitor(std::shared_ptr<AbstractRenderer> ar) {
    this->open = true;
    this->renderer = std::move(ar);
}

void SimpleDrawingVisitor::visit(Sprite* s) {
    this->renderList.push_back(s);
}

bool SimpleDrawingVisitor::isOpen() { return this->renderer->isOpen(); }

void SimpleDrawingVisitor::draw() {
  this->renderer->clear();
  std::vector<Sprite*> single(1);
  for (auto *sprite : this->renderList) {
    if (VectorDrawing::drawSprite(*this->renderer, sprite))
      continue;
    single[0] = sprite;
    this->renderer->draw(single);
  }
  this->renderer->present();
  this->renderList.clear();
}
