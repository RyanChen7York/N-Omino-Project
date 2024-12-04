#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
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

int main()
{
    int size = 1;
    cout << "Enter Size of N-Ominoes to Generate: " << "\n";
    while(std::cin >> size)
    {
        vector<vector<vector<bool>>> solutions = generatePolyominoes(size);
        cout << "Polyominoes of Size: " << size << "\n";
        cout << "Generated " << solutions.size() << " Solutions: " << "\n";
        cout << solutionsToString(solutions);
        cout << "Enter Size of N-Ominoes to Generate: " << "\n";
    }
    /*
    vector<vector<vector<bool>>> solutions;
    vector<vector<vector<bool>>> solutionsShift;
    vector<vector<bool>> grid;
    vector<vector<bool>> grid1;
    vector<vector<bool>> grid3;
    initializeGrid(grid3, 5);
    grid3 = addPiece(grid3, 1, 3);
    grid3 = addPiece(grid3, 3, 1);
    grid3 = addPiece(grid3, 4, 4);
    solutions.push_back(grid3);
    initializeGrid(grid, size);
    grid = addPiece(grid, 2, 1);
    grid1 = expandGrid(grid);
    grid1 = shrinkToPieces(grid1);
    vector<vector<bool>> grid2 = addPiece(grid1, 0, 1);
    //solutions.push_back(grid);
    //solutions.push_back(grid1);
    //solutions.push_back(grid2);
    
    for(int i = 0; i < solutions.size(); i++)
    {
        solutionsShift.push_back(shiftTopLeft(solutions[i]));
    }
    removeDuplicates(solutions);
    cout << solutionsToString(solutions);
    cout << "shifted: " << "\n";
    cout << solutionsToString(solutionsShift);
    */
    return EXIT_SUCCESS;
}

//initializes starting grid of size nxn
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

//expands grid in each direction by 1, makes it larger than needed to be able to add pieces to each side
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

//shrinks grid to size actually needed to hold the n-omino after shifting (1 on right, 1 on bottom)
vector<vector<bool>> shrinkToPieces(vector<vector<bool>>& grid)
{
    vector<vector<bool>> newGrid;
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

//adds piece at row, column
vector<vector<bool>> addPiece(vector<vector<bool>>& grid, int row, int col)
{
    vector<vector<bool>> newGrid = grid;
    newGrid[row][col] = true;
    return newGrid;
}

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
        vector<vector<bool>> smallerSolution = smallerSolutions[i];
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

bool nextToAPiece(vector<vector<bool>>& board, int row, int col)
{
    //if piece specified by row, col is _ of another piece
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
void removeDuplicates(vector<vector<vector<bool>>>& solutions)
{
    //auto last = unique(solutions.begin(), solutions.end());
    //solutions.erase(last, solutions.end());
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
}
string solutionsToString(vector<vector<vector<bool>>>& solutions)
{
    string result;
    for(int i = 0; i < solutions.size(); i++)
    {
        result += "Solution " + std::to_string(i) + ": \n" + gridToString(solutions[i]) + "\n";
    }
    return result;
}