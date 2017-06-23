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
/****************����ȫ�ֱ���**********************/

vector<Train> train;//��ʵ��������
Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];//���ʵ��������
bool pub_state [ MAXNUM_QUEUE + 1 ] = { 0 };//����״̬���� 1��ʾռ�� 0��ʾ����
int train_amount = 0;//ʵ�ʻ�����
FILE* file_output = NULL;//��׼����ļ�
bool have_input = 0;//�̼߳�ͨ����
int stage = 1;//���� �ٶ�����Ϊ1 �������Ϊ2
bool mp [ MAXNUM_TRAIN_NUM + 1 ] [ MAXNUM_TRAIN_NUM + 1 ];//������� mp[i][j]==0 ��ʾi����j  ==1��ʾj����i
int maxnwidth = 0;//���ʵ��ʹ�õ������ �����
int maxnhight = 0;//���ʵ��ʹ�õ����߶� �����
int maxnqueue = 0;//ʵ��ʹ�õĶ����� ����ѭ����
/***************��������ȫ�ֱ���******************/

int main ( void )
{
	memset ( railway , 0 , sizeof ( railway ) );
	memset ( mp , 0 , sizeof ( mp ) );

	/**************�򿪱�׼����ļ�***************/
	file_output = fopen ( "file_output.txt" , "w" );
	/**************�����򿪱�׼����ļ�***************/

	/************����С�𳵵ĸ���*******************/
	printf ( "������С�𳵵ĸ�����" );
	scanf ( "%d" , &train_amount );

	getchar ( );
	/************��������С�𳵵ĸ���*************/
	
	/****************��ʼ����*******************/
	for ( int i = 1; i <= train_amount; i++ )
	{
		Train temp;
		if ( i == 1 ) train.push_back ( temp );
		train.push_back ( temp );
		train_init ( i );
	}
	/**************������ʼ����*****************/

	/**************��ʼ��mp����******************/
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
	/************������ʼ��mp����****************/
	
	check_pub ( );//��鹫�����
	
	station_init ( );//����վ̨��Ϣ

	while ( 1 )
	{
		output ( );
		//��⵽���������ѭ�� �ȴ�input�����޸������ (�߳�ͬ��)
		if ( _kbhit ( ) )
		{
			input ( );
		}

		Sleep ( time_space );
		if ( stage == 1 )
		{
			central_control_speed ( );//��������ٶ�����
		}
		else
		{
			central_control_byturn ( );//�������
		}
		for ( int i = 1; i <= train_amount; i++ )//���л�
		{
			train_running ( i );
		}
	}
}