#ifndef LAYEREDSCENE_H
#define LAYEREDSCENE_H
#include "AbstractScene.h"
#include <vector>
class LayeredScene : public AbstractScene{
    private:
    /**
     * @brief vector of vectors for setting layered scene
     * 
     */
    std::vector<std::vector<std::shared_ptr<Sprite>>> spriteList;
    public:
    /**
     * @brief Construct a new Scene object
     * 
     */
        LayeredScene();

    /**
     * @brief Destroy the Scene object
     *
     */
    ~LayeredScene();

    /**
     * @brief add a sprite to the scene
     *
     * @param s
     */
    void addSprite(std::shared_ptr<Sprite> s) override;

    void addSprite(const std::shared_ptr<Sprite> &s, int layerNumber);

    /**
     * @brief iterate through sprites in scene letting each visitor visit sprites
     *
     * @param v
     */
    void accept(Visitor* v) override;
};
#endif