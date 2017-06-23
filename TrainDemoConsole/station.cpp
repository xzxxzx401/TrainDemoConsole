/*վ̨ģ��*/
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

/************����ȫ�ֱ���*****************/
extern vector<Train> train;
extern Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];
extern int train_amount;
extern int maxnwidth;//���ʵ��ʹ�õ������ �����
extern int maxnhight;//���ʵ��ʹ�õ����߶� �����
/************��������ȫ�ֱ���*************/

void station_init ( void )
{
	system ( "cls" );
	/***********��ӡ����ͻ�***************/
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
	/***********������ӡ����ͻ�***************/

	printf ( "���ڿ�ʼ����վ̨�������Ƿ���ļ���ȡ��Y/N:\n" );
	char c_station;
	char c;

	scanf ( "%c" , &c_station );
	getchar ( );

	/***********��ʼ��վ̨***********/
	while ( 1 )
	{
		if ( c_station == 'Y' )//���ļ���ȡ
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
			else//�ļ���ȡʧ��
			{
				printf ( "��ȡվ̨����ʧ�ܣ���Ϊ�ֶ����룡\n" );
				c_station = 'N';
			}
		}
		else if ( c_station == 'N' )//�����ļ���ȡ
		{
			vector<pair<int , int>>vec_station;//���ڱ���
			/*��ȡ�û�����վ̨��Ϣ*/
			while ( 1 )
			{
				printf ( "������վ̨����x y������������-1 -1��\n" );
				int sx , sy;
				scanf ( "%d%d" , &sx , &sy );
				if ( ( sx == -1 && sy == -1 ) )
				{
					vec_station.push_back ( make_pair ( sx , sy ) );
					break;
				}
				else if ( railway [ sx ] [ sy ].train_num == 0 )
				{
					printf ( "��������������괦û�й�����������룡\n" );
				}
				else
				{
					railway [ sx ] [ sy ].is_station = 1;
					vec_station.push_back ( make_pair ( sx , sy ) );
					printf ( "��������Ҫͣ���Ļ𳵺�ͣ��ʱ�䣬��0 0������" );
					while ( 1 )
					{
						int t_train , t_time;
						scanf ( "%d%d" , &t_train , &t_time );
						if ( t_train > train_amount )
						{
							printf ( "�𳵲����ڣ��������룡\n" );
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
			/*����*/
			printf ( "�Ƿ��뱣��վ̨������Y/N" );
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
					printf ( "����վ̨����ʧ�ܣ�\n" );
				}
				else
				{
					int sum = vec_station.size ( );
					for ( int i = 0; i < sum; i++ )
					{
						fprintf ( station_save_file , "%d %d\n" , vec_station [ i ].first , vec_station [ i ].second );
					}
					printf ( "����վ̨�����ɹ���\n" );
				}
				fclose ( station_save_file );
			}
			/*��������*/
			break;
		}
		else
		{
			printf ( "����������������룡\n" );
			scanf ( "%c" , &c_station );
			getchar ( );
		}
	}
}