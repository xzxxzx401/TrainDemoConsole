/*站台模块*/
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "data_structure.h"

using namespace std;

/************声明全局变量*****************/
extern vector<Train> train;
extern Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];
extern int train_amount;
extern int maxnwidth;//轨道实际使用的最大宽度 输出用
extern int maxnhight;//轨道实际使用的最大高度 输出用
/************结束声明全局变量*************/

void station_init ( void )
{
	system ( "cls" );
	/***********打印轨道和火车***************/
	for ( int i = 1; i <= maxnhight; i++ )
	{
		for ( int j = 1; j <= maxnwidth; j++ )
		{
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
				if ( railway [ j ] [ i ].train_num > 0 )
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

	printf ( "现在开始输入站台参数，是否从文件读取？Y/N:\n" );
	char c_station;
	char c;

	scanf ( "%c" , &c_station );
	getchar ( );

	/***********初始化站台***********/
	while ( 1 )
	{
		if ( c_station == 'Y' )//从文件读取
		{
			FILE* file_station;
			if ( ( file_station = fopen ( "station.txt" , "r" ) ) && file_station != NULL )
			{
				while ( 1 )
				{
					int sx , sy;
					fscanf ( file_station , "%d%d" , &sx , &sy );
					if ( ( sx == -1 && sy == -1 ) )
					{
						break;
					}
					else if ( railway [ sx ] [ sy ].train_num == 0 )
					{
						continue;
					}
					else
					{
						railway [ sx ] [ sy ].is_station = 1;
						while ( 1 )
						{
							int t_train , t_time;
							fscanf ( file_station , "%d%d" , &t_train , &t_time );
							if ( t_train > train_amount )
							{
								continue;
							}
							else if ( t_train == 0 )
							{
								break;
							}
							else
							{
								railway [ sx ] [ sy ].station_time [ t_train ] = t_time;
							}
						}
					}
				}
				break;
			}
			else//文件读取失败
			{
				printf ( "读取站台参数失败！改为手动输入！\n" );
				c_station = 'N';
			}
		}
		else if ( c_station == 'N' )//不从文件读取
		{
			vector<pair<int , int>>vec_station;//用于保存
			/*读取用户输入站台信息*/
			while ( 1 )
			{
				printf ( "请输入站台坐标x y，结束请输入-1 -1！\n" );
				int sx , sy;
				scanf ( "%d%d" , &sx , &sy );
				if ( ( sx == -1 && sy == -1 ) )
				{
					vec_station.push_back ( make_pair ( sx , sy ) );
					break;
				}
				else if ( railway [ sx ] [ sy ].train_num == 0 )
				{
					printf ( "输入错误！输入坐标处没有轨道！重新输入！\n" );
				}
				else
				{
					railway [ sx ] [ sy ].is_station = 1;
					vec_station.push_back ( make_pair ( sx , sy ) );
					printf ( "请输入需要停靠的火车和停靠时间，以0 0结束：" );
					while ( 1 )
					{
						int t_train , t_time;
						scanf ( "%d%d" , &t_train , &t_time );
						if ( t_train > train_amount )
						{
							printf ( "火车不存在！重新输入！\n" );
						}
						else if ( t_train == 0 )
						{
							vec_station.push_back ( make_pair ( t_train , t_time ) );
							break;
						}
						else
						{
							vec_station.push_back ( make_pair ( t_train , t_time ) );
							railway [ sx ] [ sy ].station_time [ t_train ] = t_time;
						}
					}
				}
			}
			/*保存*/
			printf ( "是否想保存站台参数？Y/N" );
			char station_save;
			while ( ( c = getchar ( ) ) != '\n' && c != EOF ) ;
			scanf ( "%c" , &station_save );
			getchar ( );
			if ( station_save == 'Y' )
			{
				FILE* station_save_file;
				station_save_file = fopen ( "station.txt" , "w" );
				if ( station_save_file == NULL )
				{
					printf ( "保存站台参数失败！\n" );
				}
				else
				{
					int sum = vec_station.size ( );
					for ( int i = 0; i < sum; i++ )
					{
						fprintf ( station_save_file , "%d %d\n" , vec_station [ i ].first , vec_station [ i ].second );
					}
					printf ( "保存站台参数成功！\n" );
				}
				fclose ( station_save_file );
			}
			/*结束保存*/
			break;
		}
		else
		{
			printf ( "输入错误！请重新输入！\n" );
			scanf ( "%c" , &c_station );
			getchar ( );
		}
	}
}