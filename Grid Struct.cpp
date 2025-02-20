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

