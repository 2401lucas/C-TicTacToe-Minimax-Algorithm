#include "pch.h"
#include "tic_tac_toe_stats.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>


class Game
{
	enum class Player
	{
		none = '-',
		human = 'X',
		computer = 'O'
	};

	struct Move
	{
		unsigned int x = 0;
		unsigned int y = 0;
	};

	Player matrix[3][3];
	
public:
	Game()
	{
		available = pow(2, board_size);
		for (unsigned int i = 0; i < board_size; i++)
		{
			for (unsigned int j = 0; j < board_size; j++)
			{
				matrix[i][j] = Player::none;
			}
		}
	}

	void draw()
	{
		std::cout << "+-----------------+";
		for (unsigned int i = 0; i < board_size; i++)
		{
			std::cout << "\n|";
			for (unsigned int j = 0; j < board_size; j++)
			{
				std::cout << std::setw(board_size) << static_cast<char>(matrix[i][j]) << std::setw(board_size) << " |";
			}
		}
		std::cout << "\n+-----------------+\n";
	}

	bool is_tie() const
	{
		if(!winner)
		{
			return available == -1;
		}
		return false;
	}

	bool check(Player player)
	{

		for (unsigned int i = 0; i < board_size; i++)
		{
			// Check horizontals
			if (matrix[i][0] == player && matrix[i][1] == player && matrix[i][2] == player)
				return true;

			// Check verticals
			if (matrix[0][i] == player && matrix[1][i] == player && matrix[2][i] == player)
				return true;
		}
		if (matrix[0][0] == player && matrix[1][1] == player && matrix[2][2] == player)
			return true;

		if (matrix[0][2] == player && matrix[1][1] == player && matrix[2][0] == player)
			return true;

		return false;
	}

	Move minimax()
	{
		int level = 0;
		int score = std::numeric_limits<int>::max();
		Move move;

		for (unsigned int i = 0; i < board_size; i++)
		{
			for (unsigned int j = 0; j < board_size; j++)
			{
				if (matrix[i][j] == Player::none)
				{
					matrix[i][j] = Player::computer;

					int temp = max_search(level);

					if (temp < score)
					{
						score = temp;
						move.x = i;
						move.y = j;
					}
					matrix[i][j] = Player::none;
				}
			}
		}
		available--;
		return move;
	}

	int max_search(int level)
	{
		if (check(Player::human)) { return -10; }
		else if (check(Player::computer)) { return 10; }
		else if (is_tie()) { return 0; }

		int score = std::numeric_limits<int>::min();

		for (unsigned int i = 0; i < board_size; i++)
		{
			for (unsigned int j = 0; j < board_size; j++)
			{
				if (matrix[i][j] == Player::none)
				{
					matrix[i][j] = Player::human;
					score = std::max(score, (min_search(level + 1) - level));
					matrix[i][j] = Player::none;
				}
			}
		}

		return score;
	}

	int min_search(int level)
	{
		if (check(Player::human)) { return  -10; }
		else if (check(Player::computer)) { return 10; }
		else if (is_tie()) { return 0; }

		int score = std::numeric_limits<int>::max();

		for (unsigned int i = 0; i < board_size; i++)
		{
			for (unsigned int j = 0; j < board_size; j++)
			{
				if (matrix[i][j] == Player::none)
				{
					matrix[i][j] = Player::computer;
					score = std::min(score, (max_search(level + 1) + level));
					matrix[i][j] = Player::none;
				}
			}
		}

		return score;
	}

	void player_move()
	{
		bool fail = true;
		unsigned int x = -1, y = -1;

		do
		{
			std::cout << "Your Move: ";

			char c;
			std::cin >> c;
			y = c - '0';
			std::cin >> c;
			x = c - '0';

			fail = matrix[x][y] != Player::none;

			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		} while (fail);

		matrix[x][y] = Player::human;
		available--;
	}

	void play()
	{
		bool turn = true;
		bool exit = false;

		draw();
		std::cout << "Enter your move in coordinate form[row, col]. ex: 02\n";

		do
		{
			// human move
			if (turn)
			{
				player_move();

				if (check(Player::human))
				{
					std::cout << "Human Wins\n";
					exit = true;
					winner = true;
				}
			}
			else
			{
				std::cout << "\nComputer Move: ";

				Move aimove = minimax();
				std::cout << aimove.y << aimove.x << "\n";
				matrix[aimove.x][aimove.y] = Player::computer;

				if (check(Player::computer))
				{
					std::cout << "Computer Wins\n";
					exit = true;
					winner = true;
				}
			}

			if (is_tie())
			{
				std::cout << "\n*** Tie ***\n";
				exit = true;
			}

			turn = !turn;
			draw();

		} while (!exit);
	}
};

int main()
{
	Game tictactoe;
	tictactoe.play();
	std::cin.ignore();
}