#include <iostream>
using namespace std;

struct Game
{
	const int width = 20;
	const int height = 10;
	int score = 0;
	int lastScore = 0;
	int foodX = 0;
	int foodY = 0;
	bool gameOver = false;
	const char* pathToFile = ".\\GameResultSnake.txt";
};

struct Snake
{
	int x[100], y[100];
	int length;
};

Game game;
Snake snake;
int direction = 0;

void loadGameResult()
{
	FILE* file;
	errno_t code = fopen_s(&file, game.pathToFile, "r");
	if (code == 0 && file != nullptr)
	{
		fscanf_s(file, "Final Score: %d", &game.lastScore);
		fclose(file);
	}
	else
	{
		game.score = 0;
	}
}

void saveGameResult()
{
	FILE* file;
	errno_t code = fopen_s(&file, game.pathToFile, "w");
	if (code == 0 && file != nullptr)
	{
		fprintf_s(file, "Final Score: %d\n", game.score);
		fclose(file);
	}
	else
	{
		cout << "Error." << endl;
	}
}

void generateFood()
{
	srand(time(NULL));

	bool foodOnSnake = true;
	while (foodOnSnake)
	{
		foodOnSnake = false;
		game.foodX = rand() % game.width;
		game.foodY = rand() % game.height;
		for (int i = 0; i < snake.length; i++)
		{
			if (snake.x[i] == game.foodX && snake.y[i] == game.foodY)
			{
				foodOnSnake = true;
				break;
			}
		}
	}
}

void setup()
{
	game.gameOver = false;
	direction = 0;
	snake.length = 1;
	snake.x[0] = game.width / 2;
	snake.y[0] = game.height / 2;
	game.score = 0;
	generateFood();
	loadGameResult();
}

void draw()
{
	system("cls");
	for (int i = 0; i < game.width + 2; i++)
	{
		cout << "#";
	}
	cout << endl;

	for (int i = 0; i < game.height; i++)
	{
		for (int j = 0; j < game.width; j++)
		{
			if (j == 0)
			{
				cout << "#";
			}
			if (i == snake.y[0] && j == snake.x[0])
			{
				cout << "O";
			}
			else if (i == game.foodY && j == game.foodX)
			{
				cout << "*";
			}
			else
			{
				bool snakePart = false;
				for (int k = 0; k < snake.length; k++)
				{
					if (snake.x[k] == j && snake.y[k] == i)
					{
						cout << "o";
						snakePart = true;
						break;
					}
				}
				if (!snakePart)
				{
					cout << " ";
				}
			}

			if (j == game.width - 1)
			{
				cout << "#";
			}
		}
		cout << endl;
	}
	for (int i = 0; i < game.width + 2; i++)
	{
		cout << "#";
	}
	cout << endl;

	cout << "Score: " << game.score << " Last Score: " << game.lastScore << endl;
	cout << "Use W/A/S/D to move and X to quit." << endl;
}

void input()
{
	char current;
	if (cin >> current)
	{
		switch (current)
		{
			case 'a':
				direction = 1;
				break;
			case 'd':
				direction = 2;
				break;
			case 'w':
				direction = 3;
				break;
			case 's':
				direction = 4;
				break;
			case 'x':
				game.gameOver = true;
				break;
			default:
				break;
		}
	}
}

void logic()
{
	int prevX = snake.x[0];
	int prevY = snake.y[0];
	int prev2X, prev2Y;

	if (direction == 1)
	{
		snake.x[0]--;
	}
	else if (direction == 2)
	{
		snake.x[0]++;
	}
	else if (direction == 3)
	{
		snake.y[0]--;
	}
	else if (direction == 4)
	{
		snake.y[0]++;
	}

	for (int i = 1; i < snake.length; i++)
	{
		prev2X = snake.x[i];
		prev2Y = snake.y[i];
		snake.x[i] = prevX;
		snake.y[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	if (snake.x[0] == game.foodX && snake.y[0] == game.foodY)
	{
		game.score += 10;
		snake.length++;

		snake.x[snake.length - 1] = prevX;
		snake.y[snake.length - 1] = prevY;

		generateFood();
	}

	if (snake.x[0] >= game.width || snake.x[0] < 0 || snake.y[0] >= game.height || snake.y[0] < 0)
	{
		game.gameOver = true;
	}

	for (int i = 1; i < snake.length; i++)
	{
		if (snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0])
		{
			game.gameOver = true;
		}
	}
}

int main()
{
	setup();
	while (!game.gameOver)
	{
		draw();
		input();
		logic();
	}

	saveGameResult();

	cout << "Game Over!" << endl;
}