

#ifndef PUZZLESTATE_H_INCLUDED
#define PUZZLESTATE_H_INCLUDED

#include <iostream>
#include <vector>

class PuzzleState final
{
    private:
            int pos;
            int n;
            int zeroCol;
            int zeroRow;
    public:
            std::vector<int> board;

            PuzzleState(int n, const std::vector<int> &board) : n(n), board(board){}

            int getN(){return n;}
            int getPos(){return pos;}
            int getZeroCol(){return zeroCol;}
            int getZeroRow(){return zeroRow;}
            std::vector<int> getBoard(){return board;}

            void setPos(int x){pos = x;}
            void setCol(int x){zeroCol = x;}
            void setRow(int x){zeroRow = x;}

            void printBoard();
            void findRowCol();

            bool operator==(const PuzzleState &rhs) const
            {
                if(rhs.board.size() != board.size())
                    return false;
                for(size_t i = 0; i < rhs.board.size(); ++i)
                    if(board[i] != rhs.board[i])
                        return false;
                return true;
            }
};

void PuzzleState::printBoard()
{
    for(size_t i = 0; i < board.size(); ++i)
    {
        if(i%n == 0)
           std::cout << "\n";
        std::cout << board[i] << " ";
    }
}

void PuzzleState::findRowCol()
{
    int pos = -1;

    for(size_t i = 0; i < board.size(); ++i)
        if(board[i] == 0)
            pos = i;

    zeroCol = (pos % n) + 1;

    std::vector<std::vector<int> > board2D;
    pos = 0;

    for(size_t i = 0; i < board.size(); ++i)
    {
        if(i % n == 0)
            board2D.resize(board2D.size()+1);
        board2D[i/n].push_back(board[i]);
    }

    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            if(board2D[j][i] != 0)
                ++pos;
            if(board2D[j][i] == 0)
            {
                j = 999999;
                i = 999999;
            }
        }
    }

    zeroRow = (pos % n) + 1;
}

#endif
