#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
using std::cout, std::vector, std::string, std::unique, std::chrono::milliseconds;

struct grid
{
    int size;
    vector<vector<bool>> board;

    int rightMost;
    int bottomMost;
};

grid intitializeGrid(int size);

int main()
{
    int size = 1;
    cout << "Enter Size of N-Ominoes to Generate: " << "\n";
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
    return EXIT_SUCCESS;
}

grid initializeGrid(int size)
{
    grid newGrid;
    for(int i = 0; i < size; i++)
    {
        vector<bool> newRow;
        for(int j = 0; j < size; j++)
        {
            newRow.push_back(false);
        }
        newGrid.board.push_back(newRow);
    }
    newGrid.size = size;
    newGrid.rightMost = 0;
    newGrid.bottomMost = 0;
    return newGrid;
}

grid expandGrid(grid smaller)
{
    grid newGrid;
    int n = smaller.size + 2;
    for(int i = 0; i < n; i++)
    {
        vector<bool> newRow;
        for(int j = 0; j < n; j++)
        {
            if(i > 0 && j > 0 && i < n - 1 && j < n - 1)
            {
                newRow.push_back(smaller.board[i-1][j-1]);
            }
            else
            {
                newRow.push_back(false);
            }
        }
        newGrid.board.push_back(newRow);
    }

    //
    newGrid.size = n - 1;
    newGrid.rightMost = smaller.rightMost;
    newGrid.bottomMost = smaller.bottomMost;
    return newGrid;
}

grid shrinkToSize(grid bigger)
{
    int n = bigger.size;
    bigger.board.erase(bigger.board.end() - 1);
    for(int i = 0; i < n; i++)
    {
        bigger.board[i].erase(bigger.board[i].end() - 1);
    }
    return bigger;
}

