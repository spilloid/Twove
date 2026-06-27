#include "SDLInputWrapper.h"

namespace {
/**
 * @brief The scancodes we surface, paired with their neutral Key.
 *
 * A table keeps the translation declarative: adding a key is one row, not
 * another branch in a switch.
 */
struct KeyMapping {
    SDL_Scancode scancode;
    Key key;
};

const KeyMapping kKeyTable[] = {
        {SDL_SCANCODE_UP, Key::Up},
        {SDL_SCANCODE_DOWN, Key::Down},
        {SDL_SCANCODE_LEFT, Key::Left},
        {SDL_SCANCODE_RIGHT, Key::Right},
        {SDL_SCANCODE_SPACE, Key::Space},
        {SDL_SCANCODE_RETURN, Key::Enter},
        {SDL_SCANCODE_ESCAPE, Key::Escape},
        {SDL_SCANCODE_W, Key::W},
        {SDL_SCANCODE_A, Key::A},
        {SDL_SCANCODE_S, Key::S},
        {SDL_SCANCODE_D, Key::D},
};
} // namespace

SDLInputWrapper::SDLInputWrapper(SDL_Window *window) : window(window) {}

std::vector<Key> SDLInputWrapper::getKeyPresses() {
    std::vector<Key> pressed;
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    for (const auto &mapping : kKeyTable) {
        if (state[mapping.scancode])
            pressed.push_back(mapping.key);
    }
    return pressed;
}

click SDLInputWrapper::getLastClick() {
    // SDL on the web maps touch to mouse events, so the same path serves both.
    // A press held longer than this reads as a long-press (used for touch).
    const Uint32 LONG_PRESS_MS = 400;

    int x = 0, y = 0;
    Uint32 buttons = SDL_GetMouseState(&x, &y);
    bool down = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

    Uint32 now = SDL_GetTicks();
    if (down && !this->wasDown)
        this->pressStartTicks = now; // press just began
    bool isLong = down && (now - this->pressStartTicks >= LONG_PRESS_MS);
    this->wasDown = down;

    return click{x, y, down ? 1 : 0, isLong ? 1 : 0};
}
