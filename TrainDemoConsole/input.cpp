/*����ģ��*/
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <queue>
#include <functional>
#include<windows.h>
#include "data_structure.h"
using namespace std;
//��������
extern void train_init ( int train_num );
extern void check_pub ( void );
extern void station_init ( void );
//��������
extern vector<Train> train;
extern Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];
extern bool have_input;//�̼߳�ͨ����
extern int train_amount;//ʵ�ʻ�����
extern FILE* file_output;//��׼����ļ�
extern int stage;//���� �ٶ�����Ϊ1 �������Ϊ2
extern vector <int > pub_speed [ MAXNUM_QUEUE ];
extern vector <int > pub_byturn [ MAXNUM_QUEUE ];

void input ( void )
{
	char in1 , in2;
	scanf ( "%c" , &in1 );
	if ( in1 == '\n' ) return;
	scanf ( "%c" , &in2 );
	getchar ( );
	if ( in1 == 's'&&in2 == 't' )//ͣ��
	{
		printf ( "������Ҫֹͣ�Ļ𳵱�ţ�����0���أ�" );
		int id;
		scanf ( "%d" , &id );
		if ( id == 0 ) return;//����0����

		train [ id ].speed = 0;
		train [ id ].state = 1;

		//ɾ��������������������
		if ( train [ id ].pub_id == -1 )
		{
			for ( int i = 1; i <= MAXNUM_QUEUE; i++ )
			{
				if ( stage == 1 )//�ٶ����Ȳ���
				{
					vector<int>::iterator result = find ( pub_speed [ i ].begin ( ) , pub_speed [ i ].end ( ) , id );
					if ( result != pub_speed [ i ].end ( ) )
					{
						pub_speed [ i ].erase ( result );
					}
				}
				else if ( stage == 2 )//�������
				{
					vector<int>::iterator result = find ( pub_byturn [ i ].begin ( ) , pub_byturn [ i ].end ( ) , id );
					if ( result != pub_byturn [ i ].end ( ) )
					{
						pub_byturn [ i ].erase ( result );
					}
				}
			}
		}
		fprintf ( file_output , "��%d���û�ֹͣ��\n\n" , id );
	}
	else if ( in1 == 'b'&&in2 == 't' )//����
	{
		have_input = 1;
		int id;
		printf ( "������Ҫ��ʼ�Ļ𳵣�����0���أ�" );
		scanf ( "%d" , &id );
		if ( id == 0 ) return; //����0����

		train [ id ].speed = train [ id ].speed_init;
		train [ id ].state = 0;
		//����Ҫ����ά������ ��Ϊmain������central_control�����л�
		fprintf ( file_output , "��%d���û�������\n\n" , id );
		have_input = 0;
	}
	else if ( in1 == 'v'&&in2 == 't' )//���ٶ�
	{
		have_input = 1;
		int temp;
		printf ( "������Ҫ�޸��ٶȵĻ𳵱�ţ�����0���أ�" );
		scanf ( "%d" , &temp );
		if ( temp == 0 ) return;//����0����

		printf ( "��%dĿǰ���ٶ���%d���������%d�����ٶȣ�" , temp , train [ temp ].speed_init , temp );
		scanf ( "%d" , &( train [ temp ].speed_init ) );
		if ( train [ temp ].speed != 0 )
		{
			train [ temp ].speed = train [ temp ].speed_init;
		}
		train [ temp ].next_pos.x = train [ temp ].pos.x;
		train [ temp ].next_pos.y = train [ temp ].pos.y;
		fprintf ( file_output , "��%d���û��ı��ٶȣ����ٶ�Ϊ%d��\n\n" , temp , train [ temp ].speed_init );
		have_input = 0;
	}
	else if ( in1 == 'd'&&in2 == 't' )//�ķ���
	{
		have_input = 1;
		printf ( "������Ҫ�޸ķ���Ļ𳵱�ţ�����0���أ�" );
		int id;
		scanf ( "%d" , &id );
		if ( id == 0 ) return;//����0����

		if ( train [ id ].direction == 0 )
		{
			printf ( "��%dĿǰ�ķ�����˳ʱ�룬��������·���" , id );
		}
		else if ( train [ id ].direction == 1 )
		{
			printf ( "��%dĿǰ�ķ�������ʱ�룬��������·���" , id );
		}
		scanf ( "%d" , &( train [ id ].direction ) );
		/*������ķ���Ļ��ڵȴ����빲�� ��ô��Ҫ����ά������*/
		if ( train [ id ].pub_id == -1 )
		{
			for ( int i = 1; i <= MAXNUM_QUEUE; i++ )
			{
				if ( stage == 1 )//�ٶ����Ȳ���
				{
					vector<int>::iterator result = find ( pub_speed [ i ].begin ( ) , pub_speed [ i ].end ( ) , id );
					if ( result != pub_speed [ i ].end ( ) )
					{
						pub_speed [ i ].erase ( result );
					}
				}
				else if ( stage == 2 )//�������
				{
					vector<int>::iterator result = find ( pub_byturn [ i ].begin ( ) , pub_byturn [ i ].end ( ) , id );
					if ( result != pub_byturn [ i ].end ( ) )
					{
						pub_byturn [ i ].erase ( result );
					}
				}
			}
		}
		//����д��next_pos
		train [ id ].next_pos.x = train [ id ].pos.x;
		train [ id ].next_pos.y = train [ id ].pos.y;
		if ( railway [ train [ id ].next_pos.x ] [ train [ id ].next_pos.y ].train_num != -1 && train [ id ].state_station == 0 )
		{
			train [ id ].speed = train [ id ].speed_init;
		}
		fprintf ( file_output , "��%d���û��ı䷽��\n\n" , id );
		have_input = 0;
	}
	else if ( in1 == 't'&&in2 == 'h' )//�Ĳ���
	{
		if ( stage == 1 )
		{
			printf ( "��ǰ�����ǿ쳵���ȣ��л����������������1�������������룺" );
			char a;
			scanf ( "%c" , &a );
			if ( a == '1' )
			{
				stage = 2;
				for ( int i = 0; i < MAXNUM_QUEUE; i++ )
				{
					pub_speed [ i ].clear ( );
				}
				printf ( "���Ʋ����л����µĿ��Ʋ����ǽ�����ԣ�\n\n" );
			}
		}
		else if ( stage == 2 )
		{
			printf ( "��ǰ�����ǽ�����ԣ��л����쳵����������1�������������룺" );
			char a;
			scanf ( "%c" , &a );
			if ( a == '1' )
			{
				stage = 1;
				for ( int i = 0; i < MAXNUM_QUEUE; i++ )
				{
					pub_byturn [ i ].clear ( );
				}
				printf ( "���Ʋ����л����µĿ��Ʋ������ٶ����Ȳ��ԣ�\n\n" );
			}
		}
	}
	else if ( in1 == 'n'&&in2 == 't' )//������
	{
		Train temp;
		train.push_back ( temp );
		train_amount++;
		train_init ( train_amount );
		check_pub ( );
		station_init ( );
		printf ( "�û�������һ���𳵣�\n\n" );
	}
	else if ( in1 == 's'&&in2 == 's' )//վ̨
	{
		station_init ( );
	}
	else if ( in1 == 's'&&in2 == 'a' )//ͣ�����л�
	{
		for ( int i = 1; i <= train_amount; i++ )
		{
			train [ i ].speed = 0;
			train [ i ].state = 1;
		}
	}
	else if ( in1 == 'b'&&in2 == 'a' )//��ʼ���л�
	{
		for ( int i = 1; i <= train_amount; i++ )
		{
			train [ i ].speed = train [ i ].speed_init;
			train [ i ].state = 0;
		}
	}
}