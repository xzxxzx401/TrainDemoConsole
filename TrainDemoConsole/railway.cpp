#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include "data_structure.h"

using namespace std;

//声明变量
extern Railway railway [ MAXNUM_RAILWAY ] [ MAXNUM_RAILWAY ];
extern Train train [ MAXNUM_TRAIN_NUM ];

void railway_init ( int train_num )
{
	
	printf ( "请按顺时针输入火车%d轨道顶点坐标x y，以-1 -1结束\n" , train_num );
	int ax , ay , bx , by , x0 , y0;
	scanf ( "%d%d" , &x0 , &y0 );
	getchar ( );
	ax = x0 , ay = y0;
	bool flag = 1;
	int i , j;

	while ( flag )
	{
		scanf ( "%d%d" , &bx , &by );
		getchar ( );
		if ( bx == -1 && by == -1 )
		{
			bx = x0 , by = y0;
			flag = 0;
		}
		if ( bx == ax )
		{
			//判断火车运行方向 定义i以处理轨道拐点问题
			if ( train [ train_num ].direction == 0 )
			{
				if ( ay > by )
				{
					i = by + 1 , j = ay;
				}
				else if ( ay < by )
				{
					i = ay , j = by - 1;
				}
			}
			else if ( train[train_num].direction == 1 )
			{
				if ( ay > by )
				{
					i = by , j = ay - 1;
				}
				else if ( ay < by )
				{
					i = ay + 1 , j = by;
				}
			}
			for ( ; i <= j; i++ )
			{
				//判断公共轨道

				/*如果已经是公共轨道 train_num=-1 那就把对应的is_public置为1
				如果没有轨道train_num=0 那就把train_num置为对应序号
				如果已经是某个火车独享的轨道 就是train_num>=1 那么吧train_num置为-1 变成公共轨道 对应的is_public置为1*/

				if ( railway [ ax ] [ i ].train_num == -1 )
				{
					railway [ ax ] [ i ].is_public [ train_num ] = 1;
				}
				else if ( railway [ ax ] [ i ].train_num == 0 )
				{
					railway [ ax ] [ i ].train_num = train_num;
				}
				else
				{
					railway [ ax ] [ i ].train_num = -1;
					railway [ ax ] [ i ].is_public [ train_num ] = 1;
					for ( int kk = 0; kk < MAXNUM_TRAIN_NUM; kk++ )
					{
						if ( railway [ ax ] [ i ].direction [ kk ] > 0 )
						{
							railway [ ax ] [ i ].is_public [ kk ] = 1;
						}
					}
				}
				//判断方向
				if ( ay > by )
				{
					if ( train[train_num].direction == 0 )
					{
						railway [ ax ] [ i ].direction [ train_num ] = 4;
					}
					else if ( train[train_num].direction == 1 )
					{
						railway [ ax ] [ i ].direction [ train_num ] = 2;
					}
				}
				else if ( ay < by )
				{
					if ( train[train_num].direction == 0 )
					{
						railway [ ax ] [ i ].direction [ train_num ] = 2;
					}
					else if ( train[train_num].direction == 1 )
					{
						railway [ ax ] [ i ].direction [ train_num ] = 4;
					}
				}
			}
			ax = bx , ay = by;
		}
		else if ( by == ay )
		{
			//判断火车运行方向 定义i以处理轨道拐点问题
			if ( train[train_num].direction == 0 )
			{
				if ( ax > bx )
				{
					i = bx + 1 , j = ax;
				}
				else if ( ax < bx )
				{
					i = ax , j = bx - 1;
				}

			}
			else if ( train[train_num].direction == 1 )
			{
				if ( ax > bx )
				{
					i = bx , j = ax - 1;
				}
				else if ( ax < bx )
				{
					i = ax + 1 , j = bx;
				}
			}

			for ( ; i <= j; i++ )
			{
				//判断公共轨道
				if ( railway [ i ] [ ay ].train_num == -1 )
				{
					railway [ i ] [ ay ].is_public [ train_num ] = 1;
				}
				else if ( railway [ i ] [ ay ].train_num == 0 )
				{
					railway [ i ] [ ay ].train_num = train_num;
				}
				else
				{
					railway [ i ] [ ay ].train_num = -1;
					railway [ i ] [ ay ].is_public [ train_num ] = 1;
					for ( int kk = 0; kk < MAXNUM_TRAIN_NUM; kk++ )
					{
						if ( railway [ i ] [ ay ].direction [ kk ] > 0 )
						{
							railway [ i ] [ ay ].is_public [ kk ] = 1;
						}
					}
				}
				//判断方向
				if ( ax > bx )
				{
					if ( train[train_num].direction == 0 )
					{
						railway [ i ] [ ay ].direction [ train_num ] = 3;
					}
					else if ( train[train_num].direction == 1 )
					{
						railway [ i ] [ ay ].direction [ train_num ] = 1;
					}
				}
				else if ( ax < bx )
				{
					if ( train[train_num].direction == 0 )
					{
						railway [ i ] [ ay ].direction [ train_num ] = 1;
					}
					else if ( train[train_num].direction == 1 )
					{
						railway [ i ] [ ay ].direction [ train_num ] = 3;
					}
				}
			}
			ax = bx , ay = by;
		}
		else
		{
			printf ( "ERROR Re-input\n" );
		}
	}
}
