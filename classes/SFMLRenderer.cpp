#include "SFMLRenderer.h"
SFMLRenderer::SFMLRenderer(unsigned int screenWidth, unsigned int screenHeight)
        : open(true) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->window =
            std::make_shared<sf::RenderWindow>(sf::VideoMode(screenWidth, screenHeight), "Game");

    this->tf = std::make_shared<TextureFactory>();
}

void SFMLRenderer::clear() {
    this->window->clear(sf::Color::White);
}

void SFMLRenderer::present() {
    this->window->display();
}

void SFMLRenderer::draw(const std::vector<Sprite*>& renderList) {
    //iterate over sprite list (no clear/present -- the caller frames the draw)
    for (const auto &i : renderList) {
        std::shared_ptr<sf::Sprite> temp = std::make_shared<sf::Sprite>();
        temp->setTexture(*this->tf->getTexture(i->getTextureLocation()));
        temp->setTextureRect(sf::IntRect(0, 0, i->getWidth(), i->getHeight()));
        temp->setPosition(i->getX(), i->getY());
        this->window->draw(*temp);
    }
}

void SFMLRenderer::drawLine(int x1, int y1, int x2, int y2,
                            unsigned char r, unsigned char g, unsigned char b) {
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), sf::Color(r, g, b)),
            sf::Vertex(sf::Vector2f(x2, y2), sf::Color(r, g, b)),
    };
    this->window->draw(line, 2, sf::Lines);
}

void SFMLRenderer::drawCircle(int cx, int cy, int radius,
                              unsigned char r, unsigned char g, unsigned char b) {
    if (radius <= 0)
        return;
    sf::CircleShape circle(radius);
    circle.setPosition(cx - radius, cy - radius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color(r, g, b));
    circle.setOutlineThickness(1);
    this->window->draw(circle);
}

void SFMLRenderer::fillCircle(int cx, int cy, int radius,
                              unsigned char r, unsigned char g, unsigned char b) {
    if (radius <= 0)
        return;
    sf::CircleShape circle(radius);
    circle.setPosition(cx - radius, cy - radius);
    circle.setFillColor(sf::Color(r, g, b));
    this->window->draw(circle);
}

void SFMLRenderer::fillRect(int x, int y, int w, int h,
                            unsigned char r, unsigned char g, unsigned char b) {
    sf::RectangleShape rect(sf::Vector2f(w, h));
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color(r, g, b));
    this->window->draw(rect);
}

void SFMLRenderer::fillRectAlpha(int x, int y, int w, int h,
                                 unsigned char r, unsigned char g, unsigned char b,
                                 unsigned char a) {
    sf::RectangleShape rect(sf::Vector2f(w, h));
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color(r, g, b, a));
    this->window->draw(rect);
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

