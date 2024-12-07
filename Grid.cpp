#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
using std::cout, std::vector, std::string, std::unique;

int initializeGrid(vector<vector<bool>>& grid, int n);
vector<vector<bool>> expandGrid(vector<vector<bool>>& grid);
vector<vector<bool>> shrinkToPieces(vector<vector<bool>>& grid);
string gridToString(vector<vector<bool>>& grid);
vector<vector<bool>> addPiece(vector<vector<bool>>& grid, int row, int col);
vector<vector<bool>> shiftTopLeft(vector<vector<bool>>& grid);
vector<vector<vector<bool>>> generatePolyominoes(int n);
bool nextToAPiece(vector<vector<bool>>& grid, int row, int col);
void removeDuplicates(vector<vector<vector<bool>>>& solutions);
string solutionsToString(vector<vector<vector<bool>>>& solutions);
/* idea: Polyominoes will be store in 2d grids.
Grids will be a vector of vector of boolean values.
These boolean values track where each piece of the polyomino is at
For example:
[false][false][false] -> [ ][ ][ ]
[false][true ][false]    [ ][P][ ]
[false][true ][true ]    [ ][P][P]
Polyominoes with n pieces will be stored in n x n grids
We can make polyominoes of size n from adding pieces to smaller ones of size n - 1
Polyominoes of size n = 1 are trivial: [P]
*/
int main()
{
    int size = 1;
    cout << "Enter Size of N-Ominoes to Generate: " << "\n";
    while(std::cin >> size)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        vector<vector<vector<bool>>> solutions = generatePolyominoes(size);
        cout << "Polyominoes of Size: " << size << "\n";
        cout << "Generated " << solutions.size() << " Solutions: " << "\n";
        cout << solutionsToString(solutions);
        cout << "Enter Size of N-Ominoes to Generate: " << "\n";
        auto t2 = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1);
        cout << "time elapsed: " << elapsed.count() << " milliseconds" << "\n";
    }
    return EXIT_SUCCESS;
}

/* initializes starting grid of size nxn
for example:
       n = 2 -> [ ][ ]
                [ ][ ]
*/
int initializeGrid(vector<vector<bool>>& grid, int n)
{
    for(int i = 0; i < n; i++)
    {
        vector<bool> row;
        for(int j = 0; j < n; j++)
        {
            row.push_back(false);
        }
        grid.push_back(row);
    }
    return EXIT_SUCCESS;
}

/* expands grid in each direction by 1, makes it larger than needed to be able to add pieces to each side
for example: [P][P] -> [ ][ ][ ][ ]
             [ ][ ]    [ ][P][P][ ]
                       [ ][ ][ ][ ]
                       [ ][ ][ ][ ]
*/
vector<vector<bool>> expandGrid(vector<vector<bool>>& grid)
{
    vector<vector<bool>> newGrid;
    int n = grid.size();
    for(int i = 0; i < n + 2; i++)
    {
        vector<bool> row;
        for(int j = 0; j < n + 2; j++)
        {
            if(i > 0 && i < n + 1 && j > 0 && j < n + 1) //add pieces already in grid
            {
                row.push_back(grid[i - 1][j - 1]);
            }
            else
            {
                row.push_back(false); //new parts are empty
            }
        }
        newGrid.push_back(row);
    }
    return newGrid;
}

/*shrinks grid to size actually needed to hold the n-omino after shifting (1 on right, 1 on bottom)
for example: [P][P][P][ ] -> [P][P][P]
             [ ][ ][ ][ ]    [ ][ ][ ]
             [ ][ ][ ][ ]    [ ][ ][ ]
             [ ][ ][ ][ ]
*/
vector<vector<bool>> shrinkToPieces(vector<vector<bool>>& grid)
{
    /*vector<vector<bool>> newGrid;
    int n = grid.size();
    for(int i = 0; i < n - 1; i++)
    {
        vector<bool> row;
        for(int j = 0; j < n - 1; j++)
        {
            row.push_back(grid[i][j]);
        }
        newGrid.push_back(row);
    }
    return newGrid;
    */
    
    int n = grid.size();
    for(int i = 0; i < n - 1; i++)
    {
        grid[i].erase(grid[i].end() - 1);
    }
    grid.erase(grid.end() - 1);
    return grid;
    
}
//converts grid to string for printing
string gridToString(vector<vector<bool>>& grid)
{
    string result;
    for(int i = 0; i < grid.size(); i++)
    {
        for(int j = 0; j < grid[i].size(); j++)
        {
            string piece = grid[i][j] ? "P" : " "; 
            result += "[" + piece + "]";
        }
        result += "\n";
    }
    return result;
}

/* adds piece at row, column
for example: addPiece(grid, 1, 2)
                      0  1  2
grid: [ ][ ][ ] -> 0 [ ][ ][ ]
      [ ][ ][ ]    1 [ ][ ][P]
      [ ][ ][ ]    2 [ ][ ][ ]
*/
vector<vector<bool>> addPiece(vector<vector<bool>>& grid, int row, int col)
{
    vector<vector<bool>> newGrid = grid;
    newGrid[row][col] = true;
    return newGrid;
}
/* shifts a piece to the top left of the board
for example: [ ][ ][ ] -> [P][P][ ]
             [ ][P][P]    [P][ ][ ]
             [ ][P][ ]    [ ][ ][ ]
*/
vector<vector<bool>> shiftTopLeft(vector<vector<bool>>& grid)
{
    int n = grid.size();
    
    //finds row of top most piece
    int topMostPieceRow = -1;
    for(int i = 0; i < n && topMostPieceRow == -1; i++)
    {
        for(int j = 0; j < n && topMostPieceRow == -1; j++)
        {
            if(grid[i][j])
            {
                topMostPieceRow = i;
            }
        }
    }

    //finds column of left most piece
    int leftMostPieceCol = -1;
    for(int j = 0; j < n && leftMostPieceCol == -1; j++)
    {
        for(int i = 0; i < n && leftMostPieceCol == -1; i++)
        {
            if(grid[i][j])
            {
                leftMostPieceCol = j;
            }
        }
    }

    //in case of empty grid, return an empty grid
    if(topMostPieceRow == -1)
    {
        vector<vector<bool>> newGrid = grid;
        return newGrid;
    }

    //shifts grid based on that
    vector<vector<bool>> newGrid;
    for(int i = 0; i < n; i++)
    {
        vector<bool> row;
        for(int j = 0; j < n; j++)
        {
            if(i + topMostPieceRow < n && j + leftMostPieceCol < n)
            {
                row.push_back(grid[i + topMostPieceRow][j + leftMostPieceCol]);
            }
            else
            {
                row.push_back(false);
            }
        }
        newGrid.push_back(row);
    }
    return newGrid;
}
//generates polyominoes that have n pieces from ones that have n - 1 pieces recursively
vector<vector<vector<bool>>> generatePolyominoes(int n)
{
    //only one solution for 1x1
    vector<vector<vector<bool>>> solutions;
    if(n == 1)
    {
        vector<vector<bool>> solution;
        initializeGrid(solution,1);
        solution = addPiece(solution,0,0);
        solutions.push_back(solution);
        return solutions;
    }

    //otherwise start with a smaller polyomino and build up on it
    vector<vector<vector<bool>>> smallerSolutions = generatePolyominoes(n-1);
    //for each smaller polyomino, expand grid, add a piece to each valid location
    for(int i = 0; i < smallerSolutions.size(); i++)
    {
        vector<vector<bool>>& smallerSolution = smallerSolutions[i];
        smallerSolution = expandGrid(smallerSolution);
        int size = smallerSolution.size(); //size of smaller solution after expanding
        for(int row = 0; row < size; row++)
        {
            for(int col = 0; col < size; col++)
            {
                if(!smallerSolution[row][col] && nextToAPiece(smallerSolution, row, col)) //if current position isn't already occupied by a piece and is next to a piece
                {
                    vector<vector<bool>> newSolution = smallerSolution;
                    newSolution[row][col] = true;
                    newSolution = shiftTopLeft(newSolution);
                    newSolution = shrinkToPieces(newSolution);
                    solutions.push_back(newSolution);
                }
            }
        }
    }
    removeDuplicates(solutions);
    return solutions;
}
//checks if piece specified by row, col is next to another piece
bool nextToAPiece(vector<vector<bool>>& board, int row, int col)
{
    //if piece specified by row, col is to the _ of another piece
    int size = board.size();
    bool leftOf = false;
    bool rightOf = false;
    bool topOf = false;
    bool bottomOf = false;
    if(col < size - 1) //leftOf
    {
        leftOf = board[row][col + 1];
    }
    if(col > 0) //rightOf
    {
        rightOf = board[row][col - 1];
    }
    if(row < size - 1) //topOf
    {
        topOf = board[row + 1][col];
    }
    if(row > 0) //bottomOf
    {
        bottomOf = board[row - 1][col];
    }
    return leftOf || rightOf || topOf || bottomOf;
}
//removes duplicate polyominoes found in solutions
void removeDuplicates(vector<vector<vector<bool>>>& solutions)
{
    //more efficient duplicate removal O(nlogn)
    std::sort(solutions.begin(), solutions.end());
    auto last = unique(solutions.begin(), solutions.end());
    solutions.erase(last, solutions.end());

    /*
    inefficient duplicate removal O(n^2)?
    int i = 0;
    while(i < solutions.size())
    {
        int j = i + 1;
        while(j < solutions.size())
        {
            if(solutions[i] == solutions[j])
            {
                solutions.erase(solutions.begin() + j);
            }
            else
            {
                j++;
            }
        }
        i++;
    }
    */
}
//prints all solutions
string solutionsToString(vector<vector<vector<bool>>>& solutions)
{
    string result;
    for(int i = 0; i < solutions.size(); i++)
    {
        result += "Solution " + std::to_string(i) + ": \n" + gridToString(solutions[i]) + "\n";
    }
    return result;
}