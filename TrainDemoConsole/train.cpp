/*��ģ�� �����𳵹���ĳ�ʼ��������*/
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

/*****************����ȫ�ֱ���****************/
extern vector<Train> train;
extern Railway railway [ MAXNUM_RAILWAY + 1 ] [ MAXNUM_RAILWAY + 1 ];
extern bool pub_state [ MAXNUM_QUEUE ];//����״̬���� 1��ʾռ�� 0��ʾ����
extern vector <int > pub_speed [ MAXNUM_QUEUE ];
extern vector <int > pub_byturn [ MAXNUM_QUEUE ];
extern int stage;//���� �ٶ�����Ϊ1 �������Ϊ2
 /*****************��������****************/

/****************��������*****************/
//void train_run_next ( int train_num );
void check_state ( int train_num );
/**************��������*******************/

/********��ȡ�𳵼������Ϣ����**********/
static void train_read ( int train_num )
{
	printf ( "�Ƿ���ļ���ȡ��%d��Ĭ�����ã�Y/N:" , train_num );
	char y_n;
	scanf ( "%c" , &y_n );
	getchar ( );
	while ( 1 )
	{
		if ( y_n == 'Y' )//���ļ�����
		{
			/******************�����ַ��� ��ȡ�ļ���*********/
			string str1 , str2;
			str1 = "train" ;
			stringstream stream;
			stream << train_num;
			stream >> str2;
			str1.append ( str2 );
			str1.append ( ".txt" );
			const char *t = str1.data ( );
			/********************��������******************/

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
				
				/*****************��ʼ�����*********************/

				int ax , ay , bx , by , x0 , y0;
				fscanf ( f1 , "%d%d" , &x0 , &y0 );
				ax = x0 , ay = y0;
				bool flag = 1;
				int i , j;
				//���𳵷���д��������
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
						/*˳ʱ�뷽��*/

						//�жϻ����з��� ����i�Դ������յ�����
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


						/*��ʱ�뷽��*/

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
						//�жϻ����з��� ����i�Դ������յ�����
						/*˳ʱ�뷽��*/
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
							//�жϹ������
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

						/*��ʱ�뷽��*/
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
						printf ( "����������������룡\n" );
					}
				}
				/*****************������ʼ�����****************/

				printf ( "��ȡ��%d�����ɹ���\n" , train_num );
				fclose ( f1 );
				break;
			}
			else//��ȡ�ļ�ʧ�� ��y_n��ΪN ��һ��ѭ��ֱ�ӽ��벻���ļ����������
			{
				printf ( "��ȡ��%d����ʧ�ܣ�����Ϊ�������룡\n" , train_num );
				y_n = 'N';
			}
		}
		else if ( y_n == 'N' )//�����ļ�����
		{
			vector< pair<int , int> > vec2;//���ڼ�¼�����Ϣ
			printf ( "�������%d����ʱ�䣺" , train_num );
			scanf ( "%d" , &( train [ train_num ].start_time ) );
			printf ( "�������%d�ٶȣ�(��Ҫ̫��)" , train_num );
			scanf ( "%d" , &( train [ train_num ].speed_init ) );
			train [ train_num ].speed = train [ train_num ].speed_init;
			printf ( "�������%d��ʼλ��x��y(������%d)��" , train_num , MAXNUM_RAILWAY );
			while ( 1 )
			{
				scanf ( "%d%d" , &( train [ train_num ].pos.x ) , &( train [ train_num ].pos.y ) );
				if ( train [ train_num ].pos.x > MAXNUM_RAILWAY || train [ train_num ].pos.y > MAXNUM_RAILWAY || train [ train_num ].pos.x < 1 || train [ train_num ].pos.y < 1 )
				{
					printf ( "�������곬����Χ�����������룡\n" );
				}
				else
				{
					break;
				}
			}
			train [ train_num ].next_pos.x = train [ train_num ].pos.x;
			train [ train_num ].next_pos.y = train [ train_num ].pos.y;
			printf ( "�������%d���з��� 0˳ 1�棺" , train_num );
			while ( 1 )
			{
				scanf ( "%d" , &( train [ train_num ].direction ) );
				if ( train [ train_num ].direction != 0 && train [ train_num ].direction != 1 )
				{
					printf ( "��������������룡\n" );
				}
				else
				{
					break;
				}
			}
			

			/*��ʼ�����*/
			printf ( "�밴˳ʱ�������%d�����������x y(������%d)����-1 -1����\n" , train_num , MAXNUM_RAILWAY );
			int ax , ay , bx , by , x0 , y0;
			while ( 1 )
			{
				scanf ( "%d%d" , &x0 , &y0 );
				if ( x0 > MAXNUM_RAILWAY || x0 < -1 || y0<-1 || y0>MAXNUM_RAILWAY )
				{
					printf ( "��������������룡\n" );
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
						printf ( "��������������룡\n" );
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
					printf ( "�������\n" );
				}
				if ( bx == ax )
				{
					/*˳ʱ�뷽��*/

					//�жϻ����з��� ����i�Դ������յ�����
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


					/*��ʱ�뷽��*/

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
					//�жϻ����з��� ����i�Դ������յ�����
					/*˳ʱ�뷽��*/
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
						//�жϹ������
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

					/*��ʱ�뷽��*/
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
					printf ( "����������������룡\n" );
				}
			}
			/*������ʼ�����*/

			printf ( "�Ƿ��뱣���%dĬ�ϲ�����Y/N:" , train_num );
			char y_n2 ;
			scanf ( "%c" , &y_n2 );
			getchar ( );
			if ( y_n2 == 'Y' )
			{
				/******************�����ַ��� ��ȡ�ļ���*********/
				string str1 , str2;
				str1 = "train" ;
				stringstream stream;
				stream << train_num;
				stream >> str2;
				str1.append ( str2 );
				str1.append ( ".txt" );
				const char *t = str1.data ( );
				/********************��������******************/

				/********************�������******************/
				FILE* f1;
				f1 = fopen ( t , "w" );
				if ( f1 == NULL )
				{
					printf ( "�����%d�Ĳ���ʧ�ܣ�\n" , train_num );
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
					printf ( "�����%d�Ĳ����ɹ���\n" , train_num );
				}
				fclose ( f1 );
			}
			break;
		}
		else//�쳣���봦��
		{
			printf ( "����������������룡\n" );
			scanf ( "%c" , &y_n );
			getchar ( );
		}
	}
	
}

/**********���л𳵺���**********/
static void train_run ( int train_num )//���л�
{
	int speed;
	bool direction;
	direction = train [ train_num ] .direction;
	speed = train [ train_num ] .speed;
	

	int *x = &( train [ train_num ].pos.x ) , *y = &( train [ train_num ].pos.y );
	int *xn = &( train [ train_num ].next_pos.x ) , *yn = &( train [ train_num ].next_pos.y );

	//����ȴ�վ̨ʱ���Ӧ�ȴ�ʱ��һ�� ��ô�ͷŻ�
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
	//�������վ̨�ȴ�ʱ�� ��ô�ȴ�ʱ��++
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
			//������1 ����2 ����3 ����4
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
			//�������վ̨ ��ô�������»� ����nextλ��
			if ( railway [ *x ] [ *y ].station_time [ train_num ] > 0 )
			{
				train [ train_num ].state_station = 2;
				train [ train_num ].next_pos.x = train [ train_num ].pos.x;
				train [ train_num ].next_pos.y = train [ train_num ].pos.y;
				break;
			}
			//��������� ͣ��
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
			//������1 ����2 ����3 ����4
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
			//�������վ̨��ô���»� ����nextλ��
			if ( railway [ *x ] [ *y ].station_time [ train_num ] > 0 )
			{
				train [ train_num ].state_station = 2;
				train [ train_num ].next_pos.x = train [ train_num ].pos.x;
				train [ train_num ].next_pos.y = train [ train_num ].pos.y;
				break;
			}
			//��������� ͣ��
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

/***********Ԥ������һ��λ��*********/
/*void train_run_next ( int train_num )//�������һ�ε�λ��
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
			//������1 ����2 ����3 ����4
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
			//������1 ����2 ����3 ����4
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

/*******����״̬���ͷŹ���******/
static void check_state ( int train_num )//����״̬ �ͷŹ������
{
	//0��ʾ�ڷǹ�����ʻ 1��ʾ���빫��
	int x = train [ train_num ].pos.x , y = train [ train_num ].pos.y ;
	int pre_pubid = train [ train_num ].checking_pub_id;
	int now_pubid = railway [ x ] [ y ].public_id;

	if ( pre_pubid != now_pubid )
	{
		if ( pre_pubid != 0 )
		{
			pub_state [ pre_pubid ] = 0;
			//�������״̬��������ͷ�
		}
		train [ train_num ].pub_id = now_pubid;
		train [ train_num ].checking_pub_id = now_pubid;
		//�𳵵�pub_id��0
	}
}

/************��ʼ����*************/
void train_init ( int train_num )//��ʼ����
{
	train_read ( train_num );
}

/*���л𳵣�Ԥ����Ͽ��һ��λ�ã����״̬*/
void train_running ( int train_num )//���л�
{
	if ( train [ train_num ].start_time > 0 )//������ڵȴ�����״̬��ô�����л�
	{
		train [ train_num ].start_time--;
		return;
	}
	train_run ( train_num );
}
