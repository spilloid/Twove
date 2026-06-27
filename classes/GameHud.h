#ifndef GAME_HUD_H
#define GAME_HUD_H

#include "AbstractRenderer.h"
#include <string>

namespace GameHud {
inline constexpr int BarHeight = 58;

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

Rect replayButton(unsigned int screenWidth);
bool contains(const Rect &rect, int x, int y);
bool isReplayClick(int x, int y, unsigned int screenWidth);
int textWidth(const std::string &text, int scale);
void drawText(AbstractRenderer &renderer, const std::string &text, int x, int y, int scale, Color color);
void drawTopBar(AbstractRenderer &renderer, unsigned int screenWidth, const std::string &title,
                const std::string &status, const std::string &score);
void drawResultBanner(AbstractRenderer &renderer, unsigned int screenWidth, int centerY,
                      const std::string &message, Color accent);
}

#endif
