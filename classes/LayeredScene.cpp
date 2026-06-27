#include "LayeredScene.h"

LayeredScene::LayeredScene() = default;

LayeredScene::~LayeredScene() = default;

void LayeredScene::addSprite(std::shared_ptr<Sprite> s) {
    this->addSprite(s, 0);
}

void LayeredScene::addSprite(const std::shared_ptr<Sprite> &s, int layeredNum) {
    this->spriteList[layeredNum].emplace_back(s);
}

void LayeredScene::accept(Visitor* v) {
    //iterate over the list of lists in order
    for (auto &i : this->spriteList) {
        for (auto &j : i) {
            v->visit(j.get());
        }
    }
}