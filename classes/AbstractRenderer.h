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
    virtual ~AbstractRenderer() = default;

    virtual unsigned int getHeight();

    virtual unsigned int getWidth();

    virtual bool isOpen() = 0;

    /**
     * @brief Clear the frame to the background colour. Call once at the start
     *        of a frame, before any draw* calls.
     */
    virtual void clear() = 0;

    /**
     * @brief Present the composed frame to the screen. Call once at the end.
     */
    virtual void present() = 0;

    /**
     * @brief Blit every sprite in the list at its (x, y) rect. Does not clear
     *        or present — the caller frames the draw with clear()/present().
     */
    virtual void draw(const std::vector<Sprite*>& renderList) = 0;

    /**
     * @brief Draw a 1px line between two points in pixel space.
     */
    virtual void drawLine(int x1, int y1, int x2, int y2,
                          unsigned char r, unsigned char g, unsigned char b) = 0;

    /**
     * @brief Draw a 1px circle outline in pixel space.
     */
    virtual void drawCircle(int cx, int cy, int radius,
                            unsigned char r, unsigned char g, unsigned char b) = 0;

    /**
     * @brief Draw a filled circle in pixel space.
     */
    virtual void fillCircle(int cx, int cy, int radius,
                            unsigned char r, unsigned char g, unsigned char b) = 0;

    /**
     * @brief Draw a filled rectangle in pixel space (used for grid lines,
     *        Quoridor walls, and other primitive overlays).
     */
    virtual void fillRect(int x, int y, int w, int h,
                          unsigned char r, unsigned char g, unsigned char b) = 0;

    /**
     * @brief Draw a translucent filled rectangle in pixel space.
     */
    virtual void fillRectAlpha(int x, int y, int w, int h,
                               unsigned char r, unsigned char g, unsigned char b,
                               unsigned char a) = 0;
};
#endif // !1RENDERER_H
