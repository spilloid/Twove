#ifndef VECTOR_DRAWING_H
#define VECTOR_DRAWING_H

#include "AbstractRenderer.h"
#include "Sprite.h"

namespace VectorSprites {
inline constexpr const char *Blank = "vector://blank";
inline constexpr const char *MarkX = "vector://mark/x";
inline constexpr const char *MarkO = "vector://mark/o";
inline constexpr const char *PawnBlue = "vector://pawn/blue";
inline constexpr const char *PawnRed = "vector://pawn/red";
inline constexpr const char *TurnBlue = "vector://turn/blue";
inline constexpr const char *TurnRed = "vector://turn/red";
inline constexpr const char *Wall = "vector://wall";
inline constexpr const char *WallGhost = "vector://wall/ghost";
inline constexpr const char *PongCourt = "vector://pong/court";
inline constexpr const char *PongPaddle = "vector://pong/paddle";
inline constexpr const char *PongBall = "vector://pong/ball";
inline constexpr const char *PongNet = "vector://pong/net";
inline constexpr const char *PongGoalLeft = "vector://pong/goal-left";
inline constexpr const char *PongGoalRight = "vector://pong/goal-right";
}

namespace VectorDrawing {
bool drawSprite(AbstractRenderer &renderer, Sprite *sprite);
}

#endif
