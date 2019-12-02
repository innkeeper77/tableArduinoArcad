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
#include <ctime>
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

	int clocks = 0;

	int TIME_THRESH = 250;

	float temp = 0;

	block active_block;

	int block_size = 3;

	int iters = 0;

	int start_time = 0;

	active_block.l_pos = 4;

	active_block.size = block_size;

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
			for (int i = active_block.l_pos; i < active_block.l_pos+active_block.size; ++i)
			{
				board[i][curr_row] = (char)WHITE;
			}

			active = true;
			start_time = clock();
			print_board(board);
		}
		else
		{
			if (wait_timer >= TIME_THRESH)
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
				++iters;
				wait_timer = 0;
				start_time = time(NULL);
			}
			else
			{
				//key poll here
  				if (iters > 30)
				{

					if (curr_row == 0)
					{
						++curr_row;

						active_block.l_pos = 4;

						active_block.size = block_size;

						active = false;
					}
					else
					{
						if (left)
						{
							for (int i = active_block.l_pos; i < active_block.l_pos + active_block.size; ++i)
							{
								if (board[i][curr_row - 1] == (char)BLACK)
								{
									board[i][curr_row] = (char)BLACK;
								}
							}
						}
						else
						{
							for (int i = active_block.l_pos; i < active_block.l_pos + active_block.size; ++i)
							{
								if (board[i][curr_row - 1] == (char)BLACK)
								{
									board[i][curr_row] = (char)BLACK;
								}
							}
						}
						
					}

					iters = 0;
				}
				//timer polling stuff here
				
				clocks = clock() - start_time;

				temp = clocks / CLOCKS_PER_SEC;

				wait_timer = (int)temp * 1000;
			}
		}
		
	}
}