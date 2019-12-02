/*
Benjamin Stanton
CSCI 3511 Hardware-Software Interface
12/1/2019
Block Stacking game
*/

#define ROWS 20
#define COLS 10
#define WHITE 219
#define BLACK 32

#include <string>
#include <iostream>
using namespace std;

struct block
{
	int l_pos;
	int size;
};

void print_board(char board[COLS][ROWS])
{
	system("CLS");

	for (int i = ROWS-1; i >= 0; --i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			cout << board[j][i];
		}

		cout << endl;
	}
}

int main()
{
	char board[COLS][ROWS];

	bool game_over = false;

	bool active = false;

	bool left = true;
	
	int curr_row = 0;

	int wait_timer = 0;

	block active_block;

	active_block.l_pos = 0;

	active_block.size = 3;

	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			board[j][i] = (char)BLACK;
		}
	}

	while (!game_over)
	{
		if (active == false)
		{
			for (int i = 0; i < active_block.size; ++i)
			{
				board[i][curr_row] = (char)WHITE;
			}

			active = true;

			print_board(board);
		}
		else
		{
			if (wait_timer >= 250)
			{
				if (left)
				{
					if (active_block.l_pos + active_block.size > COLS - 1)
					{
						left = false;
					}
					else
					{
						board[active_block.l_pos][curr_row] = (char)BLACK;
						++active_block.l_pos;
						for (int i = active_block.l_pos; i < active_block.l_pos + active_block.size; ++i)
						{
							board[i][curr_row] = (char)WHITE;

						}						
					}
					
				}
				else
				{
					if (active_block.l_pos <= 0)
					{
						left = true;
					}
					else
					{
						board[active_block.l_pos+active_block.size-1][curr_row] = (char)BLACK;
						--active_block.l_pos;
						for (int i = active_block.l_pos; i < active_block.l_pos + active_block.size; ++i)
						{
							board[i][curr_row] = (char)WHITE;
						}
					}
				}

				print_board(board);

			}
			else
			{
				++wait_timer;
			}
		}
		
	}
}