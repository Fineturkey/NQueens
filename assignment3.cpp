#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>

using namespace std;

class NQueensSolver
{
private:
    int N;
    int maxSteps;
    vector<int> board; // board[col] = row of queen in that column
    mt19937 rng;

public:
    NQueensSolver(int size, int steps = 10000) : N(size), maxSteps(steps), board(size)
    {
        rng.seed(static_cast<unsigned int>(time(nullptr)));
        initializeBoard();
    }

    void initializeBoard()
    {
        for (int col = 0; col < N; ++col)
        {
            board[col] = rand() % N;
        }
    }

    int countConflicts(int col, int row) const
    {
        int conflicts = 0;
        for (int otherCol = 0; otherCol < N; ++otherCol)
        {
            if (otherCol == col)
                continue;
            int otherRow = board[otherCol];

            // Same row
            if (otherRow == row)
                conflicts++;
            // Diagonal
            if (abs(otherRow - row) == abs(otherCol - col))
                conflicts++;
        }
        return conflicts;
    }

    int totalConflicts() const
    {
        int total = 0;
        for (int col = 0; col < N; ++col)
        {
            total += countConflicts(col, board[col]);
        }
        return total / 2; // each conflict is counted twice
    }

    bool solve()
    {
        for (int step = 0; step < maxSteps; ++step)
        {
            if (totalConflicts() == 0)
                return true;

            vector<int> conflictedCols;
            for (int col = 0; col < N; ++col)
            {
                if (countConflicts(col, board[col]) > 0)
                {
                    conflictedCols.push_back(col);
                }
            }

            if (conflictedCols.empty())
                return true;

            // Randomly select one conflicted column
            int col = conflictedCols[rng() % conflictedCols.size()];
            int minConflicts = numeric_limits<int>::max();
            vector<int> bestRows;

            for (int row = 0; row < N; ++row)
            {
                int conflicts = countConflicts(col, row);
                if (conflicts < minConflicts)
                {
                    bestRows = {row};
                    minConflicts = conflicts;
                }
                else if (conflicts == minConflicts)
                {
                    bestRows.push_back(row);
                }
            }

            // Pick randomly among best candidates
            board[col] = bestRows[rng() % bestRows.size()];
        }
        return false; // Failed to find solution within maxSteps
    }

    void printBoard() const
    {
        for (int row = 0; row < N; ++row)
        {
            for (int col = 0; col < N; ++col)
            {
                cout << (board[col] == row ? " Q " : " . ");
            }
            cout << '\n';
        }
    }
};

int main()
{
    int n;
    cout << "Enter the number of queens (N): ";
    cin >> n;

    NQueensSolver solver(n);

    if (solver.solve())
    {
        cout << "\n Solution found:\n\n";
        solver.printBoard();
    }
    else
    {
        cout << "\n No solution found within the maximum step limit.\n";
    }

    return 0;
}
