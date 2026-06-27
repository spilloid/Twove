#include "GameHud.h"

#include <algorithm>
#include <array>
#include <cctype>

namespace {
using Glyph = std::array<const char *, 7>;

const Glyph &glyphFor(char raw) {
    static const Glyph blank = {".....", ".....", ".....", ".....", ".....", ".....", "....."};
    static const Glyph a = {".###.", "#...#", "#...#", "#####", "#...#", "#...#", "#...#"};
    static const Glyph b = {"####.", "#...#", "#...#", "####.", "#...#", "#...#", "####."};
    static const Glyph c = {".####", "#....", "#....", "#....", "#....", "#....", ".####"};
    static const Glyph d = {"####.", "#...#", "#...#", "#...#", "#...#", "#...#", "####."};
    static const Glyph e = {"#####", "#....", "#....", "####.", "#....", "#....", "#####"};
    static const Glyph f = {"#####", "#....", "#....", "####.", "#....", "#....", "#...."};
    static const Glyph g = {".####", "#....", "#....", "#.###", "#...#", "#...#", ".###."};
    static const Glyph h = {"#...#", "#...#", "#...#", "#####", "#...#", "#...#", "#...#"};
    static const Glyph i = {"#####", "..#..", "..#..", "..#..", "..#..", "..#..", "#####"};
    static const Glyph j = {"..###", "...#.", "...#.", "...#.", "#..#.", "#..#.", ".##.."};
    static const Glyph k = {"#...#", "#..#.", "#.#..", "##...", "#.#..", "#..#.", "#...#"};
    static const Glyph l = {"#....", "#....", "#....", "#....", "#....", "#....", "#####"};
    static const Glyph m = {"#...#", "##.##", "#.#.#", "#...#", "#...#", "#...#", "#...#"};
    static const Glyph n = {"#...#", "##..#", "#.#.#", "#..##", "#...#", "#...#", "#...#"};
    static const Glyph o = {".###.", "#...#", "#...#", "#...#", "#...#", "#...#", ".###."};
    static const Glyph p = {"####.", "#...#", "#...#", "####.", "#....", "#....", "#...."};
    static const Glyph q = {".###.", "#...#", "#...#", "#...#", "#.#.#", "#..#.", ".##.#"};
    static const Glyph r = {"####.", "#...#", "#...#", "####.", "#.#..", "#..#.", "#...#"};
    static const Glyph s = {".####", "#....", "#....", ".###.", "....#", "....#", "####."};
    static const Glyph t = {"#####", "..#..", "..#..", "..#..", "..#..", "..#..", "..#.."};
    static const Glyph u = {"#...#", "#...#", "#...#", "#...#", "#...#", "#...#", ".###."};
    static const Glyph v = {"#...#", "#...#", "#...#", "#...#", "#...#", ".#.#.", "..#.."};
    static const Glyph w = {"#...#", "#...#", "#...#", "#.#.#", "#.#.#", "##.##", "#...#"};
    static const Glyph x = {"#...#", "#...#", ".#.#.", "..#..", ".#.#.", "#...#", "#...#"};
    static const Glyph y = {"#...#", "#...#", ".#.#.", "..#..", "..#..", "..#..", "..#.."};
    static const Glyph z = {"#####", "....#", "...#.", "..#..", ".#...", "#....", "#####"};
    static const Glyph zero = {".###.", "#...#", "#..##", "#.#.#", "##..#", "#...#", ".###."};
    static const Glyph one = {"..#..", ".##..", "..#..", "..#..", "..#..", "..#..", ".###."};
    static const Glyph two = {".###.", "#...#", "....#", "...#.", "..#..", ".#...", "#####"};
    static const Glyph three = {"####.", "....#", "....#", ".###.", "....#", "....#", "####."};
    static const Glyph four = {"#...#", "#...#", "#...#", "#####", "....#", "....#", "....#"};
    static const Glyph five = {"#####", "#....", "#....", "####.", "....#", "....#", "####."};
    static const Glyph six = {".###.", "#....", "#....", "####.", "#...#", "#...#", ".###."};
    static const Glyph seven = {"#####", "....#", "...#.", "..#..", ".#...", ".#...", ".#..."};
    static const Glyph eight = {".###.", "#...#", "#...#", ".###.", "#...#", "#...#", ".###."};
    static const Glyph nine = {".###.", "#...#", "#...#", ".####", "....#", "....#", ".###."};
    static const Glyph dash = {".....", ".....", ".....", "#####", ".....", ".....", "....."};
    static const Glyph colon = {".....", "..#..", "..#..", ".....", "..#..", "..#..", "....."};
    static const Glyph bang = {"..#..", "..#..", "..#..", "..#..", "..#..", ".....", "..#.."};
    static const Glyph slash = {"....#", "...#.", "...#.", "..#..", ".#...", ".#...", "#...."};

    switch (std::toupper(static_cast<unsigned char>(raw))) {
        case 'A': return a; case 'B': return b; case 'C': return c; case 'D': return d;
        case 'E': return e; case 'F': return f; case 'G': return g; case 'H': return h;
        case 'I': return i; case 'J': return j; case 'K': return k; case 'L': return l;
        case 'M': return m; case 'N': return n; case 'O': return o; case 'P': return p;
        case 'Q': return q; case 'R': return r; case 'S': return s; case 'T': return t;
        case 'U': return u; case 'V': return v; case 'W': return w; case 'X': return x;
        case 'Y': return y; case 'Z': return z;
        case '0': return zero; case '1': return one; case '2': return two; case '3': return three;
        case '4': return four; case '5': return five; case '6': return six; case '7': return seven;
        case '8': return eight; case '9': return nine;
        case '-': return dash; case ':': return colon; case '!': return bang; case '/': return slash;
        default: return blank;
    }
}

void fill(AbstractRenderer &renderer, const GameHud::Rect &rect, GameHud::Color color) {
    renderer.fillRect(rect.x, rect.y, rect.w, rect.h, color.r, color.g, color.b);
}

void border(AbstractRenderer &renderer, const GameHud::Rect &rect, GameHud::Color color) {
    renderer.fillRect(rect.x, rect.y, rect.w, 1, color.r, color.g, color.b);
    renderer.fillRect(rect.x, rect.y + rect.h - 1, rect.w, 1, color.r, color.g, color.b);
    renderer.fillRect(rect.x, rect.y, 1, rect.h, color.r, color.g, color.b);
    renderer.fillRect(rect.x + rect.w - 1, rect.y, 1, rect.h, color.r, color.g, color.b);
}
} // namespace

GameHud::Rect GameHud::replayButton(unsigned int screenWidth) {
    return Rect{(int) screenWidth - 94, 12, 84, 34};
}

bool GameHud::contains(const Rect &rect, int x, int y) {
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}

bool GameHud::isReplayClick(int x, int y, unsigned int screenWidth) {
    return contains(replayButton(screenWidth), x, y);
}

int GameHud::textWidth(const std::string &text, int scale) {
    if (text.empty())
        return 0;
    return (int) text.size() * 6 * scale - scale;
}

void GameHud::drawText(AbstractRenderer &renderer, const std::string &text, int x, int y,
                       int scale, Color color) {
    int cursor = x;
    for (char ch : text) {
        const Glyph &glyph = glyphFor(ch);
        for (int row = 0; row < 7; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (glyph[row][col] == '#')
                    renderer.fillRect(cursor + col * scale, y + row * scale, scale, scale,
                                      color.r, color.g, color.b);
            }
        }
        cursor += 6 * scale;
    }
}

void GameHud::drawTopBar(AbstractRenderer &renderer, unsigned int screenWidth,
                         const std::string &title, const std::string &status,
                         const std::string &score) {
    Rect bar{0, 0, (int) screenWidth, BarHeight};
    Rect button = replayButton(screenWidth);
    fill(renderer, bar, Color{14, 18, 28});
    renderer.fillRect(0, BarHeight - 2, (int) screenWidth, 2, 50, 58, 75);
    renderer.fillRect(0, 0, (int) screenWidth, 4, 39, 98, 255);

    drawText(renderer, title, 10, 8, 2, Color{236, 241, 249});
    drawText(renderer, status, 10, 33, 2, Color{143, 201, 255});

    int scoreX = std::max(10, button.x - 12 - textWidth(score, 2));
    drawText(renderer, score, scoreX, 33, 2, Color{224, 229, 238});

    fill(renderer, button, Color{33, 91, 221});
    border(renderer, button, Color{163, 205, 255});
    drawText(renderer, "REPLAY", button.x + 6, button.y + 10, 2, Color{255, 255, 255});
}

void GameHud::drawResultBanner(AbstractRenderer &renderer, unsigned int screenWidth,
                               int centerY, const std::string &message, Color accent) {
    int scale = 3;
    int textW = textWidth(message, scale);
    Rect panel{std::max(12, ((int) screenWidth - textW - 36) / 2), centerY - 28,
               std::min((int) screenWidth - 24, textW + 36), 58};
    fill(renderer, Rect{panel.x + 3, panel.y + 4, panel.w, panel.h}, Color{25, 30, 42});
    fill(renderer, panel, Color{248, 250, 252});
    border(renderer, panel, accent);
    renderer.fillRect(panel.x, panel.y, panel.w, 5, accent.r, accent.g, accent.b);
    drawText(renderer, message, panel.x + (panel.w - textW) / 2, panel.y + 20, scale, Color{17, 24, 39});
}
