#ifndef SDL_INPUT_WRAPPER_H
#define SDL_INPUT_WRAPPER_H

#include <SDL.h>

#include "AbstractInputWrapper.h"

/**
 * @brief SDL2 implementation of AbstractInputWrapper.
 *
 * Reads the live keyboard/mouse snapshot SDL maintains (kept current by the
 * event pump in SDLRenderer::isOpen) and translates native scancodes into the
 * backend-neutral Key enum.
 */
class SDLInputWrapper : public AbstractInputWrapper {
private:
    SDL_Window *window;            // borrowed, owned by SDLRenderer
    Uint32 pressStartTicks = 0;    // when the current press began (ms)
    bool wasDown = false;          // left button state on the previous poll

public:
    explicit SDLInputWrapper(SDL_Window *window);

    std::vector<Key> getKeyPresses() override;

    click getLastClick() override;
};

#endif // SDL_INPUT_WRAPPER_H
