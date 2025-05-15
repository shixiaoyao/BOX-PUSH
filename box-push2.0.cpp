#include<stdlib.h>
#include <stdio.h>
#include<graphics.h>//asyxt图形库的头文件
#include<conio.h>
#include<stdbool.h>
#include<windows.h>

//推箱子含有的元素：空地0  墙壁1  目的地2  箱子3   玩家4  箱子+目的地 5   玩家+目的地 6
//asyxt图形库的头文件
int level = 0;
IMAGE image_all[7];
int map[3][10][10] =
{
	{
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,1,1,1,1,0,0,0,0},
	{0,0,1,2,0,1,1,1,0,0},
	{1,1,1,3,3,0,2,1,0,0},
	{1,2,3,4,3,1,1,1,0,0},
	{1,1,1,1,0,1,0,0,0,0},
	{0,0,0,1,2,1,0,0,0,0},
	{0,0,0,1,1,1,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	},
	{
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,1,1,0,0,1,1,0,0},
	{0,1,4,2,1,1,2,0,1,0},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,3,3,0,0,0,0,1},
	{0,1,0,0,0,3,0,0,1,0},
	{0,0,1,0,0,3,0,1,0,0},
	{0,0,0,1,2,2,1,0,0,0},
	{0,0,0,0,1,1,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	},
	{
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,1,1,1,1,0,0,0},
	{0,1,1,1,0,0,1,0,0,0},
	{0,1,0,0,3,2,1,0,0,0},
	{0,1,0,0,3,6,1,0,0,0},
	{0,1,0,0,3,2,1,0,0,0},
	{0,1,1,1,0,0,1,0,0,0},
	{0,0,0,1,1,1,1,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	}
};//全局变量map，用二维数组来存储地图中每个格子的元素

enum state//枚举，从0开始递增的常量值，方便后续用switch匹配
{
	SPACE,//空地0
	WALL,//墙1
	DEST,//目的地2
	BOX,//箱子3
	PLAYER//玩家4
};

void clearInputBuffer()
{
	while (_kbhit()) _getch();  // 清空所有未处理的键盘输入
}

void image()//加载图片的函数
{

	for (int i = 0; i < 7; i++)
	{
		char noname[20] = "";
		sprintf(noname, "./images/%d.jpg", i);
		loadimage(image_all + i, noname, 64, 64);//参数分别是第几张，路径。分辨率
		putimage(i * 64, 0, image_all + i);
	}
}


void gamedraw()//地图绘制函数
{

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int x = j * 64;
			int y = i * 64;
			switch (map[level][i][j])
			{
			case SPACE:
				putimage(x, y, image_all);
				break;
			case WALL:
				putimage(x, y, image_all + 1);
				break;
			case DEST:
				putimage(x, y, image_all + 2);
				break;
			case BOX:
				putimage(x, y, image_all + 3);
				break;
			case PLAYER:
				putimage(x, y, image_all + 4);
				break;
			case BOX + DEST:
				putimage(x, y, image_all + 5);
				break;
			case PLAYER + DEST:
				putimage(x, y, image_all + 6);
				break;
			}


		}

	}
}
void keyEvent()
{
	int i = 0, j = 0;//记录每次按下键盘后人的位置
	for (i; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (map[level][i][j] == 4 || map[level][i][j] == 6)//注意人有在目的地和在空地两种可能
			{
				goto end;
			}

		}
	}
end:;

	//方向键的上下左右是72 80 75 77
	int ch = _getch();
	if (ch == 0xE0 || ch == 0) { // 处理方向键
		ch = _getch();
	}
	if (i < 0 || i >= 10 || j < 0 || j >= 10) return;
	switch (ch)
	{
		//往上移动，分为前面是空地，目的地（空地和目的地归为一类） 或前面是箱子两种情况（前面是墙壁则不改变）
	case 72:
	case'w':
	case'W':
		if (i > 0)
		{
			if (map[level][i - 1][j] == SPACE || map[level][i - 1][j] == DEST)//前方为空地时,只需要处理原来人在的地方和下一步人在的地方
			{
				map[level][i - 1][j] += PLAYER;
				map[level][i][j] -= PLAYER;
			}
			else if ((map[level][i - 1][j] == BOX || map[level][i - 1][j] == BOX + DEST) && i > 1)//前方为箱子时则还要考虑箱子的前方是否能移动
			{
				if (map[level][i - 2][j] == SPACE || map[level][i - 2][j] == DEST)
				{
					map[level][i - 2][j] += BOX;
					map[level][i - 1][j] = map[level][i - 1][j] + PLAYER - BOX;
					map[level][i][j] -= PLAYER;
				}
			}
		}
		break;
		//上下左右四个方向判断基本相同
	case 80:
	case's':
	case'S':
		if (i < 9)
		{
			if (map[level][i + 1][j] == SPACE || map[level][i + 1][j] == DEST)//前方为空地时,只需要处理原来人在的地方和下一步人在的地方
			{
				map[level][i + 1][j] += PLAYER;
				map[level][i][j] -= PLAYER;
			}
			else if ((map[level][i + 1][j] == BOX || map[level][i + 1][j] == BOX + DEST) && i < 8)//前方为箱子时则还要考虑箱子的前方是否能移动
			{
				if (map[level][i + 2][j] == SPACE || map[level][i + 2][j] == DEST)
				{
					map[level][i + 2][j] += BOX;
					map[level][i + 1][j] = map[level][i + 1][j] + PLAYER - BOX;
					map[level][i][j] -= PLAYER;
				}
			}
		}
		break;

	case 75:
	case'a':
	case'A':
		if (j > 0)
		{
			if (map[level][i][j - 1] == SPACE || map[level][i][j - 1] == DEST)//前方为空地时,只需要处理原来人在的地方和下一步人在的地方
			{
				map[level][i][j - 1] += PLAYER;
				map[level][i][j] -= PLAYER;
			}
			else if ((map[level][i][j - 1] == BOX || map[level][i][j - 1] == BOX + DEST) && j > 1)//前方为箱子时则还要考虑箱子的前方是否能移动
			{
				if (map[level][i][j - 2] == SPACE || map[level][i][j - 2] == DEST)
				{
					map[level][i][j - 2] += BOX;
					map[level][i][j - 1] = map[level][i][j - 1] + PLAYER - BOX;
					map[level][i][j] -= PLAYER;
				}
			}
		}
		break;
	case 77:
	case'd':
	case'D':
		if (j < 9)
		{
			if (map[level][i][j + 1] == SPACE || map[level][i][j + 1] == DEST)//前方为空地时,只需要处理原来人在的地方和下一步人在的地方
			{
				map[level][i][j + 1] += PLAYER;
				map[level][i][j] -= PLAYER;
			}
			else if ((map[level][i][j + 1] == BOX || map[level][i][j + 1] == BOX + DEST) && j < 8)//前方为箱子时则还要考虑箱子的前方是否能移动
			{
				if (map[level][i][j + 2] == SPACE || map[level][i][j + 2] == DEST)
				{
					map[level][i][j + 2] += BOX;
					map[level][i][j + 1] = map[level][i][j + 1] + PLAYER - BOX;
					map[level][i][j] -= PLAYER;
				}
			}
		}
		break;

	}
}


bool judge()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[level][i][j] == 3)
			{
				return false;
			}
		}
	}
	return true;
}

void drawCenteredText(int y, const char* text, int fontSize = 36, COLORREF color = BLACK) {
	// 计算文字尺寸
	settextstyle(fontSize, 0, "楷体");
	int width = textwidth(text);
	int height = textheight(text);
	int x = (640 - width) / 2;

	// 智能背景处理
	if (color == WHITE) {
		setfillcolor(0x3F000000); // 深色背景
		solidrectangle(x - 10, y - 5, x + width + 10, y + height + 5);
	}

	// 绘制阴影
	settextcolor(0x7F000000);
	outtextxy(x + 2, y + 2, text);

	// 绘制主文字
	settextcolor(color);
	outtextxy(x, y, text);
}
bool continuePlay()
{

	// 添加半透明背景
	setfillcolor(0x7F000000);
	solidrectangle(0, 280, 640, 360);

	// 绘制艺术字
	drawCenteredText(300, "本关完成！继续下一关吗？", 36, YELLOW);
	drawCenteredText(340, "(按 Y/N 选择)", 24, 0x00FF00);


	FlushBatchDraw();

	char ch = _getch();
	while (1)
	{
		if (ch == 'Y' || ch == 'y')
		{
			clearInputBuffer();
			return true;
		}
		else if (ch == 'N' || ch == 'n')
		{
			return false;
		}
		ch = _getch();//若输入无效（不是大小写的y,n)则仍会被清空
	}

}


int main()
{
	initgraph(10 * 64, 10 * 64);
	image();

	while (level < 3)
	{
		cleardevice();
		gamedraw();
		FlushBatchDraw();

		bool levelWon = false;
		while (!levelWon)
		{
			// 处理输入事件
			if (_kbhit())
			{
				keyEvent();
				cleardevice();
				gamedraw();
				FlushBatchDraw();
				levelWon = judge();
			}
			Sleep(20); // 避免CPU占用过高
		}

		// 胜利处理
		cleardevice();
		gamedraw();
		FlushBatchDraw();
		Sleep(100);

		if (level == 2)
		{
			// 最后一关提示
			setfillcolor(0x7F000000);
			solidrectangle(0, 0, 640, 640);
			drawCenteredText(280, "恭 喜 通 关！", 48, 0xFFD700);
			drawCenteredText(340, "按任意键退出", 32, 0x333333);
			FlushBatchDraw();
			_getch();
			break;
		}
		else
		{
			if (continuePlay())
			{
				level++;
			}
			else
			{
				break;
			}
		}
	}

	closegraph();
	return 0;
}