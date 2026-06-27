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

    void clear() override;

    void present() override;

    /**
     *
     * @param renderList draws all items in render list to screen
     */
    void draw(const std::vector<Sprite*>& renderList) override;

    void drawLine(int x1, int y1, int x2, int y2,
                  unsigned char r, unsigned char g, unsigned char b) override;

    void drawCircle(int cx, int cy, int radius,
                    unsigned char r, unsigned char g, unsigned char b) override;

    void fillCircle(int cx, int cy, int radius,
                    unsigned char r, unsigned char g, unsigned char b) override;

    void fillRect(int x, int y, int w, int h,
                  unsigned char r, unsigned char g, unsigned char b) override;

    void fillRectAlpha(int x, int y, int w, int h,
                       unsigned char r, unsigned char g, unsigned char b,
                       unsigned char a) override;

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
