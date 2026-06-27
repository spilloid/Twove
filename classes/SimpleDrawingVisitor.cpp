#include "SimpleDrawingVisitor.h"

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
  this->renderer->draw(this->renderList);
  this->renderer->present();
  this->renderList.clear();
}