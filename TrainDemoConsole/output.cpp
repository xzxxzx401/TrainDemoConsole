/*���ģ��*/

#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include "data_structure.h"
using namespace std;

/************����ȫ�ֱ���*****************/
extern vector<Train> train;
extern Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];
extern bool pub_state [ MAXNUM_QUEUE ];
extern int train_amount;
extern FILE* file_output;
extern int stage;
extern int maxnwidth;//���ʵ��ʹ�õ������ �����
extern int maxnhight;//���ʵ��ʹ�õ����߶� �����
extern int maxnqueue;//ʵ��ʹ�õĶ����� ����ѭ����
extern bool pub_state [ MAXNUM_QUEUE ];//����״̬���� 1��ʾռ�� 0��ʾ����
/***************��������*****************/


void output ( void )
{
	/***********�����д�ӡ******************/
	static int my_time = 1;
	system ( "cls" );
	printf ( "ϵͳ������%d��\n*��ʾ�����#��ʾ���������$��ʾվ̨�����ֱ�ʾ�𳵣�\n\n" , my_time );
	if ( stage == 1 )
	{
		printf ( "��ǰ����Ϊ�쳵���Ȳ��ԣ�\n\n" );
	}
	else if ( stage == 2 )
	{
		printf ( "��ǰ����Ϊ������ԣ�\n\n" );
	}
	printf ( "ֹͣ�𳵣�����'st'�����л𳵣�����'bt'��\n\n" );
	printf ( "���Ļ𳵵��ٶȣ�����'vt'�����Ļ����з�������'dt'��\n\n" );
	printf ( "�л���������'th'���½�������'nt'���޸�վ̨��������'ss'��\n\n" );
	printf ( "ֹͣȫ��������'sa'����ʼȫ��������'ba'��\n" );
	printf ( "\n" );
	/***********��ӡ����ͻ�***************/
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
	/***********������ӡ����ͻ�***************/

	printf ( "\n" );

	/**************��ӡ����Ϣ***************/
	for ( int i = 1; i <= train_amount; i++ )
	{
		if ( train [ i ].start_time > 0 )
		{
			printf ( "��%d����%d�뿪ʼ��\n\n" , i , train [ i ].start_time );
			continue;
		}
		if ( train [ i ].state == 1 )
		{
			printf ( "��%d���û���ͣ��\n\n" , i );
			continue;
		}
		if ( train [ i ].state_station == 2 )
		{
			printf ( "��%d����վ̨ͣ�� ʣ��ʱ��%d��\n\n" , i , railway [ train [ i ].pos.x ] [ train [ i ].pos.y ].station_time [ i ] - train [ i ].waiting_time );
			continue;
		}
		printf ( "��%d�ٶ�Ϊ%d��" , i , train [ i ].speed );
		if ( train [ i ].direction == 0 )
		{
			printf ( "��%d˳ʱ����ʻ��" , i );
		}
		else if ( train [ i ].direction == 1 )
		{
			printf ( "��%d��ʱ����ʻ��" , i );
		}
		if ( train [ i ].pub_id == -1 )
		{
			printf ( "��%d���ڵȴ����빫�������\n" , i );
		}
		else if ( train [ i ].pub_id == 0 )
		{
			printf ( "��%d���ڷǹ��������ʻ��\n" , i );
		}
		else if ( train [ i ].pub_id > 0 )
		{
			printf ( "��%d���ڹ��������ʻ��\n" , i );
		}
		printf ( "\n" );
	}
	for ( int i = 1; i < maxnqueue; i++ )
	{
		if ( pub_state [ i ] == 1 )printf ( "����%d��ռ�ã�\n" , i );
		else printf ( "����%d���У�\n" , i );
	}
	
	/**************������ӡ����Ϣ***************/

	/*************���������д�ӡ**********/

	/**************�ļ���ӡ*****************/
	
	fprintf ( file_output , "ϵͳ������%d��\n" , my_time );
	for ( int i = 1; i <= train_amount; i++ )
	{
		if ( train [ i ].start_time > 0 )
		{
			fprintf ( file_output , "��%d����%d�뿪ʼ��\n\n" , i , train [ i ].start_time );
			continue;
		}
		if ( train [ i ].state == 1 )
		{
			fprintf ( file_output , "��%d���û���ͣ��\n\n" , i );
			continue;
		}
		fprintf ( file_output , "��%d�ٶ�Ϊ%d��" , i , train [ i ].speed );
		if ( train [ i ].direction == 0 )
		{
			fprintf ( file_output , "��%d˳ʱ����ʻ��" , i );
		}
		else if ( train [ i ].direction == 1 )
		{
			fprintf ( file_output , "��%d��ʱ����ʻ��" , i );
		}
		if ( train [ i ].pub_id == -1 )
		{
			fprintf ( file_output , "��%d���ڵȴ����빫�������\n" , i );
		}
		else if ( train [ i ].pub_id == 0 )
		{
			fprintf ( file_output , "��%d���ڷǹ��������ʻ��\n" , i );
		}
		else if ( train [ i ].pub_id > 0 )
		{
			fprintf ( file_output , "��%d���ڹ��������ʻ��\n" , i );
		}
		fprintf ( file_output , "\n" );
	}
	fflush ( file_output );
	/*************�����ļ���ӡ*****************/

	my_time++;//ʱ���1
}
