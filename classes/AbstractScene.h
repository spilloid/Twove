#ifndef ABSTRACT_SCENE_H
#define ABSTRACT_SCENE_H

#include "Sprite.h"
#include "Visitor.h"
#include <list>
#include <memory>

//TODO: implement a composite scene.
//TODO: BSP dungeon Generation ; andy's website
class AbstractScene {
private:
public:
    /**
     * @brief add a sprite to the scene
     *
     * @param s
     */
    virtual void addSprite(std::shared_ptr<Sprite> s) = 0;

    /**
     * @brief iterate through sprites in scene letting each visitor visit sprites
     *
     * @param v
     */
    virtual void accept(Visitor* v) = 0;
};
#endif