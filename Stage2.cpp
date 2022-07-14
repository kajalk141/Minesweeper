#include <bits/stdc++.h>
using namespace std;
#define size 8
int matrix[size][size];
int dx[3] = {-1, 0, 1}, dy[3] = {-1, 0, 1};
int revealed_squares = 0;

// matrix[row][col]=0 represents a square that has been revealed.
// matrix[row][col]=1 represents a square that has not been revealed but which doesn't contain a mine.
// matrix[row][col]=2 represents a square that has not been revealed and conatins a mine.

int detectRow(int row, int col_start, int col_count);
int detectSquare(int row, int col, int square_size);
bool revealCross(int row, int col, int n);
void print(bool gameStatus);

void initialize_matrix()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = 1;
		}
	}
	revealed_squares = 0;
}

int detectRow(int row, int col_start, int col_count)
{
	int count_mines = 0;
	while (col_count--)
	{
		if (matrix[row][col_start + col_count] == 2)
			count_mines++;
	}
	return count_mines;
}

int detectSquare(int row, int col, int square_size)
{
	int count_mines = 0;
	int half_size = square_size / 2;
	for (int i = max(0, row - half_size); i <= min(size - 1, row + half_size); i++)
	{
		for (int j = max(0, col - half_size); j <= min(size - 1, col + half_size); j++)
		{
			if (matrix[i][j] == 2)
				count_mines++;
		}
	}
	return count_mines;
}

bool revealCross(int row, int col, int n)
{
	if (matrix[row][col] == 2)
	{
		cout << "Game Over!\n\n";
		print(false);
		return false;
	}
	if (matrix[row][col] == 1)
		revealed_squares++;
	matrix[row][col] = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (abs(dx[i]) != abs(dy[j]))
			{
				int r = row + dx[i], c = col + dy[j];
				if (r >= 0 && r < size && c >= 0 && c < size)
				{
					if (matrix[r][c] != 2 && detectSquare(r, c, 3) == 0)
					{
						if (matrix[r][c] == 1)
							revealed_squares++;
						matrix[r][c] = 0;
					}
				}
			}
		}
	}
	if (revealed_squares == (size * size - n))
	{
		cout << "Game Won!\n\n";
		print(false);
		return false;
	}
	print(true);
	return true;
}

void print(bool gameStatus)
{
	cout << "  0 1 2 3 4 5 6 7\n"; // prints column number
	for (int i = 0; i < size; i++)
	{
		cout << i << " "; // prints row number
		for (int j = 0; j < size; j++)
		{
			if (matrix[i][j] == 0)
			{
				// a revealed square prints the number of mines adjacent to it
				cout << detectSquare(i, j, 3) << " ";
			}
			else if (matrix[i][j] == 1)
			{
				// an unrevealed square
				cout << ". ";
			}
			else
			{
				// if the game is still going on
				if (gameStatus)
					cout << ". ";
				// if the game has been won/lost, reveal the mines
				else
					cout << "* ";
			}
			//			cout << matrix[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int main()
{
	initialize_matrix();
	cout << "Welcome to minesweeper!\n\nHow many mines? ";
	int n;
	cin >> n;
	cout << "\nEnter coordinate pairs of the mines:\n\n";
	for (int i = 0; i < n; i++)
	{
		int x, y;
		cin >> x >> y;
		if ((x >= 0 && x < size) && (y >= 0 && y < size))
			matrix[x][y] = 2;
	}
	cout << "\nGame Started\n\n";
	print(true);
	int type, cntHelp = 0;
	bool gameStatus = true;
	while (gameStatus == true)
	{
		cin >> type;
		if (type == 1)
		{
			if (cntHelp >= 3)
			{
				cout << "All hints exhausted!\n";
			}
			int row, col_start, col_count;
			cin >> row >> col_start >> col_count;
			if (row >= 0 && row < size && col_start >= 0 && col_count >= 0 && (col_start + col_count) < size)
			{
				cntHelp++;
				cout << "\nThere are " << detectRow(row, col_start, col_count) << " mine(s) in row " << row << ", from column " << col_start << " to " << col_start + col_count - 1 << "\n\n";
			}
			else
				cout << "\nCoordinates not on map\n\n";
			print(true);
		}
		else if (type == 2)
		{
			if (cntHelp >= 3)
			{
				cout << "All hints exhausted!\n";
			}
			int row, col, sq_size;
			cin >> row >> col >> sq_size;
			if (row >= 0 && row < size && col >= 0 && col < size && sq_size > 0 && sq_size % 2 == 1)
			{
				cntHelp++;
				cout << "\nThere are " << detectSquare(row, col, sq_size) << " mine(s) in the square centered at row" << row << " & column " << col << " of size " << sq_size << "\n\n";
			}
			else if (sq_size % 2 == 0)
				cout << "Only odd square sizes are allowed!\n\n";
			else
				cout << "\nCoordinates not on map\n\n";
			print(true);
		}
		else if (type == 3)
		{
			int row, col;
			cin >> row >> col;
			if (row >= 0 && row < size && col >= 0 && col < size)
				gameStatus = revealCross(row, col, n);
			else
				cout << "\nCoordinates not on map\n\n";
		}
	}
}
