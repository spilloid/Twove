#include "GameEngine.h"

#include <utility>

GameEngine::GameEngine() {
  this->scene = std::make_shared<SimpleScene>();

}

GameEngine::~GameEngine() = default;

void GameEngine::addVisitor(const std::shared_ptr<Visitor> &v) {
  this->sceneVisitors.push_back(v);
}

void GameEngine::addSprite(std::shared_ptr<Sprite> s) {
  this->scene->addSprite(std::move(s));
}

void GameEngine::setScene(std::shared_ptr<AbstractScene> as) {
  this->scene = std::move(as);
}

void GameEngine::update() {
  // operating on scene
  for (auto &sceneVisitor : this->sceneVisitors)
    this->scene->accept(sceneVisitor.get()); // end if ; check interval
} // eof