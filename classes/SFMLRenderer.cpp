#include "SFMLRenderer.h"
SFMLRenderer::SFMLRenderer(unsigned int screenWidth, unsigned int screenHeight)
        : open(true) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->window =
            std::make_shared<sf::RenderWindow>(sf::VideoMode(screenWidth, screenHeight), "Game");

    this->tf = std::make_shared<TextureFactory>();
}

void SFMLRenderer::draw(const std::vector<Sprite*>& renderList) {
    // 1. REFRESH SCREEN
    this->window->clear(sf::Color::White);
    //iterate over sprite list
    for (const auto &i : renderList) {
        std::shared_ptr<sf::Sprite> temp = std::make_shared<sf::Sprite>();
        temp->setTexture(*this->tf->getTexture(i->getTextureLocation()));
        temp->setTextureRect(sf::IntRect(0, 0, i->getWidth(), i->getHeight()));
        temp->setPosition(i->getX(), i->getY());
        this->window->draw(*temp);
    }
    this->window->display();
}
bool SFMLRenderer::isOpen()
{
    if (this->window->isOpen())
    {
        sf::Event event{};
        // TODO: explore way to watch for events on window smarter....
        //perhaps multi-threading?
        if (this->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                this->window->close();
                this->open = false;
            } // end if; window is closed
        }     // end if ; check event poll
        return this->open;
    } else {
        return false;
    }

}

std::shared_ptr<sf::RenderWindow> SFMLRenderer::getWindow() {
    return this->window;
}

