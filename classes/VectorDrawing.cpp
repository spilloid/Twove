#include "VectorDrawing.h"

#include <algorithm>
#include <cmath>
#include <string>

namespace {
int rounded(double value) {
    return static_cast<int>(std::round(value));
}

int positive(double value) {
    return std::max(1, rounded(value));
}

void drawBorder(AbstractRenderer &r, int x, int y, int w, int h,
                unsigned char red, unsigned char green, unsigned char blue) {
    r.fillRect(x, y, w, 1, red, green, blue);
    r.fillRect(x, y + h - 1, w, 1, red, green, blue);
    r.fillRect(x, y, 1, h, red, green, blue);
    r.fillRect(x + w - 1, y, 1, h, red, green, blue);
}

void drawThickLine(AbstractRenderer &r, int x1, int y1, int x2, int y2, int thickness,
                   unsigned char red, unsigned char green, unsigned char blue) {
    int half = std::max(1, thickness) / 2;
    for (int offset = -half; offset <= half; ++offset) {
        r.drawLine(x1 + offset, y1, x2 + offset, y2, red, green, blue);
        r.drawLine(x1, y1 + offset, x2, y2 + offset, red, green, blue);
    }
}

void drawThickCircle(AbstractRenderer &r, int cx, int cy, int radius, int thickness,
                     unsigned char red, unsigned char green, unsigned char blue) {
    int start = std::max(1, radius - thickness / 2);
    int end = std::max(start, radius + thickness / 2);
    for (int current = start; current <= end; ++current)
        r.drawCircle(cx, cy, current, red, green, blue);
}

void drawInsetRect(AbstractRenderer &r, int x, int y, int w, int h, int inset,
                   unsigned char red, unsigned char green, unsigned char blue) {
    r.fillRect(x + inset, y + inset, std::max(1, w - inset * 2), std::max(1, h - inset * 2),
               red, green, blue);
}

void drawMarkX(AbstractRenderer &r, int x, int y, int w, int h) {
    int inset = std::max(8, std::min(w, h) / 7);
    int thick = std::max(5, std::min(w, h) / 10);
    drawThickLine(r, x + inset + 2, y + inset + 3, x + w - inset + 2, y + h - inset + 3,
                  thick, 33, 37, 50);
    drawThickLine(r, x + w - inset + 2, y + inset + 3, x + inset + 2, y + h - inset + 3,
                  thick, 33, 37, 50);
    drawThickLine(r, x + inset, y + inset, x + w - inset, y + h - inset,
                  thick, 35, 91, 255);
    drawThickLine(r, x + w - inset, y + inset, x + inset, y + h - inset,
                  thick, 35, 91, 255);
    drawThickLine(r, x + inset + thick / 2, y + inset, x + w / 2, y + h / 2,
                  std::max(2, thick / 3), 141, 197, 255);
}

void drawMarkO(AbstractRenderer &r, int x, int y, int w, int h) {
    int radius = std::max(4, std::min(w, h) / 2 - std::max(7, std::min(w, h) / 8));
    int thick = std::max(5, std::min(w, h) / 10);
    int cx = x + w / 2;
    int cy = y + h / 2;
    drawThickCircle(r, cx + 2, cy + 3, radius, thick, 42, 37, 45);
    drawThickCircle(r, cx, cy, radius, thick, 221, 64, 72);
    drawThickCircle(r, cx - 1, cy - 1, std::max(1, radius - thick / 2), 2, 255, 160, 151);
}

void drawPawn(AbstractRenderer &r, int x, int y, int w, int h,
              unsigned char red, unsigned char green, unsigned char blue) {
    int radius = std::max(4, std::min(w, h) / 2 - 2);
    int cx = x + w / 2;
    int cy = y + h / 2;
    r.fillCircle(cx + 2, cy + 3, radius, 47, 52, 64);
    r.fillCircle(cx, cy, radius, 23, 30, 45);
    r.fillCircle(cx, cy, std::max(1, radius - 4), red, green, blue);
    drawThickCircle(r, cx, cy, radius, 3, 17, 24, 39);
    r.fillCircle(cx - radius / 3, cy - radius / 3, std::max(2, radius / 5), 246, 249, 255);
}

void drawTurnRing(AbstractRenderer &r, int x, int y, int w, int h,
                  unsigned char red, unsigned char green, unsigned char blue) {
    int radius = std::max(4, std::min(w, h) / 2 - 2);
    int cx = x + w / 2;
    int cy = y + h / 2;
    drawThickCircle(r, cx, cy, radius, 5, red, green, blue);
    drawThickCircle(r, cx, cy, std::max(1, radius - 6), 2, 255, 255, 255);
}

void drawWall(AbstractRenderer &r, int x, int y, int w, int h) {
    r.fillRect(x + 2, y + 3, w, h, 45, 49, 59);
    r.fillRect(x, y, w, h, 22, 27, 34);
    drawInsetRect(r, x, y, w, h, 2, 52, 60, 73);
    r.fillRect(x + 2, y + 2, std::max(1, w - 4), std::max(1, h / 4), 96, 105, 123);
    drawBorder(r, x, y, w, h, 10, 14, 20);
}

void drawGhostWall(AbstractRenderer &r, int x, int y, int w, int h) {
    r.fillRectAlpha(x, y, w, h, 39, 98, 255, 128);
    drawBorder(r, x, y, w, h, 165, 205, 255);
    r.fillRectAlpha(x + 2, y + 2, std::max(1, w - 4), std::max(1, h - 4), 255, 255, 255, 64);
}

void drawPongCourt(AbstractRenderer &r, int x, int y, int w, int h) {
    r.fillRect(x, y, w, h, 14, 18, 26);
    r.fillRect(x + 8, y + 8, std::max(1, w - 16), std::max(1, h - 16), 20, 25, 36);
    drawBorder(r, x + 8, y + 8, std::max(1, w - 16), std::max(1, h - 16), 72, 82, 103);
}

void drawPongPaddle(AbstractRenderer &r, int x, int y, int w, int h) {
    r.fillRect(x + 2, y + 2, w, h, 4, 8, 16);
    r.fillRect(x, y, w, h, 225, 230, 239);
    r.fillRect(x + 2, y + 2, std::max(1, w - 4), std::max(1, h - 4), 39, 98, 255);
    r.fillRect(x + 2, y + 2, std::max(1, w / 3), std::max(1, h - 4), 126, 200, 255);
}

void drawPongBall(AbstractRenderer &r, int x, int y, int w, int h) {
    int radius = std::max(3, std::min(w, h) / 2);
    int cx = x + w / 2;
    int cy = y + h / 2;
    r.fillCircle(cx + 2, cy + 2, radius, 3, 7, 15);
    r.fillCircle(cx, cy, radius, 255, 255, 255);
    r.fillCircle(cx - radius / 3, cy - radius / 3, std::max(1, radius / 4), 152, 221, 255);
}

void drawPongNet(AbstractRenderer &r, int x, int y, int w, int h) {
    r.fillRect(x, y, w, h, 121, 132, 154);
    r.fillRect(x, y, std::max(1, w / 2), h, 231, 238, 249);
}

void drawPongGoal(AbstractRenderer &r, int x, int y, int w, int h, bool left) {
    r.fillRect(x, y, w, h, 8, 12, 20);
    r.fillRect(x + (left ? 0 : std::max(0, w - 2)), y, std::min(2, w), h,
               left ? 221 : 35, left ? 64 : 91, left ? 72 : 255);
}
} // namespace

bool VectorDrawing::drawSprite(AbstractRenderer &renderer, Sprite *sprite) {
    if (sprite == nullptr)
        return false;

    std::string id = sprite->getTextureLocation();
    int x = rounded(sprite->getX());
    int y = rounded(sprite->getY());
    int w = positive(sprite->getWidth());
    int h = positive(sprite->getHeight());

    if (id == VectorSprites::Blank)
        return true;
    if (id == VectorSprites::MarkX) {
        drawMarkX(renderer, x, y, w, h);
        return true;
    }
    if (id == VectorSprites::MarkO) {
        drawMarkO(renderer, x, y, w, h);
        return true;
    }
    if (id == VectorSprites::PawnBlue) {
        drawPawn(renderer, x, y, w, h, 41, 112, 255);
        return true;
    }
    if (id == VectorSprites::PawnRed) {
        drawPawn(renderer, x, y, w, h, 230, 70, 78);
        return true;
    }
    if (id == VectorSprites::TurnBlue) {
        drawTurnRing(renderer, x, y, w, h, 41, 112, 255);
        return true;
    }
    if (id == VectorSprites::TurnRed) {
        drawTurnRing(renderer, x, y, w, h, 230, 70, 78);
        return true;
    }
    if (id == VectorSprites::Wall) {
        drawWall(renderer, x, y, w, h);
        return true;
    }
    if (id == VectorSprites::WallGhost) {
        drawGhostWall(renderer, x, y, w, h);
        return true;
    }
    if (id == VectorSprites::PongCourt) {
        drawPongCourt(renderer, x, y, w, h);
        return true;
    }
    if (id == VectorSprites::PongPaddle) {
        drawPongPaddle(renderer, x, y, w, h);
        return true;
    }
    if (id == VectorSprites::PongBall) {
        drawPongBall(renderer, x, y, w, h);
        return true;
    }
    if (id == VectorSprites::PongNet) {
        drawPongNet(renderer, x, y, w, h);
        return true;
    }
    if (id == VectorSprites::PongGoalLeft) {
        drawPongGoal(renderer, x, y, w, h, true);
        return true;
    }
    if (id == VectorSprites::PongGoalRight) {
        drawPongGoal(renderer, x, y, w, h, false);
        return true;
    }

    return false;
}
