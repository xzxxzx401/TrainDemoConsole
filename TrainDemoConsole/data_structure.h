/*���ݽṹ���� ����const���� point �� ���*/

/************const��������******************/
const int MAXNUM_RAILWAY = 200;//�������
const int MAXNUM_TRAIN_NUM = 200;//��������
const int MAXNUM_QUEUE = 100;//��󹫹������������
const int time_space = 1000;//ʱ����
/************����const��������************/

/************point�ṹ�� ��ʾ����**********/
typedef struct point
{
	int x = 0;
	int y = 0;
} Point;

/*****************��********************/
typedef struct a_train
{
	int start_time = 0;//����ʱ��
	int speed = 0;//���ٶ�
	int speed_init = 0;//��Ĭ���ٶ�
	Point pos;//��λ��
	bool direction = 0;//�𳵷��� 0˳1��
	int state = 0; //�ӵ���ͣ����ʱΪ1 ����Ϊ0 
	int state_station = 0;//����վ̨ͣΪ2 ����Ϊ0
	int time = 0;//��ʱ��
	//int train_flag = 0;//��ʾ��״̬ 1ͣ 0��
	Point next_pos;//����һ���λ��
	int checking_pub_id = 0;////��ʾ���������еĹ��������� �ǹ������Ϊ0
	int pub_id = 0;//��ʾ���������еĹ��������� �ǹ������Ϊ0 �ȴ����������-1
	int train_id = 0;//�𳵱��
	int waiting_time = 0;//ͣ��ʱ�� ��station_time��Ӧ

} Train ;



/*****************���********************/
typedef struct
{
	int train_num = 0;//�����𳵵���� 0��ʾ�޹�� -1��ʾ�������
	int direction [ 2 ] [ MAXNUM_TRAIN_NUM ] = { { 0 } };
	//���˳����ʱ��ǰ���ķ��� ������1 ����2 ����3 ����4
	//int public_lenth [ MAXNUM_TRAIN_NUM ];//�������ʣ�೤��
	bool is_public [ MAXNUM_TRAIN_NUM ] = { 0 };//����ĳ���� �Ƿ��ǹ������
	bool is_using = 0;
	int public_id = 0;//�������ʶ����� ��Ӧ������ 
	int station_time [ MAXNUM_TRAIN_NUM ] = { 0 };//��ʾĳһ���ڸõ��ͣ��ʱ��
	bool is_station = 0;//��ʾ�ڴ˴��Ƿ��л�ͣ��
} Railway;
