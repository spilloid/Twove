#ifndef GRIDDRAWINGVISITOR_H
#define GRIDDRAWINGVISITOR_H
#include "AbstractDrawingVisitor.h"
class GridDrawingVisitor : public AbstractDrawingVisitor
{
private:
    /**
     * @brief maximum number of squares in X direction
     * 
     */
    int maxX;
    /**
     * @brief maximum number of squares in Y direction
     * 
     */
    int maxY;
    std::vector<Sprite*> renderList;
    bool statusRailVisible = false;
    unsigned char statusR = 0;
    unsigned char statusG = 0;
    unsigned char statusB = 0;
public:
/**
 * @brief Construct a new Grid Drawing Visitor object
 * 
 * @param maxX  : size of grid for X direction
 * @param maxY : size of grid for Y direction
 * @param ar : abstract renderer
 */
    GridDrawingVisitor(int maxX, int maxY, std::shared_ptr<AbstractRenderer> ar);

    /**
     * @brief Show a slim coloured rail at the top of the grid, useful for
     *        lightweight state such as the active Quoridor player.
     */
    void setStatusRail(unsigned char r, unsigned char g, unsigned char b);

    /**
     * @brief Hide the optional status rail.
     */
    void clearStatusRail();

    /**
     * @brief Draw cache to screen
     * 
     */
    void draw() override;

    /**
     * @brief is the screen window open?
     * 
     * @return true 
     * @return false 
     */
    bool isOpen() override;

    /**
     * @brief cache a sprite for later drawing
     * 
     * @param s 
     */
    void visit(Sprite* s) override;
};
#endif
