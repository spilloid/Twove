#include "Pong.h"

void Pong::start()
{
    //constants
    const int MAXX = 500, MINX = 0, MAXY = 500, MINY = 0,
            paddleHeight = 50, paddleWidth = 10, PADDLESPRING = 20;
    const std::string black = "./assets/img/bbox.png";
    //declare sprites : texture, (x,y), (width,height)
    std::shared_ptr<Sprite> player1 = std::make_shared<Sprite>(
            black, MINX + 100, MAXY / 2, paddleWidth, paddleHeight);
    std::shared_ptr<Sprite> player2 = std::make_shared<Sprite>(
            black, MAXX - 100, MAXY / 2, paddleWidth, paddleHeight);
    std::shared_ptr<Sprite> player1Goal = std::make_shared<Sprite>(
            black, MINX + 10, 0, 5, MAXY * 2);
    std::shared_ptr<Sprite> player2Goal = std::make_shared<Sprite>(
            black, MAXX - 10, 0, 5, MAXY * 2);
    std::shared_ptr<Sprite> ball = std::make_shared<Sprite>(
            black, MAXX / 2, MAXY / 2, 10, 10);
    //open a window via the build-time backend (SDL2, SFML, ...)
    std::shared_ptr<Backend> backend = createDefaultBackend();
    Platform platform = backend->create(MAXX, MAXY, "Pong");
    std::shared_ptr<AbstractRenderer> ar = platform.renderer;
    //declare visitors
    std::shared_ptr<BounceBoundsVisitor> bv = std::make_shared<BounceBoundsVisitor>(MINX, MAXX, MINY, MAXY);
    std::shared_ptr<ForceVisitor> fv = std::make_shared<ForceVisitor>();
    std::shared_ptr<BoundingBoxCollisionVisitor> bbcv = std::make_shared<BoundingBoxCollisionVisitor>();
    //raycast from the ball along its velocity -- lets the AI "see" the ball coming
    std::shared_ptr<RayCastCollisionVisitor> rcv = std::make_shared<RayCastCollisionVisitor>();
    std::shared_ptr<SimpleDrawingVisitor> draw = std::make_shared<SimpleDrawingVisitor>(ar);

    //declare game engine
    std::shared_ptr<GameEngine> ge = std::make_shared<GameEngine>();

    //input wrapper comes from the same backend, already wired to the window
    std::shared_ptr<AbstractInputWrapper> in = platform.input;

    //add sprites to scene
    ge->addSprite(player1);
    ge->addSprite(player2);
    ge->addSprite(player1Goal);
    ge->addSprite(player2Goal);
    ge->addSprite(ball);

    //tweak visitors
    bbcv->setWatched(ball.get());
    rcv->setWatched(ball.get());
    fv->applyForce(ball, 10, rand());

    //add visitors to scene
    ge->addVisitor(bbcv);
    ge->addVisitor(rcv);
    ge->addVisitor(fv);
    ge->addVisitor(bv);
    //gotta add the drawing visitor last
    ge->addVisitor(draw);

    //backend-neutral stopwatch (std::chrono under the hood)
    Clock tick;

    //start the game
    srand(time(nullptr));
    int p1points = 0, p2points = 0;
    while (draw->isOpen())
    {
        frameYield(16); // browser: yield to the event loop; native: no-op
        if (tick.getElapsedMilliseconds() > 50) {
            //update engine
            ge->update();
            //handle clock
            tick.restart();

            //handle keypress (Up/W move the paddle up, Down/S move it down)
            std::vector<Key> keys = in->getKeyPresses();
            if (keys.empty())
                fv->stop(player1);
            else
                for (Key &key : keys) {
                    switch (key) {
                        case Key::Up:
                        case Key::W:
                            fv->applyForce(player1, 1, 0);
                            break;
                        case Key::Down:
                        case Key::S:
                            fv->applyForce(player1, 1, 180);
                            break;
                        default:
                            break;
                    }
                }
            //handle collisions
            std::list<Sprite*> l = bbcv->getCollisions();
            for (auto &c : l) {
                bool reset = false;
                if (c == player1Goal.get()) {
                    p2points++;
                    //player 2 scores
                    reset = true;
                } else if (c == player2Goal.get()) {
                    p1points++;
                    //player 1 scores
                    reset = true;
                    //by golly, we hit a paddle
                } else if (c == player1.get()) {
                    //bounce off paddle
                    //push ball off to stop multiple collisions
                    ball->setXY(ball->getX() + paddleWidth, ball->getY());
                    ball->setDXY(
                            fabs(ball->getDX()),
                            PADDLESPRING * ((ball->getY() - player1->getY()) / paddleHeight - 0.5f)
                    );
                    std::cout << ball->getDX() << " " << ball->getDY() << std::endl;
                } else if (c == player2.get()) {
                    ball->setXY(ball->getX() - paddleWidth, ball->getY());
                    ball->setDXY(
                            -fabs(ball->getDX()),
                            PADDLESPRING * ((ball->getY() - player2->getY()) / paddleHeight - 0.5f)
                    );
                    std::cout << ball->getDX() << " " << ball->getDY() << std::endl;
                }
                //game over? next round!
                if (reset) {
                    ball->setXY(MAXX / 2, MAXY / 2);
                    ball->setDXY(0, 0);
                    player1->setXY(player1->getX(), MAXY / 2);
                    player2->setXY(player2->getX(), MAXY / 2);
                    fv->applyForce(ball, 10, rand()%45+90 );
                    std::cout << "POINT! Score : Player 1: " << p1points << " | Player 2: " << p2points << std::endl;
                    //silly wait to pause game shortly (yielding so the browser stays alive)
                    while (tick.getElapsedSeconds() < 2) frameYield(16);
                    //start the next round
                }
            }
            //raycast "AI": the ray from the ball hits player2Goal only when the
            //ball is actually heading toward player 2's side. Chase only then;
            //otherwise drift back to centre. That makes the AI beatable -- catch
            //it out of position and you can score.
            bool incoming = false;
            for (auto *hit : rcv->getCollisions())
                if (hit == player2Goal.get()) incoming = true;
            double aiTarget = incoming ? ball->getY() - paddleHeight / 2.0 - 20
                                       : MAXY / 2.0 - paddleHeight / 2.0;
            player2->setXY(player2->getX(), aiTarget);

            draw->draw();
        }
    }
    std::cout << "PONG CLOSED" << std::endl;
}
