#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<stdbool.h>

#define MAP_WIDTH 118
#define MAP_HEIGHT 29
#define MAXLEN 100  //蛇身体最大长度

struct SNAKE{
    int len;  //蛇身长度
    int speed;  //移动速度
    int x[MAXLEN];  //组成蛇身的小方块的横坐标
    int y[MAXLEN];  //组成蛇身的小方块的纵坐标
} snake;

struct FOOD{
    int x;
    int y;
} food;

void gotoxy(int, int);
void Create_MAP();
void Random_Food();
void Move_Snake();
bool Snake_Status();

int Sum_Score = 0;  //玩家得分
int Change_Sign = 0;  //判断有没有吃到食物
int key = 75;

void gotoxy(int x,int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD point;
    point.X = x;
    point.Y = y;
    SetConsoleCursorPosition(handle, point);
}

void Create_MAP()
{
    //'■'在x轴上占两个位置，在y轴上占一个位置
    for (int i = 0; i <= MAP_WIDTH;i += 2)  //上下边界
    {
        gotoxy(i, 0);
        printf("■");
        gotoxy(i, MAP_HEIGHT);
        printf("■");
    }
    for (int i = 1; i < MAP_HEIGHT;i += 1)  //左右边界
    {
        gotoxy(0, i);
        printf("■");
        gotoxy(MAP_WIDTH, i);
        printf("■");
    }
    //生成初始食物
    while (1)
	{
		srand((unsigned int)time(NULL));
		food.x = rand() % (MAP_WIDTH - 4) + 2;
		food.y = rand() % (MAP_HEIGHT - 2) + 1;
		//生成的食物横坐标的奇偶必须和初试时蛇头所在坐标的奇偶一致，因为一个字符占两个字节位置，若不一致
		//会导致吃食物的时候只吃到一半
		if (food.x % 2 == 0)
			break;
	}
	//将光标移到食物的坐标处打印食物
	gotoxy(food.x, food.y);
	printf("★");
    //打印初始的蛇身
    snake.len = 5;
    snake.speed = 200;
    snake.x[0] = MAP_WIDTH / 2 + 1;
    snake.y[0] = MAP_HEIGHT / 2;
    gotoxy(snake.x[0], snake.y[0]);
    printf("■");
    for (int i = 1;i < snake.len;i++)
    {
        snake.x[i] = snake.x[i - 1] + 2;
        snake.y[i] = snake.y[i - 1];
        gotoxy(snake.x[i], snake.y[i]);
        printf("■");
    }
    gotoxy(MAP_WIDTH, 0);
    return;
}

void Random_Food()  //随机生成食物
{
    if(snake.x[0] == food.x && snake.y[0] == food.y) {  //蛇头与食物重叠，则需要在生成一个新的食物
        while(1)
        {
            int sign = 1;  //检验生成的食物是否在蛇身上
            srand((unsigned)time(NULL));
            food.x = rand() % (MAP_WIDTH - 4) + 2;
            food.y = rand() % (MAP_HEIGHT - 2) + 1;
            for (int i = 0; i < snake.len;i++)
            {
                if(food.x != snake.x[i] || food.y != snake.y[i])
                    continue;
                else{
                    sign = 0;
                    break;
                }    
            }
            if(sign && food.x % 2 == 0)
                break;
        }
    gotoxy(food.x, food.y);
    printf("★");
    Change_Sign = 1;
    snake.len++;
    Sum_Score += 10;
    snake.speed -= 5;
    }
    return;
}

bool Snake_Status()  //判断蛇的状态是否合法
{
    if(snake.y[0] == 0 || snake.y[0] == MAP_HEIGHT)
        return false;
    if(snake.x[0] == 0 || snake.x[0] == MAP_WIDTH)
        return false;
    for(int i = 1; i < snake.len;i++)
    {
        if(snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0])
            return false;
    }
    return true;
}

/*
*控制台按键所代表的数字
	*“↑”：72
	*“↓”：80
	*“←”：75
	*“→”：77
*/
void Move_Snake()
{
    int pre_key = key;
    if(kbhit()) {
        fflush(stdin);
        key = getch();
        key = getch();
    }
    if(Change_Sign == 0)  //没吃到食物，擦去蛇尾
    {
        gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
        printf("  ");
    }
    for (int i = snake.len - 1; i > 0;i--)  //除去蛇头向前移动
    {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }
    //由于后一次运动的方向不能与前一次相反，所以如果后一次与前一次相反，后一次运动方向改为前一次
    if(pre_key == 72 && key == 80)
        key = 72;
    if(pre_key == 80 && key == 72)
        key = 80;
    if(pre_key == 75 && key == 77)
        key = 75;
    if(pre_key == 77 && key == 75)
        key = 77;
    switch(key)
    {
        case 75:
            snake.x[0] -= 2;
            break;
        case 77:
            snake.x[0] += 2;
            break;
        case 72:
            snake.y[0] -= 1;
            break;
        case 80:
            snake.y[0] += 1;
            break;
    }
    gotoxy(snake.x[0], snake.y[0]);
    printf("■");
    Change_Sign = 0;
    gotoxy(MAP_WIDTH, 0);
    return;
}

int main()
{
    Create_MAP();
    while(1)
    {
        Move_Snake();
        if(!Snake_Status())
            break;
        Random_Food();
        Sleep(snake.speed);
    }
    gotoxy(MAP_WIDTH / 2 + 1 ,MAP_HEIGHT / 2);
    printf("Game Over!\n");
    gotoxy(MAP_WIDTH / 2, MAP_HEIGHT / 2 + 1);
    printf("本次游戏得分:%d", Sum_Score);
    Sleep(5000);
    return 0;
}