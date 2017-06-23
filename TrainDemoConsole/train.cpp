/*火车模块 包含火车轨道的初始化与运行*/
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

/*****************声明全局变量****************/
extern vector<Train> train;
extern Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];
extern bool pub_state [ MAXNUM_QUEUE ];//公轨状态数组 1表示占用 0表示空闲
extern vector <int > pub_speed [ MAXNUM_QUEUE ];
extern vector <int > pub_byturn [ MAXNUM_QUEUE ];
extern int stage;//策略 速度优先为1 交替策略为2
 /*****************结束声明****************/

/****************声明函数*****************/
//void train_run_next ( int train_num );
void check_state ( int train_num );
/**************结束声明*******************/

/********读取火车及轨道信息函数**********/
static void train_read ( int train_num )
{
	printf ( "是否从文件读取火车%d的默认配置？Y/N:" , train_num );
	char y_n;
	scanf ( "%c" , &y_n );
	getchar ( );
	while ( 1 )
	{
		if ( y_n == 'Y' )//从文件读入
		{
			/******************连接字符串 获取文件名*********/
			string str1 , str2;
			str1 = "train" ;
			stringstream stream;
			stream << train_num;
			stream >> str2;
			str1.append ( str2 );
			str1.append ( ".txt" );
			const char *t = str1.data ( );
			/********************结束连接******************/

			FILE* f1;
			if ( ( f1 = fopen ( t , "r" ) ) && f1 != NULL )
			{
				fscanf ( f1 , "%d" , &( train [ train_num ].start_time ) );
				fscanf ( f1 , "%d" , &( train [ train_num ].speed_init ) );
				train [ train_num ].speed = train [ train_num ].speed_init;
				fscanf ( f1 , "%d%d" , &( train [ train_num ].pos.x ) , &( train [ train_num ].pos.y ) );
				train [ train_num ].next_pos.x = train [ train_num ].pos.x;
				train [ train_num ].next_pos.y = train [ train_num ].pos.y;
				fscanf ( f1 , "%d" , &( train [ train_num ].direction ) );
				
				/*****************初始化轨道*********************/

				int ax , ay , bx , by , x0 , y0;
				fscanf ( f1 , "%d%d" , &x0 , &y0 );
				ax = x0 , ay = y0;
				bool flag = 1;
				int i , j;
				//将火车方向写入轨道数组
				while ( flag )
				{
					fscanf ( f1 , "%d%d" , &bx , &by );

					if ( bx == -1 && by == -1 )
					{
						bx = x0 , by = y0;
						flag = 0;
					}
					
					if ( bx == ax )
					{
						/*顺时针方向*/

						//判断火车运行方向 定义i以处理轨道拐点问题
						if ( ay > by )
						{
							i = by + 1 , j = ay;
						}
						else if ( ay < by )
						{
							i = ay , j = by - 1;
						}
						for ( ; i <= j; i++ )
						{
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
									if ( railway [ ax ] [ i ].direction [ 0 ] [ kk ] > 0 )
									{
										railway [ ax ] [ i ].is_public [ kk ] = 1;
									}
								}
							}
							if ( ay > by )
							{
								railway [ ax ] [ i ].direction [ 0 ] [ train_num ] = 4;
							}
							else if ( ay < by )
							{
								railway [ ax ] [ i ].direction [ 0 ] [ train_num ] = 2;
							}
						}


						/*逆时针方向*/

						if ( ay > by )
						{
							i = by , j = ay - 1;
						}
						else if ( ay < by )
						{
							i = ay + 1 , j = by;
						}
						for ( ; i <= j; i++ )
						{
							if ( ay > by )
							{
								railway [ ax ] [ i ].direction [ 1 ] [ train_num ] = 2;
							}
							else if ( ay < by )
							{
								railway [ ax ] [ i ].direction [ 1 ] [ train_num ] = 4;
							}
						}

						ax = bx , ay = by;
					}

					else if ( by == ay )
					{
						//判断火车运行方向 定义i以处理轨道拐点问题
						/*顺时针方向*/
						if ( ax > bx )
						{
							i = bx + 1 , j = ax;
						}
						else if ( ax < bx )
						{
							i = ax , j = bx - 1;
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
									if ( railway [ i ] [ ay ].direction [ 0 ] [ kk ] > 0 )
									{
										railway [ i ] [ ay ].is_public [ kk ] = 1;
									}
								}
							}

							if ( ax > bx )
							{
								railway [ i ] [ ay ].direction [ 0 ] [ train_num ] = 3;
							}
							else if ( ax < bx )
							{
								railway [ i ] [ ay ].direction [ 0 ] [ train_num ] = 1;
							}
						}

						/*逆时针方向*/
						if ( ax > bx )
						{
							i = bx , j = ax - 1;
						}
						else if ( ax < bx )
						{
							i = ax + 1 , j = bx;
						}
						for ( ; i <= j; i++ )
						{
							if ( ax > bx )
							{
								railway [ i ] [ ay ].direction [ 1 ] [ train_num ] = 1;
							}
							else if ( ax < bx ) 
							{
								railway [ i ] [ ay ].direction [ 1 ] [ train_num ] = 3;
							}
						}

						ax = bx , ay = by;
					}
					else
					{
						printf ( "输入错误！请重新输入！\n" );
					}
				}
				/*****************结束初始化轨道****************/

				printf ( "读取火车%d参数成功！\n" , train_num );
				fclose ( f1 );
				break;
			}
			else//读取文件失败 将y_n置为N 下一次循环直接进入不从文件读入的区域
			{
				printf ( "读取火车%d参数失败！将改为键盘输入！\n" , train_num );
				y_n = 'N';
			}
		}
		else if ( y_n == 'N' )//不从文件读入
		{
			vector< pair<int , int> > vec2;//用于记录轨道信息
			printf ( "请输入火车%d启动时间：" , train_num );
			scanf ( "%d" , &( train [ train_num ].start_time ) );
			printf ( "请输入火车%d速度：(不要太大)" , train_num );
			scanf ( "%d" , &( train [ train_num ].speed_init ) );
			train [ train_num ].speed = train [ train_num ].speed_init;
			printf ( "请输入火车%d初始位置x，y(不大于%d)：" , train_num , MAXNUM_RAILWAY );
			while ( 1 )
			{
				scanf ( "%d%d" , &( train [ train_num ].pos.x ) , &( train [ train_num ].pos.y ) );
				if ( train [ train_num ].pos.x > MAXNUM_RAILWAY || train [ train_num ].pos.y > MAXNUM_RAILWAY || train [ train_num ].pos.x < 1 || train [ train_num ].pos.y < 1 )
				{
					printf ( "输入坐标超出范围！请重新输入！\n" );
				}
				else
				{
					break;
				}
			}
			train [ train_num ].next_pos.x = train [ train_num ].pos.x;
			train [ train_num ].next_pos.y = train [ train_num ].pos.y;
			printf ( "请输入火车%d运行方向 0顺 1逆：" , train_num );
			while ( 1 )
			{
				scanf ( "%d" , &( train [ train_num ].direction ) );
				if ( train [ train_num ].direction != 0 && train [ train_num ].direction != 1 )
				{
					printf ( "输入错误！重新输入！\n" );
				}
				else
				{
					break;
				}
			}
			

			/*初始化轨道*/
			printf ( "请按顺时针输入火车%d轨道顶点坐标x y(不大于%d)，以-1 -1结束\n" , train_num , MAXNUM_RAILWAY );
			int ax , ay , bx , by , x0 , y0;
			while ( 1 )
			{
				scanf ( "%d%d" , &x0 , &y0 );
				if ( x0 > MAXNUM_RAILWAY || x0 < -1 || y0<-1 || y0>MAXNUM_RAILWAY )
				{
					printf ( "输入错误！重新输入！\n" );
				}
				else
				{
					break;
				}
			}

			vec2.push_back ( make_pair ( x0 , y0 ) );

			getchar ( );
			ax = x0 , ay = y0;
			bool flag = 1;
			int i , j;

			while ( flag )
			{
				while ( 1 )
				{
					scanf ( "%d%d" , &bx , &by );
					if ( bx > MAXNUM_RAILWAY || bx < -1 || by<-1 || by>MAXNUM_RAILWAY )
					{
						printf ( "输入错误！重新输入！\n" );
					}
					else
					{
						break;
					}
				}
				vec2.push_back ( make_pair ( bx , by ) );
				getchar ( );
				if ( bx == -1 && by == -1 )
				{
					bx = x0 , by = y0;
					flag = 0;
				}
				if ( ( bx == -1 || by == -1 ) && flag == 1 )
				{
					printf ( "输入错误！\n" );
				}
				if ( bx == ax )
				{
					/*顺时针方向*/

					//判断火车运行方向 定义i以处理轨道拐点问题
					if ( ay > by )
					{
						i = by + 1 , j = ay;
					}
					else if ( ay < by )
					{
						i = ay , j = by - 1;
					}
					for ( ; i <= j; i++ )
					{
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
								if ( railway [ ax ] [ i ].direction [ 0 ] [ kk ] > 0 )
								{
									railway [ ax ] [ i ].is_public [ kk ] = 1;
								}
							}
						}
						if ( ay > by )
						{
							railway [ ax ] [ i ].direction [ 0 ] [ train_num ] = 4;
						}
						else if ( ay < by )
						{
							railway [ ax ] [ i ].direction [ 0 ] [ train_num ] = 2;
						}
					}


					/*逆时针方向*/

					if ( ay > by )
					{
						i = by , j = ay - 1;
					}
					else if ( ay < by )
					{
						i = ay + 1 , j = by;
					}
					for ( ; i <= j; i++ )
					{
						if ( ay > by )
						{
							railway [ ax ] [ i ].direction [ 1 ] [ train_num ] = 2;
						}
						else if ( ay < by )
						{
							railway [ ax ] [ i ].direction [ 1 ] [ train_num ] = 4;
						}
					}

					ax = bx , ay = by;
				}

				else if ( by == ay )
				{
					//判断火车运行方向 定义i以处理轨道拐点问题
					/*顺时针方向*/
					if ( ax > bx )
					{
						i = bx + 1 , j = ax;
					}
					else if ( ax < bx )
					{
						i = ax , j = bx - 1;
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
								if ( railway [ i ] [ ay ].direction [ 0 ] [ kk ] > 0 )
								{
									railway [ i ] [ ay ].is_public [ kk ] = 1;
								}
							}
						}

						if ( ax > bx )
						{
							railway [ i ] [ ay ].direction [ 0 ] [ train_num ] = 3;
						}
						else if ( ax < bx )
						{
							railway [ i ] [ ay ].direction [ 0 ] [ train_num ] = 1;
						}
					}

					/*逆时针方向*/
					if ( ax > bx )
					{
						i = bx , j = ax - 1;
					}
					else if ( ax < bx )
					{
						i = ax + 1 , j = bx;
					}
					for ( ; i <= j; i++ )
					{
						if ( ax > bx )
						{
							railway [ i ] [ ay ].direction [ 1 ] [ train_num ] = 1;
						}
						else if ( ax < bx )
						{
							railway [ i ] [ ay ].direction [ 1 ] [ train_num ] = 3;
						}
					}

					ax = bx , ay = by;
				}
				else
				{
					printf ( "输入错误！请重新输入！\n" );
				}
			}
			/*结束初始化轨道*/

			printf ( "是否想保存火车%d默认参数？Y/N:" , train_num );
			char y_n2 ;
			scanf ( "%c" , &y_n2 );
			getchar ( );
			if ( y_n2 == 'Y' )
			{
				/******************连接字符串 获取文件名*********/
				string str1 , str2;
				str1 = "train" ;
				stringstream stream;
				stream << train_num;
				stream >> str2;
				str1.append ( str2 );
				str1.append ( ".txt" );
				const char *t = str1.data ( );
				/********************结束连接******************/

				/********************保存参数******************/
				FILE* f1;
				f1 = fopen ( t , "w" );
				if ( f1 == NULL )
				{
					printf ( "保存火车%d的参数失败！\n" , train_num );
				}
				else
				{
					fprintf ( f1 , "%d\n" , train [ train_num ].start_time );
					fprintf ( f1 , "%d\n" , train [ train_num ].speed_init );
					fprintf ( f1 , "%d %d\n" , train [ train_num ].pos.x , train [ train_num ].pos.y );
					fprintf ( f1 , "%d\n" , train [ train_num ].direction );
					for ( int i = 0; i < vec2.size ( ); i++ )
					{
						fprintf ( f1 , "%d %d\n" , vec2 [ i ].first , vec2 [ i ].second );
					}
					printf ( "保存火车%d的参数成功！\n" , train_num );
				}
				fclose ( f1 );
			}
			break;
		}
		else//异常输入处理
		{
			printf ( "输入错误！请重新输入！\n" );
			scanf ( "%c" , &y_n );
			getchar ( );
		}
	}
	
}

/**********运行火车函数**********/
static void train_run ( int train_num )//运行火车
{
	int speed;
	bool direction;
	direction = train [ train_num ] .direction;
	speed = train [ train_num ] .speed;
	

	int *x = &( train [ train_num ].pos.x ) , *y = &( train [ train_num ].pos.y );
	int *xn = &( train [ train_num ].next_pos.x ) , *yn = &( train [ train_num ].next_pos.y );

	//如果等待站台时间和应等待时间一致 那么释放火车
	if ( train [ train_num ].waiting_time == railway [ *x ] [ *y ].station_time [ train_num ] && train [ train_num ].waiting_time > 0 )
	{
		train [ train_num ].state_station = 0;
		train [ train_num ].waiting_time = 0;
		train [ train_num ].speed = train [ train_num ].speed_init;

		if ( railway [ *xn ] [ *yn ].train_num == -1 && train [ train_num ].pub_id <= 0 )
		{
			if ( stage == 1 )
			{
				int pub_id = railway [ *xn ] [ *yn ].public_id;
				if ( train [ train_num ].state == 0 && train [ train_num ].pub_id != pub_id )
				{
					if ( find ( pub_speed [ pub_id ].begin ( ) , pub_speed [ pub_id ].end ( ) , train_num ) == pub_speed [ pub_id ].end ( ) )
					{
						pub_speed [ pub_id ].push_back ( train_num );
						train [ train_num ].speed = 0;
						train [ train_num ].pub_id = -1;
					}
				}
			}
			else if ( stage == 2 )
			{
				int pub_id = railway [ *xn ] [ *yn ].public_id;
				if ( train [ train_num ].state == 0 && train [ train_num ].pub_id != pub_id )
				{
					if ( find ( pub_byturn [ pub_id ].begin ( ) , pub_byturn [ pub_id ].end ( ) , train_num ) == pub_byturn [ pub_id ].end ( ) )
					{
						pub_byturn [ pub_id ].push_back ( train_num );
						train [ train_num ].speed = 0;
						train [ train_num ].pub_id = -1;
					}
				}
			}
		}
		check_state ( train_num );
	}
	//如果处于站台等待时间 那么等待时间++
	if ( train [ train_num ].state_station == 2 )
	{
		train [ train_num ].waiting_time++;
		return;
	}
	if ( ( *x ) == ( *xn ) && ( *y ) == ( *yn ) )
	{
		if ( direction == 0 )
		{
			switch ( railway [ *xn ] [ *yn ].direction [ 0 ] [ train_num ] )
			{
			case 1: {( *xn ) += 1; break; }
			case 2: {( *yn ) += 1; break; }
			case 3: {( *xn ) += -1; break; }
			case 4: {( *yn ) += -1; break; }
			}
		}
		else
		{
			switch ( railway [ *xn ] [ *yn ].direction [ 1 ] [ train_num ] )
			{
			case 1: {( *xn ) += 1; break; }
			case 2: {( *yn ) += 1; break; }
			case 3: {( *xn ) += -1; break; }
			case 4: {( *yn ) += -1; break; }
			}
		}
	}
	if ( direction == 0 )
	{
		for ( int i = 1; i <= speed; i++ )
		{
			//向右是1 向下2 向左3 向上4
			switch ( railway [ *x ] [ *y ].direction [ 0 ] [ train_num ] )
			{
			case 1: {( *x ) += 1; break; }
			case 2: {( *y ) += 1; break; }
			case 3: {( *x ) += -1; break; }
			case 4: {( *y ) += -1; break; }
			}
			switch ( railway [ *xn ] [ *yn ].direction [ 0 ] [ train_num ] )
			{
			case 1: {( *xn ) += 1; break; }
			case 2: {( *yn ) += 1; break; }
			case 3: {( *xn ) += -1; break; }
			case 4: {( *yn ) += -1; break; }
			}
			//如果到了站台 那么立刻听下火车 更新next位置
			if ( railway [ *x ] [ *y ].station_time [ train_num ] > 0 )
			{
				train [ train_num ].state_station = 2;
				train [ train_num ].next_pos.x = train [ train_num ].pos.x;
				train [ train_num ].next_pos.y = train [ train_num ].pos.y;
				break;
			}
			//到公共轨道 停车
			if ( railway [ *xn ] [ *yn ].train_num == -1 && train [ train_num ].pub_id <= 0 )
			{
				if ( stage == 1 )
				{
					int pub_id = railway [ *xn ] [ *yn ].public_id;
					if ( train [ train_num ].state == 0 && train [ train_num ].pub_id != pub_id )
					{
						if ( find ( pub_speed [ pub_id ].begin ( ) , pub_speed [ pub_id ].end ( ) , train_num ) == pub_speed [ pub_id ].end ( ) )
						{
							pub_speed [ pub_id ].push_back ( train_num );
							train [ train_num ].speed = 0;
							train [ train_num ].pub_id = -1;
						}
					}
				}
				else if ( stage == 2 )
				{
					int pub_id = railway [ *xn ] [ *yn ].public_id;
					if ( train [ train_num ].state == 0 && train [ train_num ].pub_id != pub_id )
					{
						if ( find ( pub_byturn [ pub_id ].begin ( ) , pub_byturn [ pub_id ].end ( ) , train_num ) == pub_byturn [ pub_id ].end ( ) )
						{
							pub_byturn [ pub_id ].push_back ( train_num );
							train [ train_num ].speed = 0;
							train [ train_num ].pub_id = -1;
						}
					}
				}
				break;
			}
			check_state ( train_num );
		}
	}
	else if ( direction == 1 )
	{
		for ( int i = 1; i <= speed; i++ )
		{
			//向右是1 向下2 向左3 向上4
			switch ( railway [ *x ] [ *y ].direction [ 1 ] [ train_num ] )
			{
			case 1: {( *x ) += 1; break; }
			case 2: {( *y ) += 1; break; }
			case 3: {( *x ) += -1; break; }
			case 4: {( *y ) += -1; break; }
			}
			switch ( railway [ *xn ] [ *yn ].direction [ 1 ] [ train_num ] )
			{
			case 1: {( *xn ) += 1; break; }
			case 2: {( *yn ) += 1; break; }
			case 3: {( *xn ) += -1; break; }
			case 4: {( *yn ) += -1; break; }
			}
			//如果到了站台那么听下火车 更新next位置
			if ( railway [ *x ] [ *y ].station_time [ train_num ] > 0 )
			{
				train [ train_num ].state_station = 2;
				train [ train_num ].next_pos.x = train [ train_num ].pos.x;
				train [ train_num ].next_pos.y = train [ train_num ].pos.y;
				break;
			}
			//到公共轨道 停车
			if ( railway [ *xn ] [ *yn ].train_num == -1 && train [ train_num ].pub_id <= 0 )
			{
				if ( stage == 1 )
				{
					int pub_id = railway [ *xn ] [ *yn ].public_id;
					if ( train [ train_num ].state == 0 && train [ train_num ].pub_id != pub_id )
					{
						if ( find ( pub_speed [ pub_id ].begin ( ) , pub_speed [ pub_id ].end ( ) , train_num ) == pub_speed [ pub_id ].end ( ) )
						{
							pub_speed [ pub_id ].push_back ( train_num );
							train [ train_num ].speed = 0;
							train [ train_num ].pub_id = -1;
						}
					}
				}
				else if ( stage == 2 )
				{
					int pub_id = railway [ *xn ] [ *yn ].public_id;
					if ( train [ train_num ].state == 0 && train [ train_num ].pub_id != pub_id )
					{
						if ( find ( pub_byturn [ pub_id ].begin ( ) , pub_byturn [ pub_id ].end ( ) , train_num ) == pub_byturn [ pub_id ].end ( ) )
						{
							pub_byturn [ pub_id ].push_back ( train_num );
							train [ train_num ].speed = 0;
							train [ train_num ].pub_id = -1;
						}
					}
				}
				break;
			}
			check_state ( train_num );
		}
	}
}

/***********预处理下一次位置*********/
/*void train_run_next ( int train_num )//算出火车下一次的位置
{
	int speed;
	bool direction;
	direction = train [ train_num ] .direction;
	speed = train [ train_num ] .speed;
	if ( direction == 0 )
	{
		for ( int i = 1; i <= speed; i++ )
		{
			int *x = &( train [ train_num ].next_pos.x ) , *y = &( train [ train_num ].next_pos.y );
			//向右是1 向下2 向左3 向上4
			switch ( railway [ *x ] [ *y ].direction [ 0 ] [ train_num ] )
			{
			case 1: {( *x ) += 1; break; }
			case 2: {( *y ) += 1; break; }
			case 3: {( *x ) += ( -1 ); break; }
			case 4: {( *y ) += ( -1 ); break; }
			}
		}
	}
	else if ( direction == 1 )
	{
		for ( int i = 1; i <= speed; i++ )
		{
			int *x = &( train [ train_num ].next_pos.x ) , *y = &( train [ train_num ].next_pos.y );
			//向右是1 向下2 向左3 向上4
			switch ( railway [ *x ] [ *y ].direction [ 1 ] [ train_num ] )
			{
			case 1: {( *x ) += 1; break; }
			case 2: {( *y ) += 1; break; }
			case 3: {( *x ) += ( -1 ); break; }
			case 4: {( *y ) += ( -1 ); break; }
			}
		}
	}
}
*/

/*******检查火车状态并释放公轨******/
static void check_state ( int train_num )//检查火车状态 释放公共轨道
{
	//0表示在非公轨行驶 1表示进入公轨
	int x = train [ train_num ].pos.x , y = train [ train_num ].pos.y ;
	int pre_pubid = train [ train_num ].checking_pub_id;
	int now_pubid = railway [ x ] [ y ].public_id;

	if ( pre_pubid != now_pubid )
	{
		if ( pre_pubid != 0 )
		{
			pub_state [ pre_pubid ] = 0;
			//公共轨道状态标记数组释放
		}
		train [ train_num ].pub_id = now_pubid;
		train [ train_num ].checking_pub_id = now_pubid;
		//火车的pub_id置0
	}
}

/************初始化火车*************/
void train_init ( int train_num )//初始化火车
{
	train_read ( train_num );
}

/*运行火车，预处理峡下一次位置，检查状态*/
void train_running ( int train_num )//运行火车
{
	if ( train [ train_num ].start_time > 0 )//如果处于等待运行状态那么不运行火车
	{
		train [ train_num ].start_time--;
		return;
	}
	train_run ( train_num );
}
