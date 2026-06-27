#ifndef ABSTRACTCOLLISIONVISITOR_H
#define ABSTRACTCOLLISIONVISITOR_H
#include "Visitor.h"
#include <list>
/**
 * @brief Provides an abstraction for handling visitor based collisions where algorithm is separate from visitation
 * 
 */
class AbstractCollisionVisitor : public Visitor{
    protected:
    /**
     * @brief list of sprites colliding with watched since last check
     * 
     */
    std::list<Sprite*> collisions;
    /**
     * @brief sprite to watch and compare for collisions (non-owning)
     *
     */
    Sprite* watched = nullptr;
    public:
    /**
     * @brief update currently colliding list based on algorithm
     *
     * @param s
     */
    void visit(Sprite* s) override = 0;

    /**
     * @brief
     *
     * @return std::list<Sprite*> list of all sprites that collided with watched since last check
     */
    std::list<Sprite*> getCollisions();

    /**
     * @brief Set the Watched object
     *
     * @param s
     */
    void setWatched(Sprite* s);

};
#endif