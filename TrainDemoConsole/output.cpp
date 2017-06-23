/*输出模块*/

#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include "data_structure.h"
using namespace std;

/************声明全局变量*****************/
extern vector<Train> train;
extern Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];
extern bool pub_state [ MAXNUM_QUEUE ];
extern int train_amount;
extern FILE* file_output;
extern int stage;
extern int maxnwidth;//轨道实际使用的最大宽度 输出用
extern int maxnhight;//轨道实际使用的最大高度 输出用
extern int maxnqueue;//实际使用的队列数 减少循环用
extern bool pub_state [ MAXNUM_QUEUE ];//公轨状态数组 1表示占用 0表示空闲
/***************结束声明*****************/


void output ( void )
{
	/***********命令行打印******************/
	static int my_time = 1;
	system ( "cls" );
	printf ( "系统运行了%d秒\n*表示轨道，#表示公共轨道，$表示站台，数字表示火车！\n\n" , my_time );
	if ( stage == 1 )
	{
		printf ( "当前策略为快车优先策略！\n\n" );
	}
	else if ( stage == 2 )
	{
		printf ( "当前策略为交替策略！\n\n" );
	}
	printf ( "停止火车，输入'st'；运行火车，输入'bt'；\n\n" );
	printf ( "更改火车的速度，输入'vt'；更改火车运行方向，输入'dt'；\n\n" );
	printf ( "切换策略输入'th'，新建火车输入'nt'，修改站台数据输入'ss'；\n\n" );
	printf ( "停止全部火车输入'sa'，开始全部火车输入'ba'；\n" );
	printf ( "\n" );
	/***********打印轨道和火车***************/
	int max_pub = 0;
	for ( int i = 1; i <= maxnhight; i++ )
	{
		for ( int j = 1; j <= maxnwidth; j++ )
		{
			if ( railway [ j ] [ i ].public_id > max_pub )
			{
				max_pub = railway [ j ] [ i ].public_id;
			}
			bool flag = 0;
			for ( int k = 1; k <= train_amount; k++ )
			{
				if ( j == train [ k ].pos.x&&i == train [ k ].pos.y )
				{
					printf ( "%d" , k );
					flag = 1;
				}
			}
			if ( flag == 0 )
			{
				if ( railway [ j ] [ i ].is_station == 1 )
				{
					printf ( "$" );
				}
				else if ( railway [ j ] [ i ].train_num > 0 )
				{
					printf ( "*" );
				}
				else if ( railway [ j ] [ i ].train_num < 0 )
				{
					printf ( "#" );
				}
				else
				{
					printf ( " " );
				}
			}
		}
		printf ( "\n" );
	}
	/***********结束打印轨道和火车***************/

	printf ( "\n" );

	/**************打印火车信息***************/
	for ( int i = 1; i <= train_amount; i++ )
	{
		if ( train [ i ].start_time > 0 )
		{
			printf ( "火车%d还有%d秒开始！\n\n" , i , train [ i ].start_time );
			continue;
		}
		if ( train [ i ].state == 1 )
		{
			printf ( "火车%d被用户暂停！\n\n" , i );
			continue;
		}
		if ( train [ i ].state_station == 2 )
		{
			printf ( "火车%d正在站台停靠 剩余时间%d！\n\n" , i , railway [ train [ i ].pos.x ] [ train [ i ].pos.y ].station_time [ i ] - train [ i ].waiting_time );
			continue;
		}
		printf ( "火车%d速度为%d！" , i , train [ i ].speed );
		if ( train [ i ].direction == 0 )
		{
			printf ( "火车%d顺时针行驶！" , i );
		}
		else if ( train [ i ].direction == 1 )
		{
			printf ( "火车%d逆时针行驶！" , i );
		}
		if ( train [ i ].pub_id == -1 )
		{
			printf ( "火车%d正在等待进入公共轨道！\n" , i );
		}
		else if ( train [ i ].pub_id == 0 )
		{
			printf ( "火车%d正在非公共轨道行驶！\n" , i );
		}
		else if ( train [ i ].pub_id > 0 )
		{
			printf ( "火车%d正在公共轨道行驶！\n" , i );
		}
		printf ( "\n" );
	}
	for ( int i = 1; i < maxnqueue; i++ )
	{
		if ( pub_state [ i ] == 1 )printf ( "公轨%d被占用！\n" , i );
		else printf ( "公轨%d空闲！\n" , i );
	}
	
	/**************结束打印火车信息***************/

	/*************结束命令行打印**********/

	/**************文件打印*****************/
	
	fprintf ( file_output , "系统运行了%d秒\n" , my_time );
	for ( int i = 1; i <= train_amount; i++ )
	{
		if ( train [ i ].start_time > 0 )
		{
			fprintf ( file_output , "火车%d还有%d秒开始！\n\n" , i , train [ i ].start_time );
			continue;
		}
		if ( train [ i ].state == 1 )
		{
			fprintf ( file_output , "火车%d被用户暂停！\n\n" , i );
			continue;
		}
		fprintf ( file_output , "火车%d速度为%d！" , i , train [ i ].speed );
		if ( train [ i ].direction == 0 )
		{
			fprintf ( file_output , "火车%d顺时针行驶！" , i );
		}
		else if ( train [ i ].direction == 1 )
		{
			fprintf ( file_output , "火车%d逆时针行驶！" , i );
		}
		if ( train [ i ].pub_id == -1 )
		{
			fprintf ( file_output , "火车%d正在等待进入公共轨道！\n" , i );
		}
		else if ( train [ i ].pub_id == 0 )
		{
			fprintf ( file_output , "火车%d正在非公共轨道行驶！\n" , i );
		}
		else if ( train [ i ].pub_id > 0 )
		{
			fprintf ( file_output , "火车%d正在公共轨道行驶！\n" , i );
		}
		fprintf ( file_output , "\n" );
	}
	fflush ( file_output );
	/*************结束文件打印*****************/

	my_time++;//时间加1
}
