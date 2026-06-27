#ifndef SIMPLE_SCENE_H
#define SIMPLE_SCENE_H

#include "AbstractScene.h"

class SimpleScene : public AbstractScene {
private:
    /**
     * @brief list of sprites in scene. could easily be a different data structure
     * 
     */
    std::list<std::shared_ptr<Sprite>> spriteList;
public:
    /**
     * @brief Construct a new Scene object
     * 
     */
        SimpleScene();

    /**
     * @brief Destroy the Scene object
     *
     */
    ~SimpleScene();

    /**
     * @brief add a sprite to the scene
     *
     * @param s
     */
    void addSprite(std::shared_ptr<Sprite> s) override;

    /**
     * @brief iterate through sprites in scene letting each visitor visit sprites
     *
     * @param v
     */
    void accept(Visitor* v) override;
};
#endif