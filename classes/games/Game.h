#ifndef GAME_H
#define GAME_H
#include "../GameEngine.h"
#include "../Sprite.h"
#include "../GravityVisitor.h"
#include "../BounceBoundsVisitor.h"
#include "../WrapBoundsVisitor.h"
#include "../ForceVisitor.h"
#include "../BoundingBoxCollisionVisitor.h"
#include "../RayCastCollisionVisitor.h"
#include "../SimpleDrawingVisitor.h"
#include "../GridDrawingVisitor.h"
#include "../Backend.h"
#include "../Clock.h"
#include "../Yield.h"
#include <list>
#include <memory>
class Game{
    private:
    public:
        virtual ~Game() = default;
        virtual void start()=0;
};
#endif