#include "stm32f10x.h"
#include "ili9320.h"
#include "ili9320_api.h"
#include <stdio.h>
#include "ARMJISHU_TouchScreen_ADS7843.h"
#include "picture.h"
#include "food.h"
#include "SNAKE.h"
#include "BODY.h"
#include "GAMEOVER.h"
#define NONE					0
#define LEFT					1
#define RIGHT					2
#define UP						3
#define DOWN					4
#define GAMEOVER			1
#define GAMEWAITING   2
#define GAMEING				3
#define GAMEPAUSE			4
#define FOOD	1
#define NOFOOD 0


typedef struct
{	
	u8 Snake_X;
	u8 Snake_Y;
	u8 Snake_Type;    //蛇头：1，蛇身：2，蛇尾：3  
}Snake_In[200];

u8 DisplayBuffer[192]={0};	//显示缓存,右上为[0,0],左下为[11,15]
//0:空，1：蛇，2：食物
u8 StartFlag=GAMEOVER;	//游戏开始标志
uint8_t MoveDirection=DOWN;	//移动方向
uint8_t preMoveDirection=DOWN;	//移动方向
uint8_t FoodCoordinate_x,FoodCoordinate_y;	//食物坐标
uint16_t SnakeLenth;	//蛇身长度
uint16_t SnakeCoordinate[192];	//蛇身坐标,高八位放x轴坐标,低8位放y轴坐标
u8 eatFood=0;
u8 snake_time=0,body_time=0;
uint32_t seed=0;

void gamePicInit(void);
void snake_move(void);
void Init_Snake(void);
void showSnakeHead(uint16_t head);
void showSnakeBody(uint16_t body);
void showSnakeEnd(uint16_t end);
void showFood(uint8_t FoodCoordinate_x,uint8_t FoodCoordinate_y);
void gameMain(void);
int Check_Coordinate(int x,int y);
void Move_Snake(uint8_t x,uint8_t y,uint8_t state);
void newShow(void);
uint8_t endJudge(void);
void createFood(void);
int myRand(void);