

#ifndef PUZZLEUTIL_H_INCLUDED
#define PUZZLEUTIL_H_INCLUDED

#include "PuzzleNode.h"

namespace Util
{
    // Hash Function For Our Unordered Map Of Vectors.
    // Function Is Not Mine, Was Found Online.
    struct VectorHash{
        size_t operator()(const std::vector<int>& v) const {
            std::hash<int> hasher;
            size_t seed = 0;
            for (int i : v) {
                seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            }
            return seed;
        }
    };

    // Our Set Of Known States.
    extern std::unordered_set<std::vector<int>, VectorHash> exploredSet;
    // Our Goal State.
    extern std::vector<int> goalState;

    // Goal State Creation. Blank Space In Upper Left Corner
    // Followed By 1 To n-1.
    void createGoal(int n)
    {
        goalState.push_back(0);
        for(int i = 0; i < n*n-1; ++i)
            goalState.push_back(i+1);
    }

    // Check If State Already Exists To Avoid Duplicates.
    bool isExplored(const PuzzleState &st)
    {
        if(exploredSet.count(st.board) > 0)
            return true;
        return false;
    }
};

#endif
