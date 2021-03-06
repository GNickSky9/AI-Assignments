

#include <fstream>
#include <cmath>
#include <queue>
#include <stack>
#include <chrono>
#include <unordered_set>
#include "PuzzleUtil.h"

using namespace Util;

std::unordered_set<std::vector<int>, VectorHash> Util::exploredSet;
std::vector<int> Util::goalState;

// Implementation Of Misplaced Tiles Heuristic
int misplacedTiles(PuzzleNode *nd)
{
    int cnt = 0;
    for(int i = 0; i < nd->currState.board.size(); ++i)
    {
        if(nd->currState.board[i] == 0)
            continue;
        if(nd->currState.board[i] != goalState[i])
            ++cnt;
    }
    return cnt;
}

// Implementation Of Manhatten Distance Heuristic
int ManhattenDistance(PuzzleNode *nd)
{
    int sum = 0;
    int rowSize = nd->currState.getN();
    for(int i = 0; i < nd->currState.board.size(); ++i)
    {
        for(int j = 0; j < nd->currState.board.size(); ++j)
        {
            if(nd->currState.board[i] == goalState[j])
                sum += (std::abs(i/rowSize - j/rowSize) + std::abs(i%rowSize - j%rowSize));
        }
    }
    return sum;
}

// Compare Functor For Priority Queue In A* Search.
struct Compare
{
    bool operator() (PuzzleNode *left, PuzzleNode *right)
    {
        int lf = misplacedTiles(left) + left->getDepth();
        int rf = misplacedTiles(right) + right->getDepth();
        return lf < rf;
    }
};

// A* Search.
bool astrSolver(PuzzleNode *root, std::vector<PuzzleNode *> &path, int &qDepth, int &maxDepth)
{
    std::priority_queue<PuzzleNode *, std::vector<PuzzleNode *>, Compare> frontier;
    root->action = 's';
    root->f = 0;
    frontier.push(root);

    int x = -1;
    int cnt = 0;
    ++cnt;

    while(!frontier.empty())
    {
        PuzzleNode *temp = frontier.top();
        frontier.pop();
        exploredSet.insert(temp->getBoard());
        path.push_back(temp);
        if(temp->currState.board == goalState)
            return true;
        if(temp->moveUp())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()-(child.getN())];
            child.board[child.getPos()-(child.getN())] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()-child.getN());
            child.setRow(child.getZeroRow()-1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'U';
            temp->upPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }

        if(temp->moveLeft())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()-1];
            child.board[child.getPos()-1] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()-1);
            child.setCol(child.getZeroCol()-1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'L';
            temp->leftPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }

        if(temp->moveDown())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()+(child.getN())];
            child.board[child.getPos()+(child.getN())] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()+child.getN());
            child.setRow(child.getZeroRow()+1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'D';
            temp->downPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }

        if(temp->moveRight())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()+1];
            child.board[child.getPos()+1] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()+1);
            child.setCol(child.getZeroCol()+1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'R';
            temp->rightPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }
        ++cnt;
        maxDepth = cnt;
    }
}

// Depth First Search Solver.
bool dfsSolver(PuzzleNode *root, std::vector<PuzzleNode *> &path, int &qDepth, int &maxDepth)
{
    std::stack<PuzzleNode *> frontier;
    root->action = 's';
    frontier.push(root);

    int x = -1;
    int cnt = 0;
    ++cnt;

    while(!frontier.empty())
    {
        PuzzleNode *temp = frontier.top();
        frontier.pop();
        exploredSet.insert(temp->getBoard());
        path.push_back(temp);

        if(temp->moveUp())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()-(child.getN())];
            child.board[child.getPos()-(child.getN())] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()-child.getN());
            child.setRow(child.getZeroRow()-1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'U';
            temp->upPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }

        if(temp->moveLeft())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()-1];
            child.board[child.getPos()-1] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()-1);
            child.setCol(child.getZeroCol()-1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'L';
            temp->leftPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }

        if(temp->moveDown())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()+(child.getN())];
            child.board[child.getPos()+(child.getN())] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()+child.getN());
            child.setRow(child.getZeroRow()+1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'D';
            temp->downPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }

        if(temp->moveRight())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()+1];
            child.board[child.getPos()+1] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()+1);
            child.setCol(child.getZeroCol()+1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'R';
            temp->rightPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }
        ++cnt;
        maxDepth = cnt;

    }
}

// Breadth First Search Solver.
bool bfsSolver2(PuzzleNode *root, std::vector<PuzzleNode *> &path, int &qDepth, int &maxDepth)
{
    std::queue<PuzzleNode *> frontier;
    root->action = 's';
    frontier.push(root);

    int x = -1;
    int cnt = 0;
    ++cnt;

    while(!frontier.empty())
    {
        PuzzleNode *temp = frontier.front();
        frontier.pop();
        exploredSet.insert(temp->getBoard());
        path.push_back(temp);

        if(temp->moveUp())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()-(child.getN())];
            child.board[child.getPos()-(child.getN())] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()-child.getN());
            child.setRow(child.getZeroRow()-1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'U';
            temp->upPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }

        if(temp->moveLeft())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()-1];
            child.board[child.getPos()-1] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()-1);
            child.setCol(child.getZeroCol()-1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'L';
            temp->leftPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }

        if(temp->moveDown())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()+(child.getN())];
            child.board[child.getPos()+(child.getN())] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()+child.getN());
            child.setRow(child.getZeroRow()+1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'D';
            temp->downPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }

        if(temp->moveRight())
        {
            PuzzleState child(temp->currState);
            x = child.board[child.getPos()+1];
            child.board[child.getPos()+1] = 0;
            child.board[child.getPos()] = x;
            child.setPos(child.getPos()+1);
            child.setCol(child.getZeroCol()+1);
            PuzzleNode *chNode = new PuzzleNode(child,temp);
            chNode->action = 'R';
            temp->rightPtr = chNode;

            if(!isExplored(child))
            {
                if(child.getBoard() == goalState)
                {
                    qDepth = frontier.size();
                    path.push_back(chNode);
                    return true;
                }
                else
                    frontier.push(chNode);
            }
            else
                delete chNode;
        }
        ++cnt;
        maxDepth = cnt;

    }
}

using get_time = std::chrono::steady_clock;

// Assume Our Given Puzzle Is Solvable, We
// Have No Check For If A Puzzle Is Solvable.
int main()
{
    std::ifstream input("puzzle.txt");      // Open File
    if(!input) throw -1;                    // Check If File Opened
    std::vector<int> values;                // Our Data Set/Initial State, ex: 1 2 5 3 4 0 6 7 8
    int val;
    int cnt = 0;                            // Count To Help Find Position Of Blank/Zero
    int pos = -1;                           // Position Of Blank/Zero
    while(input >> val)                     // Read In The Data
    {
        if(val == 0)
            pos = cnt;
        values.push_back(val);
        ++cnt;
    }

    int n = sqrt(values.size());            // Our n x n Size
    PuzzleState initial(n,values);          // Create An Initial State
    initial.setPos(pos);                    // Set Position Of Blank
    initial.findRowCol();                   // Find Row And Column Of Blank

    createGoal(n);                          // Create The Goal State
    PuzzleNode root(initial);               // Create Root Node
    PuzzleNode *ptr = &root;

    std::vector<PuzzleNode*> path;          // Our Path From Root To Goal
    int qDepth = 0;                         // Depth Of The Queue/Stack
    int maxDepth = 0;                       // Max Depth Of The Queue/Stack

    int usrInp = -1;                        // Get User Input To Choose What Search
    std::cout << "What Search Would You Like To Use?\n(1) BFS\n(2) DFS\n(3) A*\n";
    std::cin >> usrInp;

    if(usrInp == 1)
    {
        auto start = get_time::now();                       // Start Timer Before Search
        bfsSolver2(ptr,path,qDepth,maxDepth);
        auto end = get_time::now();
        auto diff = end - start;
        std::cout << "\n\nElapsed Time Of BFS Search: " <<  // Stop Timer After Search
        std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << "ms\n";
    }
    else if(usrInp == 2)
    {
        auto start = get_time::now();
        dfsSolver(ptr,path,qDepth,maxDepth);
        auto end = get_time::now();
        auto diff = end - start;
        std::cout << "\n\nElapsed Time Of DFS Search: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << "ms\n";
    }
    else if(usrInp == 3)
    {
        auto start = get_time::now();
        astrSolver(ptr,path,qDepth,maxDepth);
        auto end = get_time::now();
        auto diff = end - start;
        std::cout << "\n\nElapsed Time Of A* Search: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << "ms\n";
    }
    else
        throw -1;

    // Print Stats.
    std::cout << "Queue/Stack Depth: " << qDepth << "\n";
    std::cout << "Max. Queue/Stack Depth: " << maxDepth << "\n";
    std::cout << "Nodes Expanded: " << maxDepth << "\n";
    std::cout << "Total Path Cost: " << path.back()->getDepth() << "\n\n";

    std::vector<PuzzleNode *> realPath;     // Path From Goal To Initial State
    PuzzleNode *ptr2 = path.back();
    while(ptr2 != nullptr)
    {
        realPath.push_back(ptr2);
        ptr2 = ptr2->parent;
    }

    std::cout << "Path:" << "\n";
    /* Print Boards/States Of Nodes Along Path
    for(int i = realPath()-1; i >= 0; --i)
    {
        realPath[i]->currState.printBoard();
    }
    */
    // Print Actions As Characters Of Nodes Along Path
    for(int i = realPath.size()-2; i >= 0; --i)
    {
        std::cout << realPath[i]->action;
        std::cout << "\t->\t";
        if(i%6==0)
            std::cout << "\n";
    }

    return 0;
}
