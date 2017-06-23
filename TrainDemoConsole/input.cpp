/*输入模块*/
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <queue>
#include <functional>
#include<windows.h>
#include "data_structure.h"
using namespace std;
//声明函数
extern void train_init ( int train_num );
extern void check_pub ( void );
extern void station_init ( void );
//声明变量
extern vector<Train> train;
extern Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];
extern bool have_input;//线程间通信用
extern int train_amount;//实际火车数量
extern FILE* file_output;//标准输出文件
extern int stage;//策略 速度优先为1 交替策略为2
extern vector <int > pub_speed [ MAXNUM_QUEUE ];
extern vector <int > pub_byturn [ MAXNUM_QUEUE ];

void input ( void )
{
	char in1 , in2;
	scanf ( "%c" , &in1 );
	if ( in1 == '\n' ) return;
	scanf ( "%c" , &in2 );
	getchar ( );
	if ( in1 == 's'&&in2 == 't' )//停车
	{
		printf ( "请输入要停止的火车编号，输入0返回：" );
		int id;
		scanf ( "%d" , &id );
		if ( id == 0 ) return;//输入0返回

		train [ id ].speed = 0;
		train [ id ].state = 1;

		//删除公轨队列里面的这辆火车
		if ( train [ id ].pub_id == -1 )
		{
			for ( int i = 1; i <= MAXNUM_QUEUE; i++ )
			{
				if ( stage == 1 )//速度优先策略
				{
					vector<int>::iterator result = find ( pub_speed [ i ].begin ( ) , pub_speed [ i ].end ( ) , id );
					if ( result != pub_speed [ i ].end ( ) )
					{
						pub_speed [ i ].erase ( result );
					}
				}
				else if ( stage == 2 )//交替策略
				{
					vector<int>::iterator result = find ( pub_byturn [ i ].begin ( ) , pub_byturn [ i ].end ( ) , id );
					if ( result != pub_byturn [ i ].end ( ) )
					{
						pub_byturn [ i ].erase ( result );
					}
				}
			}
		}
		fprintf ( file_output , "火车%d被用户停止！\n\n" , id );
	}
	else if ( in1 == 'b'&&in2 == 't' )//开车
	{
		have_input = 1;
		int id;
		printf ( "请输入要开始的火车，输入0返回：" );
		scanf ( "%d" , &id );
		if ( id == 0 ) return; //输入0返回

		train [ id ].speed = train [ id ].speed_init;
		train [ id ].state = 0;
		//不需要重新维护队列 因为main先运行central_control在运行火车
		fprintf ( file_output , "火车%d被用户启动！\n\n" , id );
		have_input = 0;
	}
	else if ( in1 == 'v'&&in2 == 't' )//改速度
	{
		have_input = 1;
		int temp;
		printf ( "请输入要修改速度的火车编号，输入0返回：" );
		scanf ( "%d" , &temp );
		if ( temp == 0 ) return;//输入0返回

		printf ( "火车%d目前的速度是%d，请输入火车%d的新速度：" , temp , train [ temp ].speed_init , temp );
		scanf ( "%d" , &( train [ temp ].speed_init ) );
		if ( train [ temp ].speed != 0 )
		{
			train [ temp ].speed = train [ temp ].speed_init;
		}
		train [ temp ].next_pos.x = train [ temp ].pos.x;
		train [ temp ].next_pos.y = train [ temp ].pos.y;
		fprintf ( file_output , "火车%d被用户改变速度！新速度为%d！\n\n" , temp , train [ temp ].speed_init );
		have_input = 0;
	}
	else if ( in1 == 'd'&&in2 == 't' )//改方向
	{
		have_input = 1;
		printf ( "请输入要修改方向的火车编号，输入0返回：" );
		int id;
		scanf ( "%d" , &id );
		if ( id == 0 ) return;//输入0返回

		if ( train [ id ].direction == 0 )
		{
			printf ( "火车%d目前的方向是顺时针，请输入火车新方向：" , id );
		}
		else if ( train [ id ].direction == 1 )
		{
			printf ( "火车%d目前的方向是逆时针，请输入火车新方向：" , id );
		}
		scanf ( "%d" , &( train [ id ].direction ) );
		/*如果被改方向的火车在等待进入共轨 那么需要重新维护队列*/
		if ( train [ id ].pub_id == -1 )
		{
			for ( int i = 1; i <= MAXNUM_QUEUE; i++ )
			{
				if ( stage == 1 )//速度优先策略
				{
					vector<int>::iterator result = find ( pub_speed [ i ].begin ( ) , pub_speed [ i ].end ( ) , id );
					if ( result != pub_speed [ i ].end ( ) )
					{
						pub_speed [ i ].erase ( result );
					}
				}
				else if ( stage == 2 )//交替策略
				{
					vector<int>::iterator result = find ( pub_byturn [ i ].begin ( ) , pub_byturn [ i ].end ( ) , id );
					if ( result != pub_byturn [ i ].end ( ) )
					{
						pub_byturn [ i ].erase ( result );
					}
				}
			}
		}
		//重新写入next_pos
		train [ id ].next_pos.x = train [ id ].pos.x;
		train [ id ].next_pos.y = train [ id ].pos.y;
		if ( railway [ train [ id ].next_pos.x ] [ train [ id ].next_pos.y ].train_num != -1 && train [ id ].state_station == 0 )
		{
			train [ id ].speed = train [ id ].speed_init;
		}
		fprintf ( file_output , "火车%d被用户改变方向！\n\n" , id );
		have_input = 0;
	}
	else if ( in1 == 't'&&in2 == 'h' )//改策略
	{
		if ( stage == 1 )
		{
			printf ( "当前策略是快车优先！切换到交替策略请输入1，否则任意输入：" );
			char a;
			scanf ( "%c" , &a );
			if ( a == '1' )
			{
				stage = 2;
				for ( int i = 0; i < MAXNUM_QUEUE; i++ )
				{
					pub_speed [ i ].clear ( );
				}
				printf ( "控制策略切换！新的控制策略是交替策略！\n\n" );
			}
		}
		else if ( stage == 2 )
		{
			printf ( "当前策略是交替策略！切换到快车优先请输入1，否则任意输入：" );
			char a;
			scanf ( "%c" , &a );
			if ( a == '1' )
			{
				stage = 1;
				for ( int i = 0; i < MAXNUM_QUEUE; i++ )
				{
					pub_byturn [ i ].clear ( );
				}
				printf ( "控制策略切换！新的控制策略是速度优先策略！\n\n" );
			}
		}
	}
	else if ( in1 == 'n'&&in2 == 't' )//创建火车
	{
		Train temp;
		train.push_back ( temp );
		train_amount++;
		train_init ( train_amount );
		check_pub ( );
		station_init ( );
		printf ( "用户创建了一个火车！\n\n" );
	}
	else if ( in1 == 's'&&in2 == 's' )//站台
	{
		station_init ( );
	}
	else if ( in1 == 's'&&in2 == 'a' )//停下所有火车
	{
		for ( int i = 1; i <= train_amount; i++ )
		{
			train [ i ].speed = 0;
			train [ i ].state = 1;
		}
	}
	else if ( in1 == 'b'&&in2 == 'a' )//开始所有火车
	{
		for ( int i = 1; i <= train_amount; i++ )
		{
			train [ i ].speed = train [ i ].speed_init;
			train [ i ].state = 0;
		}
	}
}