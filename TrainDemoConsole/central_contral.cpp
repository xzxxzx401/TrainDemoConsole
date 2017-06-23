/*中央控制模块 包含策略函数，搜索函数*/
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <queue>
#include <functional>
#include "data_structure.h"

using namespace std;
/*****************定义变量******************/

vector <int > pub_speed [ MAXNUM_QUEUE ] ;//火车入公轨队列
vector <int> pub_byturn [ MAXNUM_QUEUE ];//火车入共轨队列
/*****************结束定义变量**************/

/*****************声明变量******************/

extern vector<Train> train;
extern Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];
extern int train_amount;//火车数量
extern bool pub_state [ MAXNUM_QUEUE ];//公轨状态数组 1表示占用 0表示空闲
extern int stage;//策略 1表示速度优先 2表示交替
extern bool mp [ MAXNUM_TRAIN_NUM + 1 ] [ MAXNUM_TRAIN_NUM + 1 ];//交替策略 mp[i][j]==0 表示i优于j  ==1表示j优于i
extern int maxnwidth;//轨道实际使用的最大宽度 输出用
extern int maxnhight;//轨道实际使用的最大高度 输出用
extern int maxnqueue;//实际使用的队列数 减少循环用

/*****************结束声明变量**************/

/*****************声明函数******************/
static void dfs ( int pub_id , int x , int y );
/*****************结束声明函数**************/

/************中央控制系统函数***************/

/*速度优先策略*/
void central_control_speed ( void )
{
	/*先判断每个火车 如果nextpos是共轨 那么把它停下来 放到队列里
	在查看队列 如果队列非空 那取出顶端火车*/
	//共轨内部火车排序
	for ( int i = 1; i <= maxnqueue; i++ )
	{
		int size = pub_speed [ i ].size ( );
		if ( size > 1 )
		{
			for ( int j = 0; j < size - 1; j++ )
			{
				for ( int k = j + 1; k < size; k++ )
				{
					if ( train [ pub_speed [ i ] [ j ] ].speed_init < train [ pub_speed [ i ] [ k ] ].speed_init )
					{
						int temp;
						temp = pub_speed [ i ] [ j ];
						pub_speed [ i ] [ j ] = pub_speed [ i ] [ k ];
						pub_speed [ i ] [ k ] = temp;
					}
				}
			}
		}
	}
	//发车
	for ( int i = 1; i <= maxnqueue; i++ )
	{
		int size = pub_speed [ i ].size ( );
		if ( size > 0 )
		{
			int id = pub_speed [ i ] [ 0 ];
			int x = train [ id ].next_pos.x , y = train [ id ].next_pos.y;

			if ( pub_state [ i ] == 0 )
			{
				if ( train [ id ].state == 0 )//如果火车没有被用户暂停
				{
					train [ id ].speed = train [ id ].speed_init;
					pub_state [ i ] = 1;
					train [ id ].pub_id = railway [ x ] [ y ].public_id;
				}
				
				vector<int>::iterator ite;
				ite = pub_speed[ i ].begin ( );
				pub_speed [ i ] .erase ( ite );
			}
		}
	}
}

/*交替策略*/
void central_control_byturn ( void )
{
	//共轨内部火车排序
	for ( int i = 1; i <= maxnqueue; i++ )
	{
		int size = pub_byturn [ i ].size ( );
		if ( size > 1 )
		{
			for ( int j = 0; j < size - 1; j++ )
			{
				for ( int k = j + 1; k < size; k++ )
				{
					if ( mp [ pub_byturn [ i ] [ j ] ] [ pub_byturn [ i ] [ k ] ] == 0 )
					{
						int temp;
						temp = pub_byturn [ i ] [ j ];
						pub_byturn [ i ] [ j ] = pub_byturn [ i ] [ k ];
						pub_byturn [ i ] [ k ] = temp;
					}
				}
			}
		}
	}
	//发车
	for ( int i = 1; i <= maxnqueue; i++ )
	{
		int size = pub_byturn [ i ].size ( );
		if ( size > 0 )
		{
			int id = pub_byturn [ i ] [ 0 ];
			int x = train [ id ].next_pos.x , y = train [ id ].next_pos.y;

			if ( pub_state [ i ] == 0 )
			{
				if ( train [ id ].state == 0 )//如果火车没有被用户暂停
				{
					train [ id ].speed = train [ id ].speed_init;
					pub_state [ i ] = 1;
					train [ id ].pub_id = railway [ x ] [ y ].public_id;
				}
				if ( size > 1 )
				{
					for ( int j = 1; j < size; j++ )
					{
						mp [ pub_byturn [ i ] [ 0 ] ] [ pub_byturn [ i ] [ j ] ] = ( mp [ pub_byturn [ i ] [ 0 ] ] [ pub_byturn [ i ] [ j ] ] + 1 ) % 2;
						mp [ pub_byturn [ i ] [ j ] ] [ pub_byturn [ i ] [ 0 ] ] = ( mp [ pub_byturn [ i ] [ j ] ] [ pub_byturn [ i ] [ 0 ] ] + 1 ) % 2;
					}
				}
				vector<int>::iterator ite;
				ite = pub_byturn [ i ].begin ( );
				pub_byturn [ i ] .erase ( ite );
			}
		}
	}
}

/*搜索*/
void check_pub ( void )//dfs检查有几段公共轨道 以便定义队列
{
	static int res = 1;
	for ( int i = 0; i <= MAXNUM_RAILWAY; i++ )
	{
		for ( int j = 0; j <= MAXNUM_RAILWAY; j++ )
		{
			if ( railway [ i ] [ j ].train_num == -1 && railway [ i ] [ j ].public_id == 0 )
			{
				dfs ( res , i , j );
				res++;
			}
		}
	}
	maxnqueue = res;
	for ( int i = 0; i <= MAXNUM_RAILWAY; i++ )
	{
		for ( int j = 0; j <= MAXNUM_RAILWAY; j++ )
		{
			if ( railway [ i ] [ j ].train_num != 0 )
			{
				if ( i > maxnwidth ) maxnwidth = i;
				if ( j > maxnhight ) maxnhight = j;
			}
		}
	}
}

/*搜索函数 检车公共轨道并标号*/
static void dfs ( int pub_id , int x , int y )
{
	railway [ x ] [ y ].public_id = pub_id;
	for ( int i = 1; i <= train_amount; i++ )
	{
		/*顺时针方向搜索*/
		int dx = 0 , dy = 0;
		if ( railway [ x ] [ y ].direction [ 0 ] [ i ] == 1 )
		{
			dx = 1;
		}
		else if ( railway [ x ] [ y ].direction [ 0 ] [ i ] == 2 )
		{
			dy = 1;
		}
		else if ( railway [ x ] [ y ].direction [ 0 ] [ i ] == 3 )
		{
			dx = ( -1 );
		}
		else if ( railway [ x ] [ y ].direction [ 0 ] [ i ] == 4 )
		{
			dy = (-1);
		}
		int nx = x + dx , ny = y + dy;
		if ( 0 < nx && nx <= MAXNUM_RAILWAY && 0 < ny && ny <= MAXNUM_RAILWAY && railway [ nx ] [ ny ].public_id == 0 && railway [ nx ] [ ny ].train_num == -1 )
		{
			dfs ( pub_id , nx , ny );
		}
		/*逆时针方向搜索*/
		if ( railway [ x ] [ y ].direction [ 1 ] [ i ] == 1 )
		{
			dx = 1;
		}
		else if ( railway [ x ] [ y ].direction [ 1 ] [ i ] == 2 )
		{
			dy = 1 ;
		}
		else if ( railway [ x ] [ y ].direction [ 1 ] [ i ] == 3 )
		{
			dx = ( -1 );
		}
		else if ( railway [ x ] [ y ].direction [ 1 ] [ i ] == 4 )
		{
			dy = ( -1 );
		}
		nx = x + dx , ny = y + dy;
		if ( 0 < nx && nx <= MAXNUM_RAILWAY && 0 < ny && ny <= MAXNUM_RAILWAY && railway [ nx ] [ ny ].public_id == 0 && railway [ nx ] [ ny ].train_num == -1 )
		{
			dfs ( pub_id , nx , ny );
		}
	}
}
