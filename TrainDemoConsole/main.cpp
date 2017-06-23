#define  _CRT_SECURE_NO_WARNINGS
# include <stdio.h>
# include<stdlib.h>
#include<vector>
#include <winsock.h>
#include <WinBase.h>
#include"head.h"
#include<cstring>
#include<conio.h>
using namespace std;
/****************定义全局变量**********************/

vector<Train> train;//火车实例化数组
Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];//轨道实例化数组
bool pub_state [ MAXNUM_QUEUE + 1 ] = { 0 };//公轨状态数组 1表示占用 0表示空闲
int train_amount = 0;//实际火车数量
FILE* file_output = NULL;//标准输出文件
bool have_input = 0;//线程间通信用
int stage = 1;//策略 速度优先为1 交替策略为2
bool mp [ MAXNUM_TRAIN_NUM + 1 ] [ MAXNUM_TRAIN_NUM + 1 ];//交替策略 mp[i][j]==0 表示i优于j  ==1表示j优于i
int maxnwidth = 0;//轨道实际使用的最大宽度 输出用
int maxnhight = 0;//轨道实际使用的最大高度 输出用
int maxnqueue = 0;//实际使用的队列数 减少循环用
/***************结束定义全局变量******************/

int main ( void )
{
	memset ( railway , 0 , sizeof ( railway ) );
	memset ( mp , 0 , sizeof ( mp ) );

	/**************打开标准输出文件***************/
	file_output = fopen ( "file_output.txt" , "w" );
	/**************结束打开标准输出文件***************/

	/************输入小火车的个数*******************/
	printf ( "请输入小火车的个数：" );
	scanf ( "%d" , &train_amount );

	getchar ( );
	/************结束输入小火车的个数*************/
	
	/****************初始化火车*******************/
	for ( int i = 1; i <= train_amount; i++ )
	{
		Train temp;
		if ( i == 1 ) train.push_back ( temp );
		train.push_back ( temp );
		train_init ( i );
	}
	/**************结束初始化火车*****************/

	/**************初始化mp数组******************/
	for ( int i = 0; i <= MAXNUM_TRAIN_NUM; i++ )
	{
		for ( int j = 0; j <= MAXNUM_TRAIN_NUM; j++ )
		{
			if ( i >= j )
			{
				mp [ i ] [ j ] = 0;
			}
			else
			{
				mp [ i ] [ j ] = 1;
			}
		}
	}
	/************结束初始化mp数组****************/
	
	check_pub ( );//检查公共轨道
	
	station_init ( );//输入站台信息

	while ( 1 )
	{
		output ( );
		//检测到输入则进入循环 等待input函数修改完参数 (线程同步)
		if ( _kbhit ( ) )
		{
			input ( );
		}

		Sleep ( time_space );
		if ( stage == 1 )
		{
			central_control_speed ( );//中央控制速度优先
		}
		else
		{
			central_control_byturn ( );//交替策略
		}
		for ( int i = 1; i <= train_amount; i++ )//运行火车
		{
			train_running ( i );
		}
	}
}