#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include <SDL.h>
#include <memory>
#include <vector>

#include "AbstractRenderer.h"
#include "SDLRAII.h"
#include "SDLTextureFactory.h"
#include "Sprite.h"

/**
 * @brief SDL2 implementation of AbstractRenderer.
 *
 * Owns the SDL window + renderer (and, through them, the texture cache) for the
 * lifetime of the game via RAII smart pointers -- no destructor needed. Member
 * order matters: textures are released first, then the renderer, then the
 * window (members destruct in reverse declaration order). isOpen() doubles as
 * the per-frame event pump.
 */
class SDLRenderer : public AbstractRenderer {
private:
    sdl::WindowPtr window;
    sdl::RendererPtr sdlRenderer;
    std::unique_ptr<SDLTextureFactory> tf;
    bool open;

public:
    /**
     * @brief Construct a renderer that takes ownership of an SDL window/renderer.
     *
     * @param window       window to draw into (ownership transferred in)
     * @param sdlRenderer  renderer bound to that window (ownership transferred in)
     * @param screenWidth  window width in pixels
     * @param screenHeight window height in pixels
     */
    SDLRenderer(sdl::WindowPtr window, sdl::RendererPtr sdlRenderer,
                unsigned int screenWidth, unsigned int screenHeight);

    void clear() override;

    void present() override;

    /**
     * @brief Blit every sprite in the list to the window (no clear/present).
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

    /**
     * @brief Pump events and report whether the window is still open.
     */
    bool isOpen() override;

    /**
     * @brief Borrow the underlying window (used by SDLInputWrapper for mouse coords).
     */
    SDL_Window *getWindow();
};

#endif // SDL_RENDERER_H
