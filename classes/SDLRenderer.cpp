#include "SDLRenderer.h"

SDLRenderer::SDLRenderer(sdl::WindowPtr window, sdl::RendererPtr sdlRenderer,
                         unsigned int screenWidth, unsigned int screenHeight)
        : window(std::move(window)), sdlRenderer(std::move(sdlRenderer)), open(true) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->tf = std::make_unique<SDLTextureFactory>(this->sdlRenderer.get());
}

void SDLRenderer::draw(const std::vector<Sprite*>& renderList) {
    SDL_Renderer *r = this->sdlRenderer.get();
    // 1. REFRESH SCREEN (white background, matching the SFML backend)
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderClear(r);

    // 2. blit each sprite, stretching its texture to the sprite's rect
    for (const auto &i : renderList) {
        SDL_Texture *texture = this->tf->getTexture(i->getTextureLocation());
        if (texture == nullptr)
            continue; // missing asset: skip rather than crash
        SDL_Rect dest;
        dest.x = static_cast<int>(i->getX());
        dest.y = static_cast<int>(i->getY());
        dest.w = static_cast<int>(i->getWidth());
        dest.h = static_cast<int>(i->getHeight());
        SDL_RenderCopy(r, texture, nullptr, &dest);
    }

    // 3. present
    SDL_RenderPresent(r);
}

bool SDLRenderer::isOpen() {
    // Drain the whole event queue each frame so we never miss a quit and so
    // SDL_GetKeyboardState (used by the input wrapper) stays current.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            this->open = false;
        } else if (event.type == SDL_KEYDOWN &&
                   event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            this->open = false;
        }
    }
    return this->open;
}

SDL_Window *SDLRenderer::getWindow() {
    return this->window.get();
}
