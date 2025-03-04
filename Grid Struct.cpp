#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
using std::cout, std::endl, std::vector, std::string, std::unique, std::chrono::milliseconds;

struct grid
{
    int size;
    vector<vector<bool>> board;

    int rightMost;
    int bottomMost;
    
    grid()
    {
        size = 1;
        board = {{false}};
        rightMost = 0;
        bottomMost = 0;
    }

    grid(int n)
    {
        size = n;
        vector<vector<bool>> newBoard;
        for(int i = 0; i < n; i++)
        {
            vector<bool> newRow;
            for(int j = 0; j < n; j++)
            {
                newRow.push_back(false);
            }
            newBoard.push_back(newRow);
        }
        board = newBoard;
        rightMost = 0;
        bottomMost = 0;
    }
};

grid expandGrid(grid& g);
grid shrinkToSize(grid& g);
void printGrid(grid& g);

int main()
{
    grid* newGridptr = new grid(2);
    grid newGrid = *newGridptr;
    expandGrid(newGrid);
    shrinkToSize(newGrid);
    printGrid(newGrid);
    //cout << "Enter Size of N-Ominoes to Generate: " << "\n";

/*     
    while(std::cin >> size)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        //vector<vector<vector<bool>>> solutions = generatePolyominoes(size);
        cout << "Polyominoes of Size: " << size << "\n";
        //cout << "Generated " << solutions.size() << " Solutions: " << "\n";
        //cout << solutionsToString(solutions);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<milliseconds>(t2-t1);
        cout << "time elapsed: " << elapsed.count() << " milliseconds" << "\n\n";
        cout << "Enter Size of N-Ominoes to Generate: " << "\n";
    }
     */
    return EXIT_SUCCESS;
}

grid expandGrid(grid& g)
{
    int n = g.size + 2;
    vector<vector<bool>> board;
    for(int i = 0; i < n; i++)
    {
        vector<bool> newRow;
        for(int j = 0; j < n; j++)
        {
            if(i > 0 && j > 0 && i < n - 1 && j < n - 1)
            {
                newRow.push_back(g.board[i-1][j-1]);
            }
            else
            {
                newRow.push_back(false);
            }
        }
        board.push_back(newRow);
    }
    g.board = board;
    g.size = n;
    return g;
}

grid shrinkToSize(grid& g)
{
    int n = g.size;
    g.board.erase(g.board.end() - 1);
    for(int i = 0; i < n; i++)
    {
        g.board[i].erase(g.board[i].end() - 1);
    }
    g.size--;
    return g;
}

void printGrid(grid& g)
{
    for(int i = 0; i < g.size; i++)
    {
        for(int j = 0; j < g.size; j++)
        {
            char piece = g.board[i][j] ? 'P' : ' ';
            cout << "[" << piece << "]";
        }
        cout << endl;
    }
}

grid addPiece(grid g, int row, int col)
{
    g.board[row][col] = true;
    return g;
}

std::pair<int,int> findTopLeft(grid& g)
{

    bool topFound = false;
    int top = -1;
    for(int row = 0; row < g.size && !topFound; row++)
    {
        for(int col = 0; col < g.size && !topFound; col++)
        {
            if(g.board[row][col])
            {
                topFound = true;
                top = row;
            }
        }
    }

    bool leftFound = false;
    int left = -1;
    for(int col = 0; col < g.size && !leftFound; col++)
    {
        for(int row = 0; row < g.size && !leftFound; row++)
        {
            if(g.board[row][col])
            {
                leftFound = true;
                left = col;
            }
        }
    }

    return std::pair<int,int>(top,left); 
}

grid shiftTopLeft(grid& g)
{
    std::pair<int,int> topLeft = findTopLeft(g);
    if(topLeft.first == -1)
    {
        return g;
    }
    vector<vector<bool>> newBoard;
    for(int newRow = 0; newRow < g.size; newRow++)
    {
        vector<bool> row;
        for(int newCol = 0; newCol < g.size; newCol++)
        {
            if(topLeft.first + newRow < g.size && topLeft.second + newCol < g.size)
            {
                row.push_back(g.board[newRow + topLeft.first][newCol + topLeft.second]);
            }
            else
            {
                row.push_back(false);
            }
        }
    }
    g.board = newBoard;
    return g;
}
grid setBottomRight(grid& g)
{
    bool bottomFound = false;
    for(int row = g.size - 1; row >= 0 && !bottomFound; row--)
    {
        for(int col = 0; col < g.size && !bottomFound; col++)
        {
            if(g.board[row][col])
            {
                bottomFound = true;
                g.bottomMost = row;
            }
        }
    }

    bool rightFound = false;
    for(int col = g.size - 1; col >= 0 && !rightFound; col--)
    {
        for(int row = 0; row < g.size && !rightFound; row++)
        {
            if(g.board[row][col])
            {
                rightFound = true;
                g.rightMost = col;
            }
        }
    }
}
