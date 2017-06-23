/*数据结构定义 包含const常量 point 火车 轨道*/

/************const常量定义******************/
const int MAXNUM_RAILWAY = 200;//轨道长宽
const int MAXNUM_TRAIN_NUM = 200;//最大火车数量
const int MAXNUM_QUEUE = 100;//最大公共轨道队列数量
const int time_space = 1000;//时间间隔
/************结束const常量定义************/

/************point结构体 表示坐标**********/
typedef struct point
{
	int x = 0;
	int y = 0;
} Point;

/*****************火车********************/
typedef struct a_train
{
	int start_time = 0;//启动时间
	int speed = 0;//火车速度
	int speed_init = 0;//火车默认速度
	Point pos;//火车位置
	bool direction = 0;//火车方向 0顺1逆
	int state = 0; //接到暂停命令时为1 否则为0 
	int state_station = 0;//火车因站台停为2 否则为0
	int time = 0;//火车时间
	//int train_flag = 0;//表示火车状态 1停 0开
	Point next_pos;//火车下一秒的位置
	int checking_pub_id = 0;////表示火车现在运行的公共轨道编号 非公共轨道为0
	int pub_id = 0;//表示火车现在运行的公共轨道编号 非公共轨道为0 等待公共轨道是-1
	int train_id = 0;//火车编号
	int waiting_time = 0;//停车时间 与station_time对应

} Train ;



/*****************轨道********************/
typedef struct
{
	int train_num = 0;//所属火车的序号 0表示无轨道 -1表示公共轨道
	int direction [ 2 ] [ MAXNUM_TRAIN_NUM ] = { { 0 } };
	//这点顺、逆时针前进的方向 向右是1 向下2 向左3 向上4
	//int public_lenth [ MAXNUM_TRAIN_NUM ];//公共轨道剩余长度
	bool is_public [ MAXNUM_TRAIN_NUM ] = { 0 };//对于某个火车 是否是公共轨道
	bool is_using = 0;
	int public_id = 0;//公共轨道识别序号 对应入轨队列 
	int station_time [ MAXNUM_TRAIN_NUM ] = { 0 };//表示某一火车在该点的停留时间
	bool is_station = 0;//表示在此处是否有火车停留
} Railway;
