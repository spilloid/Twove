#ifndef ABSTRACT_RENDERER_H
#define ABSTRACT_RENDERER_H

#include <vector>
#include <memory>
#include "Sprite.h"

class AbstractRenderer {
protected:
    bool open{};
    unsigned int screenHeight{};
    unsigned int screenWidth{};
public:
    virtual unsigned int getHeight();

    virtual unsigned int getWidth();

    virtual bool isOpen() = 0;

    virtual void draw(const std::vector<Sprite*>& renderList) = 0;
};
#endif // !1RENDERER_H