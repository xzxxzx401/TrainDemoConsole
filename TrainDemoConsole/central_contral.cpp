/*�������ģ�� �������Ժ�������������*/
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <queue>
#include <functional>
#include "data_structure.h"

using namespace std;
/*****************�������******************/

vector <int > pub_speed [ MAXNUM_QUEUE ] ;//���빫�����
vector <int> pub_byturn [ MAXNUM_QUEUE ];//���빲�����
/*****************�����������**************/

/*****************��������******************/

extern vector<Train> train;
extern Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];
extern int train_amount;//������
extern bool pub_state [ MAXNUM_QUEUE ];//����״̬���� 1��ʾռ�� 0��ʾ����
extern int stage;//���� 1��ʾ�ٶ����� 2��ʾ����
extern bool mp [ MAXNUM_TRAIN_NUM + 1 ] [ MAXNUM_TRAIN_NUM + 1 ];//������� mp[i][j]==0 ��ʾi����j  ==1��ʾj����i
extern int maxnwidth;//���ʵ��ʹ�õ������ �����
extern int maxnhight;//���ʵ��ʹ�õ����߶� �����
extern int maxnqueue;//ʵ��ʹ�õĶ����� ����ѭ����

/*****************������������**************/

/*****************��������******************/
static void dfs ( int pub_id , int x , int y );
/*****************������������**************/

/************�������ϵͳ����***************/

/*�ٶ����Ȳ���*/
void central_control_speed ( void )
{
	/*���ж�ÿ���� ���nextpos�ǹ��� ��ô����ͣ���� �ŵ�������
	�ڲ鿴���� ������зǿ� ��ȡ�����˻�*/
	//�����ڲ�������
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
	//����
	for ( int i = 1; i <= maxnqueue; i++ )
	{
		int size = pub_speed [ i ].size ( );
		if ( size > 0 )
		{
			int id = pub_speed [ i ] [ 0 ];
			int x = train [ id ].next_pos.x , y = train [ id ].next_pos.y;

			if ( pub_state [ i ] == 0 )
			{
				if ( train [ id ].state == 0 )//�����û�б��û���ͣ
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

/*�������*/
void central_control_byturn ( void )
{
	//�����ڲ�������
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
	//����
	for ( int i = 1; i <= maxnqueue; i++ )
	{
		int size = pub_byturn [ i ].size ( );
		if ( size > 0 )
		{
			int id = pub_byturn [ i ] [ 0 ];
			int x = train [ id ].next_pos.x , y = train [ id ].next_pos.y;

			if ( pub_state [ i ] == 0 )
			{
				if ( train [ id ].state == 0 )//�����û�б��û���ͣ
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

/*����*/
void check_pub ( void )//dfs����м��ι������ �Ա㶨�����
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

/*�������� �쳵������������*/
static void dfs ( int pub_id , int x , int y )
{
	railway [ x ] [ y ].public_id = pub_id;
	for ( int i = 1; i <= train_amount; i++ )
	{
		/*˳ʱ�뷽������*/
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
		/*��ʱ�뷽������*/
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
