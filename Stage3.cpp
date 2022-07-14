/*
	Project: Minesweeper
	Author: Kajal Kumari
	Github repo: https://github.com/kajalk14/Minesweeper
*/

#include <bits/stdc++.h>
using namespace std;

// dummyGrid[row][col] == -1 represents a mine.
// dummyGrid[row][col] >= 0 represents a blank cell with count of neighbouring mines

// '*' - represents a revealed mine
// '.' - represents an unrevealed cell
// '-' - represents a revealed blank cell that has no neighbouring mines
// digits 1-8 represents the number of adjacent mines corresponding to a revealed blank cell

void guide(int sizeOfGrid, int totalMines);
bool validCell(int row, int col, int sizeOfGrid);
int detectSquare(int row, int col, vector<vector<int>> &dummyGrid);
void generateMines(int totalMines, vector<vector<int>> &dummyGrid);
bool gameMove(char type, int row, int col, bool originalDig, int &numOfCellsRevealed, int &totalMines, vector<vector<int>> &dummyGrid, vector<vector<char>> &gameGrid);
void print(vector<vector<char>> &gameGrid);

int chooseDifficulty();
int setSize(int difficulty);
int calcMines(int difficulty);

void guide(int sizeOfGrid, int totalMines)
{
	cout << "Your minefield will be a " << sizeOfGrid << "x" << sizeOfGrid << " grid containing "
		 << totalMines << " mines\n";
	cout << "After every move, the current status of the minefield will be displayed automatically.\n";
	cout << "Stick to the input format to make a valid move\n";
	cout << "Input Format:\n";
	cout << "R x y: Reveal the cell at (x,y)\n";
	cout << "F x y: Flag the cell at (x,y)\n";
	cout << "U x y: Unflag the cell at (x,y)\n\n";
}

// checks the validity of a cell
bool validCell(int row, int col, int sizeOfGrid)
{
	if (row < 0 || row >= sizeOfGrid || col < 0 || col >= sizeOfGrid)
		return false;

	return true;
}

// counts number of mines in a cell & its neighbouring cells
int detectSquare(int row, int col, vector<vector<int>> &dummyGrid)
{
	int sizeOfGrid = dummyGrid.size();
	int countMines = 0;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int newRow = row + i, newCol = col + j;

			if (!validCell(newRow, newCol, sizeOfGrid))
				continue;

			if (dummyGrid[i][j] == -1)
				countMines++;
		}
	}
	return countMines;
}

// randomly generates mines in the minefield such that the mines are not too crowded or too dispersed
void generateMines(int totalMines, vector<vector<int>> &dummyGrid)
{
	int sizeOfGrid = dummyGrid.size();

	while (totalMines > 0)
	{
		int x = rand() % sizeOfGrid;
		int y = rand() % sizeOfGrid;

		// if the cell already doesn't have a mine
		if (dummyGrid[x][y] != -1)
		{
			// ensure this position is not too crowded
			int cntMines = 0;

			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					int neighX = x + i, neighY = y + j;

					if (!validCell(neighX, neighY, sizeOfGrid))
						continue;

					if (dummyGrid[neighX][neighY] == -1)
						cntMines++;
				}
			}

			if (cntMines >= 3)
				continue;

			// mark (x,y) as a mined cell and increment the count of its adjacent blank cells
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					int neighX = x + i, neighY = y + j;

					if (!validCell(neighX, neighY, sizeOfGrid))
						continue;

					if (i == 0 && j == 0)
						dummyGrid[neighX][neighY] = -1;

					else
					{
						if (dummyGrid[neighX][neighY] == -1)
							continue;
						else
							dummyGrid[neighX][neighY] += 1;
					}
				}
			}
			totalMines--;
		}
	}
}

bool gameMove(char type, int row, int col, bool originalDig, int &numOfCellsRevealed, int &totalMines, vector<vector<int>> &dummyGrid, vector<vector<char>> &gameGrid)
{
	int sizeOfGrid = dummyGrid.size();

	if ((!validCell(row, col, sizeOfGrid)) || (type != 'R' && type != 'F' && type != 'U'))
	{
		cout << "Invalid Move! Check out the guide\n";
		guide(sizeOfGrid, totalMines);
		return true;
	}

	// flag the cell
	if (type == 'F')
	{
		gameGrid[row][col] = 'F';
		print(gameGrid);
		return true;
	}
	// unflag the cell
	if (type == 'U')
	{
		gameGrid[row][col] = '.';
		print(gameGrid);
		return true;
	}
	// reveal the cell
	if (dummyGrid[row][col] == -1)
	{
		// if user chose the cell -> dig=true
		// if it's a result of revealing a blank cell (2nd or more recursion call)
		if (originalDig == true)
		{
			gameGrid[row][col] = '*';
			cout << "You Lost!\n\n";
			print(gameGrid);
			return false;
		}

		return true;
	}

	numOfCellsRevealed++;

	// if the cell is blank but has >0 neighbouring mines
	if (dummyGrid[row][col] > 0)
	{
		gameGrid[row][col] = dummyGrid[row][col] + '0';
		if (originalDig)
			print(gameGrid);
		return true;
	}

	// cell is blank and no neighbouring mines
	gameGrid[row][col] = ' ';

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int newRow = row + i, newCol = col + j;
			if (gameGrid[newRow][newCol] == ' ')
				continue;

			if (!validCell(newRow, newCol, sizeOfGrid))
				continue;
			gameMove(type, newRow, newCol, false, numOfCellsRevealed, totalMines, dummyGrid, gameGrid);
		}
	}
	if (!originalDig)
		return true;

	if (numOfCellsRevealed == (sizeOfGrid * sizeOfGrid - totalMines))
	{
		cout << "You Won!\n\n";
		print(gameGrid);
		return false;
	}

	print(gameGrid);
	return true;
}

// prints the game grid
void print(vector<vector<char>> &gameGrid)
{
	cout << "Your current Minefield: \n";
	int sizeOfGrid = gameGrid.size();

	cout << " ";
	for (int i = 0; i < sizeOfGrid; i++)
	{
		cout << setw(2) << i << " ";
	}
	cout << "\n";
	for (int i = 0; i < sizeOfGrid; i++)
	{
		cout << setw(2) << i << " ";

		for (int j = 0; j < sizeOfGrid; j++)
		{
			cout << setw(2) << gameGrid[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int chooseDifficulty()
{
	cout << "Choose your mode of difficulty:\n"
		 << "Enter 1 for Beginner (9x9)\n"
		 << "Enter 2 for Intermediate (16x16)\n"
		 << "Enter 3 for Advanced (24x24)\n\n";

	int difficulty = 0;
	cin >> difficulty;

	if (difficulty >= 1 && difficulty <= 3)
		return difficulty;
	else
	{
		cout << "Oops! Wrong Choice!\n";
		return chooseDifficulty();
	}
}

int setSize(int difficulty)
{
	if (difficulty == 1)
		return 9;

	if (difficulty == 2)
		return 16;

	return 22;
}

int calcMines(int difficulty)
{
	if (difficulty == 1)
		return 10;

	if (difficulty == 2)
		return 40;

	return 99;
}

int main()
{
	cout << "Welcome to Minesweeper!\n\n";

	int difficulty = chooseDifficulty();
	int sizeOfGrid = setSize(difficulty);
	int totalMines = calcMines(difficulty);

	vector<vector<int>> dummyGrid(sizeOfGrid, vector<int>(sizeOfGrid, 0));
	vector<vector<char>> gameGrid(sizeOfGrid, vector<char>(sizeOfGrid, '.'));

	guide(sizeOfGrid, totalMines);

	generateMines(totalMines, dummyGrid);

	cout << "Game Starts!\n\n";
	print(gameGrid);

	int numOfCellsRevealed = 0;
	bool gameStatus = true;
	char type;
	int row, col;

	while (gameStatus)
	{
		cout << "Enter coordinates (R/F/U x y): ";
		cin >> type >> row >> col;
		gameStatus = gameMove(type, row, col, true, numOfCellsRevealed, totalMines, dummyGrid, gameGrid);
	}

	return 0;
}
