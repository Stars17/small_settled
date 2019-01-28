#include<iostream>
#include<cstdlib>
#include<ctime>
#include <conio.h>
#include<vector>
#include<string>

#define MAX_LENGTH 101
#define PLAYER -3
#define WALL -2
#define SPACE -1
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3
#define NO_DIRECTION 4

using namespace std;

class point
{
public:
	int x;
	int y;
	int state;

	point() { x = 0; y = 0; state = NO_DIRECTION; }
	point(int _x, int _y, int _state = NO_DIRECTION) { x = _x; y = _y; state = _state; }
	bool operator==(const point &p) { return p.x == x && p.y == y; }
	bool operator!=(const point &p) { return p.x != x || p.y != y; }
	point operator+=(const point &p) { point temp_point(x + p.x, y + p.y); return temp_point; }
};

class maze
{
public:
	int length;
	int map[2 * MAX_LENGTH + 1][2 * MAX_LENGTH + 1];
	point start_point, end_point;
	point direction[4] = { {0,1}, {1,0},{0,-1},{-1,0} };

	maze(int difficulty);//初始化
	void prim();//生成迷宫
	void print();//输出迷宫
	void game();//玩游戏
	void DFS(bool show_process);//深度优先搜索

	bool not_visited(vector<point> &list, point &p);//是否在列表中
	bool legal(point &p, int length);//点是否合法
};
bool maze::not_visited(vector<point> &list, point &p)
{
	for (unsigned int i = 0; i < list.size(); i++)
		if (list[i].x == p.x&&list[i].y == p.y)
			return 0;
	return 1;
}
bool maze::legal(point &p, int _length)
{
	return p.x >= 0 && p.x <= _length - 1 && p.y >= 0 && p.y <= _length - 1;
}
maze::maze(int difficulty)
{
	length = difficulty;
	point start(1, 0, RIGHT), end(2 * length - 1, 2 * length, RIGHT);
	start_point = start;
	end_point = end;
	for (int x = 0; x < 2 * length + 1; x++)
		for (int y = 0; y < 2 * length + 1; y++)
			map[x][y] = WALL;
	for (int x = 0; x < length; x++)
		for (int y = 0; y < length; y++)
			map[2 * x + 1][2 * y + 1] = SPACE;
	map[start_point.x][start_point.y] = SPACE;
	map[end_point.x][end_point.y] = SPACE;
	prim();
}
void maze::prim()
{
	point P(0, 0);
	vector<point> list, temp_list;
	list.push_back(P);
	while (list.size() < length*length)
	{
		temp_list.clear();
		for (int i = 0; i < 4; i++)
		{
			point temp_point = (P += direction[i]);
			if (legal(temp_point, length) && not_visited(list, temp_point))
				temp_list.push_back(temp_point);
		}
		if (temp_list.size() == 0)
			P = list[rand() % list.size()];
		else
		{
			point new_point = temp_list[rand() % temp_list.size()];
			list.push_back(new_point);
			map[P.x + 1 + new_point.x][P.y + 1 + new_point.y] = SPACE;
			P = new_point;
		}
	}
}
void maze::print()
{
	system("cls");
	for (int x = 0; x < 2 * length + 1; x++)
	{
		for (int y = 0; y < 2 * length + 1; y++)
			switch (map[x][y])
			{
			case PLAYER:cout << "●"; break;
			case WALL:cout << "█"; break;
			case SPACE:cout << "  "; break;
			case UP:cout << "↑"; break;
			case DOWN:cout << "↓"; break;
			case LEFT:cout << "←"; break;
			case RIGHT:cout << "→"; break;
			}
		cout << "\n";
	}
}
void maze::game()
{
	point now_point = start_point;
	map[now_point.x][now_point.y] = PLAYER;
	print();
	char ch;
	while (now_point != end_point)
	{
		if (_kbhit())
		{
			map[now_point.x][now_point.y] = SPACE;
			switch (ch = _getch())
			{
			case 'w':if (now_point.x != 0)if (map[now_point.x - 1][now_point.y] == SPACE)now_point.x--; break;
			case 's':if (now_point.x != 2 * length)if (map[now_point.x + 1][now_point.y] == SPACE)now_point.x++; break;
			case 'a':if (now_point.y != 0)if (map[now_point.x][now_point.y - 1] == SPACE)now_point.y--; break;
			case 'd':if (now_point.y != 2 * length)if (map[now_point.x][now_point.y + 1] == SPACE)now_point.y++; break;
			default:break;
			}
			map[now_point.x][now_point.y] = PLAYER;
			print();
		}
	}
	cout << "\nYOU WIN!\n";
	_getch();
}
void maze::DFS(bool show_process)
{
	vector<point> path;
	path.push_back(start_point);
	while (path.back() != end_point)
	{
		if (path.back().state != NO_DIRECTION)
		{
			map[path.back().x][path.back().y] = path.back().state;
			point temp_point = (path.back() += direction[path.back().state]);
			temp_point.state = RIGHT;
			path.back().state++;
			if (legal(temp_point, 2 * length + 1) && not_visited(path, temp_point))
				if (map[temp_point.x][temp_point.y] != WALL)
					path.push_back(temp_point);
		}
		else
		{
			map[path.back().x][path.back().y] = SPACE;
			path.pop_back();
		}
		if (show_process)
		{
			system("cls");
			print();
		}
	}
	system("cls");
	print();
}

int main()
{
	srand((unsigned int)time(NULL));

	cout << "\"<DFS/GAME>[char D/G] + <maze size>[int] + {<show DFS process>[bool] / <gamemode>[bool]}\"\n";
	cout << "For example: \"D 5 0\" mean DFS mode ( maze size=5, hide DFS process )\n";
	cout << "For example: \"G 7 1\" mean game mode ( maze size=7, break through game )\n";
	cout << "Note: The maximum maze size is 100!\n";

	string DorG;
	int size;
	bool mode;
	cin >> DorG >> size >> mode;
	if (DorG == "D"&&size >= 1 && size <= 100)
	{
		while (1)
		{
			maze M(size);
			M.DFS(mode);
			getchar();
		}
	}
	else if (DorG == "G"&&size >= 1 && size <= 100)
	{
		if (mode)
		{
			int difficulty = 1;
			while (difficulty < 100)
			{
				maze M(difficulty++);
				M.game();
				system("cls");
			}
		}
		else
		{
			maze M(size);
			M.game();
		}
	}
	else
		cout << "Error!";
}