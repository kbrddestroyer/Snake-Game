#include <iostream>
#include <stdio.h>
#include <thread>
#include <conio.h>
#include <iomanip>
#include <dos.h>
#include <vector>
#include <fstream>

#include <Windows.h>

#define DELAY_TIME 100
#define MAP_SIZE_X 15
#define MAP_SIZE_Y 25
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

using namespace std;

struct PlayerStats
{
	int x;
	int y;
	int apple_x;
	int apple_y;
	int rating;
};

struct Vertex
{
	int x;
	int y;
};

PlayerStats player;

int		default_x = rand() % (MAP_SIZE_X - 2) + 1;
int		default_y = rand() % (MAP_SIZE_Y - 2) + 1;
int		default_apple_x = rand() % (MAP_SIZE_X - 2) + 1;
int		default_apple_y = rand() % (MAP_SIZE_Y - 2) + 1;

int		k			=		0;
int		direction	=		1;
bool	running		=		true;

char	map[MAP_SIZE_X][MAP_SIZE_Y];

void	keyboard		(void);
void	drawUI			(void);
void	core			(void);
void	drawMap			(void);
void	generateApple	(void);
void	clear			(void);
void 	mainmenu		(void);
void	startgame		(void);

int main()
{
	system("color 0A");
	mainmenu();

	return 0;
}

void mainmenu()
{
	const char width = 25;
	while (true)
	{
		system("cls");
		cout << "THE SNAKE GAME" << endl
			<< setw(width) << "1: " << setw(width) << "START NEW GAME" << endl
			<< setw(width) << "2: " << setw(width) << "MULTIPLAYER" << endl
			<< setw(width) << "3: " << setw(width) << "EXIT GAME" << endl;
		int mode;
		mode = _getch();
		switch (mode)
		{
			case 49:
			{
				system("cls");
				startgame();
				break;
			}
			case 50:
			{
				system("cls");
				break;
			}
			case 51:
			{
				system("cls");
				system("pause");
				return;
			}
		}
	}
}

void startgame()
{
	running = true;
	player.rating = 0;
	for (int i = 0; i < MAP_SIZE_X; i++)
		for (int j = 0; j < MAP_SIZE_Y; j++)
		{
			if (i == 0 || j == 0 || i == MAP_SIZE_X - 1 || j == MAP_SIZE_Y - 1) map[i][j] = '#';
			else 
				map[i][j] = ' ';
		}
	player.x = default_x;
	player.y = default_y;
	player.apple_x = default_apple_x;
	player.apple_y = default_apple_y;
	map[player.x][player.y] = '$';
	map[player.apple_x][player.apple_y] = '@';
	drawUI();
	_getch();
	thread KeyboardThread(keyboard);
	thread MainThread(core);
	KeyboardThread.join();
	MainThread.join();
}

void core()
{
	Vertex		vertex[MAP_SIZE_X * MAP_SIZE_Y];
	vector <pair <int, int>> Vector;
	k = 1;
	Vector.push_back({player.x, player.y});
	while (direction != 0 && running)
	{
		switch (direction)
		{
		case 1:
		{
			player.x--;
			break;
		}
		case 2:
		{
			player.x++;
			break;
		}
		case 3:
		{
			player.y++;
			break;
		}
		case 4:
		{
			player.y--;
			break;
		}
		}
		if (map[player.x][player.y] == '#' || map[player.x][player.y] == '$')
		{
			running = false;
			break;
		}
		if (map[player.x][player.y] == ' ')
		{
			map[Vector[Vector.size() - 1].first][Vector[Vector.size() - 1].second] = ' ';

			for (int i = k - 1; i > 0; i--)
				Vector[i] = Vector[i - 1];
		}
		if (map[player.x][player.y] == '@')
		{
			player.rating++;
			Vector.push_back(Vector[Vector.size() - 1]);
			for (int i = Vector.size() - 1; i > 0; i--)
				Vector[i] = Vector[i - 1];
			k++;
			generateApple();
		}
		Vector[0].first = player.x;
		Vector[0].second = player.y;
		map[Vector[0].first][Vector[0].second] = '$';
		clear();
		drawUI();
		Sleep(DELAY_TIME);
	}
	cout << "GAME OVER" << endl;
	system("pause");
	system("cls");
}

void  drawMap()
{
	for (int i = 0; i < MAP_SIZE_X; i++)
	{
		for (int j = 0; j < MAP_SIZE_Y; j++)
			cout << map[i][j] << " ";
		cout << endl;
	}
}
void drawUI()
{
	cout << "************************************************************************************************************************" << endl;
	cout << "RATING: " << setw(15) << player.rating << endl;
	cout << "************************************************************************************************************************" << endl;
	drawMap();
}

void keyboard()
{
	int code = 0;
	do
	{
		char key = _getch();
		code = static_cast<int>(key);
		switch (code)
		{
			case 72:
			{
				direction = 1;
				break;
			}
			case 80:
			{
				direction = 2;
				break;
			}
			case 77:
			{
				direction = 3;
				break;
			}
			case 75:
			{
				direction = 4;
				break;
			}
		}
	} while (code != 113 && running);
	running = false;
}

void generateApple()
{
	do
	{
		player.apple_x = rand() % (MAP_SIZE_X - 2) + 1;
		player.apple_y = rand() % (MAP_SIZE_Y - 2) + 1;
	} while (map[player.apple_x][player.apple_y] == '$' || map[player.apple_x][player.apple_y] == '#');
	map[player.apple_x][player.apple_y] = '@';
}

void clear()
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };
	cout.flush();
	if (!GetConsoleScreenBufferInfo(hOut, &csbi))
	{
		abort();
	}

	SetConsoleCursorPosition(hOut, topLeft);
}
