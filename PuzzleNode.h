

#ifndef PUZZLENODE_H_INCLUDED
#define PUZZLENODE_H_INCLUDED

#include "PuzzleState.h"

class PuzzleNode final
{
    private:
            int depth;
            int pathCost;
    public:
            PuzzleState currState;
            PuzzleNode *parent;
            PuzzleNode *leftPtr;
            PuzzleNode *rightPtr;
            PuzzleNode *upPtr;
            PuzzleNode *downPtr;
            char action;
            int f, g, h;

            PuzzleNode(PuzzleState st, PuzzleNode *par = nullptr) : currState(st), parent(par)
            {
                leftPtr = nullptr;
                rightPtr = nullptr;
                upPtr = nullptr;
                downPtr = nullptr;
                if(parent != nullptr)
                {
                    depth = parent->depth + 1;
                    pathCost = depth;
                }
                else
                {
                    depth = 0;
                    pathCost = 0;
                }
            }

            std::vector<int> getBoard(){return currState.getBoard();}

            bool moveUp()
            {
                if(currState.getZeroRow() != 1)
                    return true;
                return false;
            }

            bool moveDown()
            {
                if(currState.getZeroRow() != currState.getN())
                    return true;
                return false;
            }

            bool moveLeft()
            {
                if(currState.getZeroCol() != 1)
                    return true;
                return false;
            }

            bool moveRight()
            {
                if(currState.getZeroCol() != currState.getN())
                    return true;
                return false;
            }

           /* bool operator<(const PuzzleNode *rhs)
            {
                return f < rhs->f;
            }*/


            int getDepth(){return depth;}
            int getCost(){return pathCost;}
};

#endif
