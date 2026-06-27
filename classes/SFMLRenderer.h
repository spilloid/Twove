#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "AbstractRenderer.h"
#include "TextureFactory.h"
#include "Sprite.h"
#include <list>

class SFMLRenderer : public AbstractRenderer {
private:
    /**
 * @brief SFML window
 * 
 */
    std::shared_ptr<sf::RenderWindow> window;
    /**
       * @brief Factory for grabbing textures
       * 
       */
    std::shared_ptr<TextureFactory> tf;
    bool open;
public:
/**
 * @brief Construct a new SFMLRenderer object
 * 
 * @param screenWidth 
 * @param screenHeight 
 */
    SFMLRenderer(unsigned int screenWidth, unsigned int screenHeight);
    //TODO: PUT IN ABSTRACT

    /**
     *
     * @param renderList draws all items in render list to screen
     */
    void draw(const std::vector<Sprite*>& renderList) override;

    /**
     *
     * @return is the window open?
     */
    bool isOpen() override;

    /**
     * @brief Get the Window object
     *
     * @return std::shared_ptr<sf::Window>
     */
    std::shared_ptr<sf::RenderWindow> getWindow();

};

#endif // !1SFML_RENDERER_H