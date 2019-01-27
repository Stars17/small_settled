#include<iostream>
#include<ctime>
#include<vector>
#include<cstdlib>
#include <conio.h>
#define MAX_LENGTH 100
using namespace std;

class point { public: int x; int y; point(int X, int Y) { x = X; y = Y; } };
class maze
{
public:
	int LENGTH;//边长
	int map[2 * MAX_LENGTH + 1][2 * MAX_LENGTH + 1];//0为墙
	vector<point> list, list_temp;//访问列表
	int now_x = 1, now_y = 0;//当前位置

	maze(int difficulty);//初始化
	void prim();//生成迷宫
	void print();//输出迷宫
	void game();//玩游戏

	bool not_visited(point p);
};
bool maze::not_visited(point p)
{
	for (int i = 0; i < list.size(); i++)
		if (list[i].x == p.x&&list[i].y == p.y)
			return 0;
	return 1;
}
maze::maze(int difficulty)
{
	LENGTH = difficulty;
	for (int i = 0; i < 2 * LENGTH + 1; i++)
		for (int j = 0; j < 2 * LENGTH + 1; j++)
			map[i][j] = 0;
	for (int i = 0; i < LENGTH; i++)
		for (int j = 0; j < LENGTH; j++)
			map[2 * i + 1][2 * j + 1] = 1;
	map[1][0] = 1;
	map[2 * LENGTH - 1][2 * LENGTH] = 1;
	prim();
}
void maze::prim()
{
	srand(time(NULL));
	point P(0, 0);
	list.push_back(P);
	while (list.size() < LENGTH*LENGTH)
	{
		list_temp.clear();
		point P_left(P.x - 1, P.y), P_right(P.x + 1, P.y), P_up(P.x, P.y - 1), P_down(P.x, P.y + 1);
		if (P.x != 0 && not_visited(P_left))list_temp.push_back(P_left);
		if (P.x != LENGTH - 1 && not_visited(P_right))list_temp.push_back(P_right);
		if (P.y != 0 && not_visited(P_up))list_temp.push_back(P_up);
		if (P.y != LENGTH - 1 && not_visited(P_down))list_temp.push_back(P_down);
		if (list_temp.size() == 0)
			P = list[rand() % list.size()];
		else
		{
			point new_point = list_temp[rand() % list_temp.size()];
			list.push_back(new_point);
			map[P.x + 1 + new_point.x][P.y + 1 + new_point.y] = 1;
			P = new_point;
		}
	}
}
void maze::print()
{
	for (int i = 0; i < 2 * LENGTH + 1; i++)
	{
		for (int j = 0; j < 2 * LENGTH + 1; j++)
			if (i == now_x && j == now_y)
				cout << "●";
			else if (map[i][j] == 0)
				cout << "█";
			else
				cout << "  ";
		cout << "\n";
	}
}
void maze::game()
{
	print();
	char ch;
	int x = 0, y = 0;
	while (1)
	{
		if (now_x == 2 * LENGTH - 1 && now_y == 2 * LENGTH)
			break;
		if (_kbhit())
		{
			switch (ch = _getch())
			{
			case 'w':if (now_x != 0)if (map[now_x - 1][now_y] == 1)now_x--; break;
			case 's':if (now_x != 2 * LENGTH)if (map[now_x + 1][now_y] == 1)now_x++; break;
			case 'a':if (now_y != 0)if (map[now_x][now_y - 1] == 1)now_y--; break;
			case 'd':if (now_y != 2 * LENGTH)if (map[now_x][now_y + 1] == 1)now_y++; break;
			default:break;
			}
			system("cls");
			print();
		}
	}
	cout << "\nYOU WIN!\n";
	_getch();
}

int main()
{
	int difficulty = 1;
	while (1)
	{
		maze M(difficulty++);
		M.game();
		system("cls");
	}
}