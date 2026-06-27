#ifndef ABSTRACTDRAWINGVISITOR_H
#define ABSTRACTDRAWINGVISITOR_H
#include "Visitor.h"
#include "AbstractRenderer.h"
class AbstractDrawingVisitor : public Visitor{

    private:
    //you'll probably want a cache of some kind here for drawing
    protected:
        bool open;
        std::shared_ptr<AbstractRenderer> renderer;
    public: 
    /**
     * @brief Draw cache to screen
     * 
     */
    virtual void draw()=0;
    /**
     * @brief is the screen window open?
     * 
     * @return true 
     * @return false 
     */
    virtual bool isOpen() = 0;

    /**
     * @brief cache a sprite for later drawing
     * 
     * @param s 
     */
    void visit(Sprite* s) override = 0;
};
#endif // ABSTRACTDRAWINGVISITOR_H
