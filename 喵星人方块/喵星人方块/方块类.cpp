#include "stdafx.h"
#include "方块类.h"
namespace cat{
	Tetris::Tetris(HWND hwnd) ://默认构造函数，对所有成员进行初始化
		frame(std::vector<std::vector<unsigned>>(20, std::vector<unsigned>(10, 0))),
		block(),
		state_total(0),
		state_block(0),
		type_current(0),
		type_next(0),
		direction_block(0),
		total_number(0),
		temp_number(0),
		client_x(0),
		client_y(0),
		block_size(0),
		score(0),
		level(1),
		hWnd(hwnd),
		timer(0),
		u(1,7)//将随机分布类型初始化为1到7，分别代表7种积木
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		e.seed(sys.wMilliseconds);//使用系统时间的毫秒位初始化随机数引擎
		pen_frame1 = CreatePen(PS_SOLID, 1, RGB(184,115,51));
		pen_frame2 = CreatePen(PS_SOLID, 1, 0x1F1F1F);
		brush_block1 = CreateSolidBrush(RGB(255, 0, 0));
		brush_block2 = CreateSolidBrush(RGB(0, 255, 255));
		brush_block3 = CreateSolidBrush(RGB(255, 125, 0));
		brush_block4 = CreateSolidBrush(RGB(0, 0, 255));
		brush_block5 = CreateSolidBrush(RGB(255, 0, 255));
		brush_block6 = CreateSolidBrush(RGB(0, 255, 0));
		brush_block7 = CreateSolidBrush(RGB(255, 255, 0));
	}
	Tetris::~Tetris()//析构函数，释放自定义画笔等
	{
		DeleteObject(pen_frame1);
		DeleteObject(pen_frame2);
		DeleteObject(brush_block1);
		DeleteObject(brush_block2);
		DeleteObject(brush_block3);
		DeleteObject(brush_block4);
		DeleteObject(brush_block5);
		DeleteObject(brush_block6);
		DeleteObject(brush_block7);
	}
//////////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::get_TGM_block()
	{
		if (state_total == 2 || state_total == 3 || state_block != 0){ return  false; }
		if (state_total == 1 && state_block == 0 && type_next == 0){//若游戏第一次开始,此时下个积木的类型为0
			state_total = 1;//将游戏状态设置为正在进行
			type_current = u(e);//使用随机数获得一个积木类型并保留在type_current成员中
			type_next = u(e);//将下个积木的类型保存在type_next中
			set_TGM_block();//设置积木数据
			return true;
		}
		else if (state_total == 1 && state_block == 0)
		{
			type_current = type_next;//使用下个积木的类型
			type_next = u(e);//再次获得下个积木的类型并保存在type_next中
			set_TGM_block();//设置积木数据
			return true;
		}
		return false;
	}
////////////////////////////////////////////////////////////////////////////////////////////////
	void Tetris::set_TGM_block()
	{
		switch (type_current)
		{
		case 1://类型1设置为长条
			block[0][0] = 3; block[0][1] = 0; 
			block[1][0] = 4; block[1][1] = 0;
			block[2][0] = 5; block[2][1] = 0;
			block[3][0] = 6; block[3][1] = 0;
			break;
		case 2://类型2设置为品字
			block[0][0] = 3; block[0][1] = 0;
			block[1][0] = 4; block[1][1] = 0;
			block[2][0] = 5; block[2][1] = 0;
			block[3][0] = 4; block[3][1] = 1;
			break;
		case 3://类型3设置为品左
			block[0][0] = 3; block[0][1] = 1;
			block[1][0] = 3; block[1][1] = 0;
			block[2][0] = 4; block[2][1] = 0;
			block[3][0] = 5; block[3][1] = 0;
			break;
		case 4://类型4设置为品右
			block[0][0] = 3; block[0][1] = 0;
			block[1][0] = 4; block[1][1] = 0;
			block[2][0] = 5; block[2][1] = 0;
			block[3][0] = 5; block[3][1] = 1;
			break;
		case 5://类型5设置为反Z
			block[0][0] = 3; block[0][1] = 1;
			block[1][0] = 4; block[1][1] = 1;
			block[2][0] = 4; block[2][1] = 0;
			block[3][0] = 5; block[3][1] = 0;
			break;
		case 6://类型6设置为Z
			block[0][0] = 3; block[0][1] = 0;
			block[1][0] = 4; block[1][1] = 0;
			block[2][0] = 4; block[2][1] = 1;
			block[3][0] = 5; block[3][1] = 1;
			break;
		case 7://类型7设置为田
			block[0][0] = 4; block[0][1] = 1;
			block[1][0] = 4; block[1][1] = 0;
			block[2][0] = 5; block[2][1] = 0;
			block[3][0] = 5; block[3][1] = 1;
			break;
		default:break;
		}
		direction_block = 1;//将当前活动积木的朝向设置为1，也是它第一次出现的朝向
	}
/////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::insert_block()
	{
		if (state_total == 1 && state_block == 0)//首先判断游戏是否正在进行中以及当前是否有新获取还未插入的积木
		{ 
			for (int n(0); n != 4; ++n)
			{
				if (frame[block[n][1]][block[n][0]] != 0)
				{ 
					state_total = 3;
					KillTimer(hWnd, timer--);//杀死计时器
					InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
					return false;
				}//判断积木第一次出现的位置是否已经有方块了，若是则将游戏状态设置为失败并返回
			}
			for (int n(0); n != 4; ++n)
			{
				frame[block[n][1]][block[n][0]] = type_current;//确认有位置插入后，执行插入操作
			}
			state_block = 1;//将积木的状态设置为当前已有活跃积木
			InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
			return true;
		}
		return false;
	}
//////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::down_shift()
	{
		if (state_total == 1 && state_block == 1)//首先判断游戏是否正在进行中并且是否有一个活动的积木
		{
			for (int n(0); n != 4; ++n)//判断是否具备下移条件
			{
				if (block[n][1] >= 19){ state_block = 2; return false; }//判断是否已经触底，若是则返回下移失败
				bool b(0);
				for (int i(0); i != 4; ++i)
				{
					if (block[n][0] == block[i][0] && (block[n][1] + 1) == block[i][1])//判断下面的点是否属于当前积木
					{
						b = true;
					}
				}
				if (frame[block[n][1] + 1][block[n][0]] != 0 && b == false){ state_block = 2; return false; }//判断下移位置是否已经有方块了，若是且不属于当前积木则返回下移失败
			}
			for (int n(0); n != 4; ++n)//执行下落操作
			{
				frame[block[n][1]][block[n][0]] = 0;//将积木的当前位置清空
			}
			for (int n(0); n != 4; ++n)//执行下落操作
			{
				frame[++block[n][1]][block[n][0]] = type_current;//将积木的纵坐标加1并将对应的位置的类型设置为当前积木的类型
			}
			InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面，显示移动效果
			return true;//下移成功返回
		}
		return false;//下移失败返回
	}
////////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::left_shift()
	{
		if (state_total == 1 && state_block == 1)//首先判断游戏是否正在进行中并且是否有一个活动的积木
		{
			for (int n(0); n != 4; ++n)//判断是否具备左移条件
			{
				if (block[n][0] <= 0){ return false; }//判断是否已经靠边，若是则返回左移失败(注意*靠边一定要在是否有方块之前判断)
				bool b(0);
				for (int i(0); i != 4; ++i)
				{
					if ((block[n][0] - 1) == block[i][0] && block[n][1] == block[i][1])//判断左边的点是否属于当前积木
					{
						b = true;
					}
				}
				if (frame[block[n][1]][block[n][0] - 1] != 0 && b == false){ return false; }//判断左移位置是否已经有方块了，若是则返回左移失败
			}
			for (int n(0); n != 4; ++n)//执行左移操作
			{
				frame[block[n][1]][block[n][0]] = 0;//将积木的当前位置清空
			}
			for (int n(0); n != 4; ++n)//执行左移操作
			{
				frame[block[n][1]][--block[n][0]] = type_current;//将积木的横坐标减1并将对应的位置的颜色设置为积木的类型
			}
			InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面，显示移动效果
			return true;//左移成功返回
		}
		return false;//左移失败返回
	}
//////////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::right_shift()
	{
		if (state_total == 1 && state_block == 1)//首先判断游戏是否正在进行中并且是否有一个活动的积木
		{
			for (int n(0); n != 4; ++n)//判断是否具备右移条件
			{
				if (block[n][0] >= 9){ return false; }//判断是否已经靠边，若是则返回右移失败(注意*靠边一定要在是否有方块之前判断)
				bool b(0);
				for (int i(0); i != 4; ++i)
				{
					if ((block[n][0] + 1) == block[i][0] && block[n][1] == block[i][1])//判断右边的点是否属于当前积木
					{
						b = true;
					}
				}
				if (frame[block[n][1]][block[n][0] + 1] != 0 && b == false){ return false; }//判断右移位置是否已经有方块了，若是则返回右移失败
			}
			for (int n(0); n != 4; ++n)//执行右移操作
			{
				frame[block[n][1]][block[n][0]] = 0;//将积木的当前位置清空
			}
			for (int n(0); n != 4; ++n)//执行右移操作
			{
				frame[block[n][1]][++block[n][0]] = type_current;//将积木的横坐标加1并将对应的位置的颜色设置为积木的类型
			}
			InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面，显示移动效果
			return true;//右移成功返回
		}
		return false;//右移失败返回
	}
///////////////////////////////////////////////////////////////////////////////////////////
	void Tetris::clear_block()
	{
		frame[block[0][1]][block[0][0]] = 0;
		frame[block[1][1]][block[1][0]] = 0;
		frame[block[2][1]][block[2][0]] = 0;
		frame[block[3][1]][block[3][0]] = 0;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::TGM_rotation()
	{
		if (state_total == 1 && state_block == 1)//首先判断游戏是否正在进行中并且是否有一个活动的积木
		{
			switch (type_current)
			{
			case 1:
				switch (direction_block)
				{
				case 1:
					if (block[0][1] <= 0 || block[0][1] >= 18){ return false; }//判断长条转动时是否会越界
					if (frame[block[2][1] - 1][block[2][0]] != 0 ||
						frame[block[2][1] + 1][block[2][0]] != 0 ||
						frame[block[2][1] + 2][block[2][0]] != 0){ return false; }//判断长条转动时是否会重叠
					clear_block();//擦除当前位置
					frame[--block[0][1]][block[0][0] += 2] = type_current;
					frame[block[1][1]][++block[1][0]] = type_current;
					frame[++block[2][1]][block[2][0]] = type_current;
					frame[block[3][1] += 2][--block[3][0]] = type_current;
					direction_block = 2;
					InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
					return true;
					break;
				case 2:
					if (block[0][0] >= 9 || block[0][0] <= 1){ return false; }//判断长条转动时是否会越界
					if (frame[block[1][1]][block[1][0] - 2] != 0 ||
						frame[block[1][1]][block[1][0] - 1] != 0 ||
						frame[block[1][1]][block[1][0] + 1] != 0){ return false; }//判断长条转动时是否会重叠
					clear_block();//擦除当前位置
					frame[++block[0][1]][block[0][0] -= 2] = type_current;
					frame[block[1][1]][--block[1][0]] = type_current;
					frame[--block[2][1]][block[2][0]] = type_current;
					frame[block[3][1] -= 2][++block[3][0]] = type_current;
					direction_block = 1;
					InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
					return true;
					break;
				default:
					break;
				}
				break;
			case 2:
				switch (direction_block)
				{
				case 1:
					if (block[1][1] <= 0){ return false; }//判断基础旋转不越界
					if (frame[block[1][1] - 1][block[1][0]] != 0){ return false; }//判断基础旋转不占位
					clear_block();//擦除当前位置
					frame[--block[0][1]][++block[0][0]] = type_current;
					frame[block[1][1]][block[1][0]] = type_current;
					frame[++block[2][1]][--block[2][0]] = type_current;
					frame[--block[3][1]][--block[3][0]] = type_current;
					direction_block = 2;
					InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
					return true;
					break;
				case 2:
					if (block[0][0]<9 &&//判断基础旋转不越界
						frame[block[2][1]][block[2][0] - 1] == 0 &&//判断基础旋转不占位
						frame[block[2][1]][block[2][0] + 1] == 0)//判断基础旋转不占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] += 2][++block[0][0]] = type_current;
						frame[++block[1][1]][block[1][0]] = type_current;
						frame[block[2][1]][--block[2][0]] = type_current;
						frame[block[3][1]][++block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[0][0]<8 &&//判断右移旋转不越界
						frame[block[1][1]][block[1][0] + 1] == 0 &&//判断右移旋转不占位
						frame[block[2][1]][block[2][0] + 1] == 0 &&//判断右移旋转不占位
						frame[block[2][1]][block[2][0] + 2] == 0)//判断右移旋转不占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] += 2][block[0][0] += 2] = type_current;
						frame[++block[1][1]][++block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[block[3][1]][block[3][0] += 2] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[3][0]>0 &&//判断左移旋转不越界
						frame[block[2][1]][block[2][0] - 1] == 0 &&//判断左移旋转不占位
						frame[block[2][1]][block[2][0] - 2] == 0)//判断左移旋转不占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] += 2][block[0][0]] = type_current;
						frame[++block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1]][block[2][0] -= 2] = type_current;
						frame[block[3][1]][block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				case 3:
					if (block[3][1] <= 0 || frame[block[3][1] - 1][block[3][0]] != 0){ return false; }//判断是否具备旋转基本条件
					if (frame[block[0][1] - 1][block[0][0]] == 0)//判断基础旋转不占位
					{ 
						clear_block();//擦除当前位置
						frame[block[0][1]][--block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0]] = type_current;
						frame[block[2][1] -= 2][++block[2][0]] = type_current;
						frame[block[3][1]][++block[3][0]] = type_current;
						direction_block = 4;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (frame[block[3][1] - 1][block[3][0] - 1] == 0 &&//判断左移旋转不占位
						frame[block[3][1]][block[3][0] - 1] == 0)//判断左移旋转不占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1]][block[0][0] -= 2] = type_current;
						frame[--block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1] -= 2][block[2][0]] = type_current;
						frame[block[3][1]][block[3][0]] = type_current;
						direction_block = 4;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				case 4:
					if (block[0][0]>0 &&//判断基本旋转不越界
						frame[block[1][1]][block[1][0] - 1] == 0)//判断基本旋转不占位
					{
						clear_block();//擦除当前位置
						frame[--block[0][1]][--block[0][0]] = type_current;
						frame[block[1][1]][block[1][0]] = type_current;
						frame[++block[2][1]][++block[2][0]] = type_current;
						frame[++block[3][1]][--block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[3][0]<9 &&//判断右移旋转不越界
						frame[block[3][1]][block[3][0] + 1] == 0 &&//判断右移旋转不占位
						frame[block[3][1] + 1][block[3][0]] == 0)//判断右移旋转不占位
					{
						clear_block();//擦除当前位置
						frame[--block[0][1]][block[0][0]] = type_current;
						frame[block[1][1]][++block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0] += 2] = type_current;
						frame[++block[3][1]][block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				default:
					break;
				}
				break;
			case 3:
				switch (direction_block)
				{
				case 1:
					if (block[1][1] <= 0 ||//判断旋转是否越界
						frame[block[0][1]][block[0][0] + 1] != 0 ||//判断旋转是否具备基本条件
						frame[block[2][1] - 1][block[2][0]] != 0)//判断旋转是否具备基本条件
					{
						return false;
					}
					if (frame[block[1][1] - 1][block[1][0]] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] -= 2][block[0][0]] = type_current;
						frame[--block[1][1]][++block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[++block[3][1]][--block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (frame[block[3][1] - 1][block[3][0]] == 0 &&//判断右移动旋转是否占位
						frame[block[3][1] + 1][block[3][0]] == 0)//判断右移动旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] -= 2][++block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0] += 2] = type_current;
						frame[block[2][1]][++block[2][0]] = type_current;
						frame[++block[3][1]][block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				case 2:
					if (block[1][0] < 9 &&//判断基础旋转是否越界
						frame[block[3][1]][block[3][0] - 1] == 0 &&//判断基础旋转是否占位
						frame[block[3][1]][block[3][0] + 1] == 0 &&//判断基础旋转是否占位
						frame[block[2][1]][block[2][0] + 1] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[++block[0][1]][block[0][0] += 2] = type_current;
						frame[block[1][1] += 2][++block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0]] = type_current;
						frame[block[3][1]][--block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[1][0] < 8 &&//判断右移旋转是否越界
						frame[block[3][1]][block[3][0] + 1] == 0 &&//判断右移旋转是否占位
						frame[block[3][1]][block[3][0] + 2] == 0 &&//判断右移旋转是否占位
						frame[block[2][1]][block[2][0] + 2] == 0)//判断右移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[++block[0][1]][block[0][0] += 3] = type_current;
						frame[block[1][1] += 2][block[1][0] += 2] = type_current;
						frame[++block[2][1]][++block[2][0]] = type_current;
						frame[block[3][1]][block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[0][0] > 0 &&//判断左移旋转是否越界
						frame[block[3][1]][block[3][0] - 1] == 0 &&//判断左移旋转是否占位
						frame[block[3][1]][block[3][0] - 2] == 0)//判断左移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[++block[0][1]][++block[0][0]] = type_current;
						frame[block[1][1] += 2][block[1][0]] = type_current;
						frame[++block[2][1]][--block[2][0]] = type_current;
						frame[block[3][1]][block[3][0] -= 2] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				case 3:
					if (block[0][1]>0 &&//判断旋转是否越界
						frame[block[2][1] - 1][block[2][0]] == 0 &&//判断旋转是否占位
						frame[block[2][1] - 2][block[2][0]] == 0)//判断旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[++block[0][1]][block[0][0]] = type_current;
						frame[block[1][1]][--block[1][0]] = type_current;
						frame[--block[2][1]][block[2][0]] = type_current;
						frame[block[3][1] -= 2][++block[3][0]] = type_current;
						direction_block = 4;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				case 4:
					if (block[1][0]>0 &&//判断基础旋转是否越界
						frame[block[1][1]][block[1][0] - 1] == 0 &&//判断基础旋转是否占位
						frame[block[2][1]][block[2][0] - 1] == 0 &&//判断基础旋转是否占位
						frame[block[0][1] - 1][block[0][0]] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1]][block[0][0] -= 2] = type_current;
						frame[--block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[++block[3][1]][++block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[0][0]<9 &&//判断右移旋转是否越界
						frame[block[2][1]][block[2][0] + 1] == 0 &&//判断右移旋转是否占位
						frame[block[2][1]][block[2][0] + 2] == 0)//判断右移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1]][--block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0]] = type_current;
						frame[block[2][1]][++block[2][0]] = type_current;
						frame[++block[3][1]][block[3][0] += 2] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[1][0]>1 &&//判断左移旋转是否越界
						frame[block[2][1]][block[2][0] - 1] == 0 &&//判断左移旋转是否占位
						frame[block[2][1]][block[2][0] - 2] == 0 &&
						frame[block[1][1]][block[1][0] - 2] == 0)//判断左移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1]][block[0][0] -= 3] = type_current;
						frame[--block[1][1]][block[1][0] -= 2] = type_current;
						frame[block[2][1]][--block[2][0]] = type_current;
						frame[++block[3][1]][block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				default:
					break;
				}
				break;
			case 4:
				switch (direction_block)
				{
				case 1:
					if (block[0][1] <= 0 ||//判断旋转是否越界
						frame[block[1][1] - 1][block[1][0]] != 0 ||//判断旋转是否具备条件
						frame[block[1][1] + 1][block[1][0]] != 0)//判断旋转是否具备条件
					{
						return false;
					}
					if (frame[block[0][1] + 1][block[0][0]] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[--block[0][1]][++block[0][0]] = type_current;
						frame[block[1][1]][block[1][0]] = type_current;
						frame[++block[2][1]][--block[2][0]] = type_current;
						frame[block[3][1]][block[3][0] -= 2] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (frame[block[2][1] - 1][block[2][0]] == 0)//判断右移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[--block[0][1]][block[0][0] += 2] = type_current;
						frame[block[1][1]][++block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0]] = type_current;
						frame[block[3][1]][--block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				case 2:
					if (block[0][0]<9 &&//判断基础旋转是否越界
						frame[block[3][1] - 1][block[3][0]] == 0 &&//判断基础旋转是否占位
						frame[block[2][1]][block[2][0] + 1] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] += 2][++block[0][0]] = type_current;
						frame[++block[1][1]][block[1][0]] = type_current;
						frame[block[2][1]][--block[2][0]] = type_current;
						frame[--block[3][1]][block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[0][0]<8 &&//判断右移旋转是否越界
						frame[block[2][1]][block[2][0] + 2] == 0 &&//判断右移旋转是否占位
						frame[block[2][1]][block[2][0] + 1] == 0)//判断右移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] += 2][block[0][0] += 2] = type_current;
						frame[++block[1][1]][++block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[--block[3][1]][++block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[3][0]>0 &&//判断左移旋转是否越界
						frame[block[1][1]][block[1][0] - 2] == 0 &&//判断左移旋转是否占位
						frame[block[3][1]][block[3][0] - 1] == 0)//判断左移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] += 2][block[0][0]] = type_current;
						frame[++block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1]][block[2][0] -= 2] = type_current;
						frame[--block[3][1]][--block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				case 3:
					if (block[3][1]<=0 ||//判断旋转是否越界
						frame[block[1][1] - 1][block[1][0]] != 0 ||//判断旋转是否占位
						frame[block[1][1] - 2][block[1][0]] != 0)//判断旋转是否占位
					{
						return false;
					}
					if (frame[block[0][1] - 2][block[0][0]] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1]][--block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0]] = type_current;
						frame[block[2][1] -= 2][++block[2][0]] = type_current;
						frame[--block[3][1]][block[3][0] += 2] = type_current;
						direction_block = 4;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (frame[block[3][1] - 1][block[3][0]] == 0)//判断左移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1]][block[0][0] -= 2] = type_current;
						frame[--block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1] -= 2][block[2][0]] = type_current;
						frame[--block[3][1]][++block[3][0]] = type_current;
						direction_block = 4;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				case 4:
					if (block[0][0]>0 &&//判断基础旋转是否越界
						frame[block[1][1]][block[1][0] - 1] == 0 &&//判断基础旋转是否占位
						frame[block[1][1]][block[1][0] + 1] == 0 && //判断基础旋转是否占位
						frame[block[0][1]][block[0][0] + 1] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[--block[0][1]][--block[0][0]] = type_current;
						frame[block[1][1]][block[1][0]] = type_current;
						frame[++block[2][1]][++block[2][0]] = type_current;
						frame[block[3][1] += 2][block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[3][0]<9 &&//判断右移旋转是否越界
						frame[block[1][1]][block[1][0] + 1] == 0 &&//判断右移旋转是否占位
						frame[block[1][1]][block[1][0] + 2] == 0 && //判断右移旋转是否占位
						frame[block[0][1]][block[0][0] + 2] == 0)//判断右移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[--block[0][1]][block[0][0]] = type_current;
						frame[block[1][1]][++block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0] += 2] = type_current;
						frame[block[3][1] += 2][++block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[0][0]>1 &&//判断左移旋转是否越界
						frame[block[1][1]][block[1][0] - 1] == 0 &&//判断左移旋转是否占位
						frame[block[1][1]][block[1][0] - 2] == 0)//判断左移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[--block[0][1]][block[0][0] -= 2] = type_current;
						frame[block[1][1]][--block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0]] = type_current;
						frame[block[3][1] += 2][--block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				default:
					break;
				}
				break;
			case 5:
				switch (direction_block)
				{
				case 1:
					if (block[2][1] <= 0){ return false; }//判断旋转是否越界
					if (frame[block[0][1] - 1][block[0][0]] == 0 &&//判断基础旋转是否占位
						frame[block[0][1] - 2][block[0][0]] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] -= 2][block[0][0]] = type_current;
						frame[--block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[++block[3][1]][--block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (frame[block[2][1] - 1][block[2][0]] == 0 &&//判断右移旋转是否占位
						frame[block[3][1] + 1][block[3][0]] == 0)//判断右移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] -= 2][++block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0]] = type_current;
						frame[block[2][1]][++block[2][0]] = type_current;
						frame[++block[3][1]][block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[0][0]>0 &&//判断左移旋转是否越界
						frame[block[0][1] - 2][block[0][0] - 1] == 0 &&//判断左移旋转是否占位
						frame[block[0][1] - 1][block[0][0] - 1] == 0 &&//判断左移旋转是否占位
						frame[block[0][1] - 1][block[0][0]] == 0)//判断左移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] -= 2][--block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0] -= 2] = type_current;
						frame[block[2][1]][--block[2][0]] = type_current;
						frame[++block[3][1]][block[3][0] -= 2] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				case 2:
					if (block[2][0]<9 &&//判断基础旋转是否越界
						frame[block[2][1]][block[2][0] + 1] == 0 &&//判断基础旋转是否占位
						frame[block[1][1] + 1][block[1][0]] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] += 2][block[0][0]] = type_current;
						frame[++block[1][1]][++block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[--block[3][1]][++block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[2][0]<8 &&//判断右移旋转是否越界
						frame[block[2][1]][block[2][0] + 1] == 0 &&//判断右移旋转是否占位
						frame[block[2][1]][block[2][0] + 2] == 0 &&//判断右移旋转是否占位
						frame[block[3][1]][block[3][0] + 1] == 0)//判断右移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] += 2][++block[0][0]] = type_current;
						frame[++block[1][1]][block[1][0] += 2] = type_current;
						frame[block[2][1]][++block[2][0]] = type_current;
						frame[--block[3][1]][block[3][0] += 2] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[0][0]>0 &&//判断左移旋转是否越界
						frame[block[3][1]][block[3][0] - 1] == 0 &&//判断左移旋转是否占位
						frame[block[3][1]][block[3][0] - 2] == 0)//判断左移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[block[0][1] += 2][--block[0][0]] = type_current;
						frame[++block[1][1]][block[1][0]] = type_current;
						frame[block[2][1]][--block[2][0]] = type_current;
						frame[--block[3][1]][block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				default:
					break;
				}
				break;
			case 6:
				switch (direction_block)
				{
				case 1:
					if (block[0][1] <= 0){ return false; }//判断旋转是否越界
					if (frame[block[3][1] - 1][block[3][0]] == 0 &&//判断基础旋转是否占位
						frame[block[3][1] - 2][block[3][0]] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[--block[0][1]][block[0][0] += 2] = type_current;
						frame[block[1][1]][++block[1][0]] = type_current;
						frame[--block[2][1]][block[2][0]] = type_current;
						frame[block[3][1]][--block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[3][0]<9 &&//判断右移旋转是否越界
						frame[block[3][1] - 1][block[3][0]] == 0 &&//判断右移旋转是否占位
						frame[block[3][1] - 1][block[3][0] + 1] == 0 &&
						frame[block[3][1] - 2][block[3][0] + 1] == 0)//判断右移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[--block[0][1]][block[0][0] += 3] = type_current;
						frame[block[1][1]][block[1][0] += 2] = type_current;
						frame[--block[2][1]][++block[2][0]] = type_current;
						frame[block[3][1]][block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (frame[block[1][1] - 1][block[1][0]] == 0 &&//判断左移旋转是否占位
						frame[block[0][1] + 1][block[0][0]] == 0)//判断左移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[--block[0][1]][++block[0][0]] = type_current;
						frame[block[1][1]][block[1][0]] = type_current;
						frame[--block[2][1]][--block[2][0]] = type_current;
						frame[block[3][1]][block[3][0] -= 2] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				case 2:
					if (block[2][0] > 0 &&//判断基础旋转是否越界
						frame[block[2][1]][block[2][0] - 1] == 0 &&//判断基础旋转是否占位
						frame[block[1][1] + 1][block[1][0]] == 0)//判断基础旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[++block[0][1]][block[0][0] -= 2] = type_current;
						frame[block[1][1]][--block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0]] = type_current;
						frame[block[3][1]][++block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[0][0] < 9 &&//判断右移旋转是否越界
						frame[block[3][1]][block[3][0] + 2] == 0 &&//判断右移旋转是否占位
						frame[block[1][1] + 1][block[1][0]] == 0)//判断右移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[++block[0][1]][--block[0][0]] = type_current;
						frame[block[1][1]][block[1][0]] = type_current;
						frame[++block[2][1]][++block[2][0]] = type_current;
						frame[block[3][1]][block[3][0] += 2] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					if (block[2][0] > 1 &&//判断左移旋转是否越界
						frame[block[3][1]][block[3][0] - 1] == 0 &&//判断左移旋转是否占位
						frame[block[2][1]][block[2][0] - 1] == 0 &&//判断左移旋转是否占位
						frame[block[2][1]][block[2][0] - 2] == 0)//判断左移旋转是否占位
					{
						clear_block();//擦除当前位置
						frame[++block[0][1]][block[0][0] -= 3] = type_current;
						frame[block[1][1]][block[1][0] -= 2] = type_current;
						frame[++block[2][1]][--block[2][0]] = type_current;
						frame[block[3][1]][block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面
						return true;
					}
					break;
				default:
					break;
				}
				break;
			case 7:
				return true;//田字积木总是可以转动，直接返回
				break;
			default:break;
			}
			return false;
		}
		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::find_lines()
	{
		if (state_total == 1 && state_block == 2)//只有正在游戏并且由积木下落至被固定时才检查
		{
			int temp(0);
			for (auto &n : frame)
			{
				if (std::find(n.cbegin(),n.cend(),0)==n.cend())//若该行满格
				{
					++temp;
					for (auto &m : n){ m = 8; }//将该行的方块类型设置为8，代表该行已经有方块并且满行
				}
			}
			if (temp){ 
				if (timer){ KillTimer(hWnd, timer--); }//杀死循环计时器
				InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面，显示消行效果
				SetTimer(hWnd, ++timer, 500, NULL);
				state_total = 2;
				return true; 
			}//返回true表示有行可以消除
		}
		state_block = 0;//若没有需要消除的行
		if (get_TGM_block())//则立即插入新的积木，以免造成不惜要的停顿
		{
			insert_block();
		}
		return false;//无则返回没有
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	unsigned Tetris::remove_lines()
	{
		if (state_total == 2 && state_block == 2)//只有正在游戏并且由积木下落至被固定时才可能消行
		{
			unsigned temp(0);
			for (std::vector<std::vector<unsigned>>::iterator ite = frame.begin(); ite != frame.end();)
			{
				if ((*ite)[0] == 8)//判断该行是否被标记了
				{
					++temp; 
					ite = frame.erase(ite);//消除该行
					continue;
				}
				++ite;
			}
			std::reverse(frame.begin(), frame.end());//嗯，没规划好导致现在插入蛋痛
			for (unsigned i(0); i != temp; ++i)//嗯，没规划好导致现在插入蛋痛
			{
				frame.push_back(std::vector<unsigned>(10, 0));
			}
			std::reverse(frame.begin(), frame.end());//嗯，没规划好导致现在插入蛋痛
			if (temp)//若有消除行
			{ 
				score += (1 << (temp - 1)) * 100;//更新积分
				total_number += temp;//更新消除的累计行数
				temp_number += temp;//更新当前等级下消除的行数
				if (level<0xFFFFFFFF){ level += (temp_number / 30); }//若达到或超过30行则调整游戏等级
				temp_number %= 30;//重置每局计数
				state_block = 0;//将积木状态设置为没有活跃的积木，以便get_TGM_block()函数随后能正确的处理积木的获取
				state_total = 1;//消行完毕后恢复游戏状态
				if (get_TGM_block())//消除行后应当立即获得新的积木并插入游戏中，以免照成不必要的停顿
				{
					insert_block();
				}
				InvalidateRect(hWnd, NULL, TRUE);//立即刷新界面，显示消行效果
				KillTimer(hWnd, timer--);//杀死消行定时器
				SetTimer(hWnd, ++timer, double(1000) / (sqrt(double(level))), NULL);//按照当前游戏等级设置计时器间隔
				return temp;//返回此次消除的行数
			}
		}
		return 0;//若不消行则返回0，表示消除了0行
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Tetris::clear()
	{
		std::vector<std::vector<unsigned>> temp(20, std::vector<unsigned>(10, 0));
		frame.swap(temp);
		for (int i(0); i != 4;++i)
		{
			block[i][0] = 0;
			block[i][1] = 0;
		}
		state_total = 0;//将游戏恢复到待命状态
		state_block = 0;//以下清除当前游戏数据
		type_current = 0;
		type_next = 0;
		direction_block = 0;
		total_number = 0;
		temp_number = 0;
		score = 0;
		level = 1;
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		e.seed(sys.wMilliseconds);//使用系统时间的毫秒位初始化随机数引擎
		if (timer){ KillTimer(hWnd, timer--); }//停止计时器并将计时器设置为没有
	}
	////////////////////////////////////////////////////////////////////////
	void Tetris::stop()
	{
		clear();
		InvalidateRect(hWnd, NULL, TRUE);
	}
	/////////////////////////////////////////////////////////////////////
	void Tetris::start()
	{
		if (state_total == 0)//只有处于待命状态才能开始
		{
			state_total = 1;
			timer = 1;//将定时器设置为1，表示当前设置了定时器
			SetTimer(hWnd, timer, double(1000) / (sqrt(double(level))), NULL);//按照当前游戏等级设置计时器间隔
		}

	}
	//////////////////////////////////////////////////////
	void Tetris::quit()
	{
		if (timer){ KillTimer(hWnd, timer); }//若在退出游戏时计时器还在，则杀死它
	}
	//////////////////////////////////////////////////////
	unsigned Tetris::get_state()
	{
		return state_total;
	}
	////////////////////////////////////////////////////
	void Tetris::get_client_size(unsigned x, unsigned y)
	{
		client_x = x;
		client_y = y;
		block_size = 0;//每次计算前将该值先归零
		for (;;++block_size)//计算当前客户区尺寸允许的方块尺寸的最大值
		{
			if (((16 + 19 * block_size) > client_x) || ((21 + 24 * block_size) > client_y))
			{
				if (block_size != 0){ --block_size; }//只要不是第一次循环就超出就可以自减
				break; 
			}

		}
	}
	///////////////////////////////////////////////////////
	void Tetris::display(HDC hdc)
	{
		int x(0), y(0);
		x = (client_x - (16 + 19 * block_size)) / 2 + 2 * block_size;//设置绘图的相对坐标点
		y = (client_y - (21 + 24 * block_size)) / 2 + 2 * block_size;//设置绘图的相对坐标点
		SelectObject(hdc,pen_frame1);
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, x + 10 + 10 * block_size, y);
		LineTo(hdc, x + 10 + 10 * block_size, y + 20 + 20 * block_size);
		LineTo(hdc, x, y + 20 + 20 * block_size);
		LineTo(hdc, x, y);
		MoveToEx(hdc, x + 11 * block_size + 11, y, NULL);
		LineTo(hdc, x + 15 * block_size + 15, y);
		LineTo(hdc, x + 15 * block_size + 15, y + 2 + 2 * block_size);
		LineTo(hdc, x + 11 * block_size + 11, y + 2 + 2 * block_size);
		LineTo(hdc, x + 11 * block_size + 11, y);
		SelectObject(hdc, pen_frame2);
		for (int i(1); i != 10; ++i)
		{
			MoveToEx(hdc, x + i*(1 + block_size), y + 1,NULL);
			LineTo(hdc, x + i*(1 + block_size), y + 20 + 20 * block_size);
		}
		for (int i(1); i != 20; ++i)
		{
			MoveToEx(hdc, x + 1, y + i*(1 + block_size), NULL);
			LineTo(hdc, x + 10 + 10 * block_size, y + i*(1 + block_size));
		}
		for (int i(1); i != 4; ++i)
		{
			MoveToEx(hdc, x + 11 + 11 * block_size + i*(1 + block_size), y + 1, NULL);
			LineTo(hdc, x + 11 + 11 * block_size + i*(1 + block_size), y + 2 + 2 * block_size);
		}
		MoveToEx(hdc, x + 12 + 11 * block_size, y + 1 + block_size, NULL);
		LineTo(hdc, x + 15 * block_size + 15, y + 1 + block_size);
		RECT rect;
		for (int i(0); i != 20; ++i)
		{
			for (int m(0); m != 10; ++m)
			{
				SetRect(&rect, x + 1 + m*(block_size + 1), y + 1 + i*(block_size + 1), x + (m + 1)*(block_size + 1), y + (i + 1)*(block_size + 1));//设置矩形
				switch (frame[i][m])//判断框架中每个小块的类型
				{
				case 1:FillRect(hdc, &rect, brush_block1); break;
				case 2:FillRect(hdc, &rect, brush_block2); break;
				case 3:FillRect(hdc, &rect, brush_block3); break;
				case 4:FillRect(hdc, &rect, brush_block4); break;
				case 5:FillRect(hdc, &rect, brush_block5); break;
				case 6:FillRect(hdc, &rect, brush_block6); break;
				case 7:FillRect(hdc, &rect, brush_block7); break;
				default:
					break; 
				}
			}
		}
		int X(0), Y(y);
		X = x + 11 * (1 + block_size);
		switch (type_next)//在小框架里画下个积木
		{
		case 1:
			for (int i(0); i != 4; ++i)
			{
				SetRect(&rect, X + 1 + i*(block_size + 1), Y + 1, X + (i + 1)*(block_size + 1), Y + block_size + 1);
				FillRect(hdc, &rect, brush_block1);
			}
			break;
		case 2:
			for (int i(0); i != 3; ++i)
			{
				SetRect(&rect, X + 1 + i*(block_size + 1), Y + 1, X + (i + 1)*(block_size + 1), Y + block_size + 1);
				FillRect(hdc, &rect, brush_block2);
			}
			SetRect(&rect, X + 2 + block_size, Y + 2 + block_size, X + 2 * block_size + 2, Y + 2 * block_size + 2);
			FillRect(hdc, &rect, brush_block2);
			break;
		case 3:
			for (int i(0); i != 3; ++i)
			{
				SetRect(&rect, X + 1 + i*(block_size + 1), Y + 1, X + (i + 1)*(block_size + 1), Y + block_size + 1);
				FillRect(hdc, &rect, brush_block3);
			}
			SetRect(&rect, X + 1, Y + 2 + block_size, X + block_size + 1, Y + 2 * block_size + 2);
			FillRect(hdc, &rect, brush_block3);
			break;
		case 4:
			for (int i(0); i != 3; ++i)
			{
				SetRect(&rect, X + 1 + i*(block_size + 1), Y + 1, X + (i + 1)*(block_size + 1), Y + block_size + 1);
				FillRect(hdc, &rect, brush_block4);
			}
			SetRect(&rect, X + 2 * block_size + 3, Y + 2 + block_size, X + 3 * block_size + 3, Y + 2 * block_size + 2);
			FillRect(hdc, &rect, brush_block4);
			break;
		case 5:
			for (int i(1); i != 3; ++i)
			{
				SetRect(&rect, X + 1 + i*(block_size + 1), Y + 1, X + (i + 1)*(block_size + 1), Y + block_size + 1);
				FillRect(hdc, &rect, brush_block5);
			}
			for (int i(0); i != 2; ++i)
			{
				SetRect(&rect, X + 1 + i*(block_size + 1), Y + 2 + block_size, X + (i + 1)*(block_size + 1), Y + 2 * block_size + 2);
				FillRect(hdc, &rect, brush_block5);
			}
			break;
		case 6:
			for (int i(0); i != 2; ++i)
			{
				SetRect(&rect, X + 1 + i*(block_size + 1), Y + 1, X + (i + 1)*(block_size + 1), Y + block_size + 1);
				FillRect(hdc, &rect, brush_block6);
			}
			for (int i(1); i != 3; ++i)
			{
				SetRect(&rect, X + 1 + i*(block_size + 1), Y + 2 + block_size, X + (i + 1)*(block_size + 1), Y + 2 * block_size + 2);
				FillRect(hdc, &rect, brush_block6);
			}
			break;
		case 7:
			for (int i(1); i != 3; ++i)
			{
				SetRect(&rect, X + 1 + i*(block_size + 1), Y + 1, X + (i + 1)*(block_size + 1), Y + block_size + 1);
				FillRect(hdc, &rect, brush_block7);
				SetRect(&rect, X + 1 + i*(block_size + 1), Y + 2 + block_size, X + (i + 1)*(block_size + 1), Y + 2 * block_size + 2);
				FillRect(hdc, &rect, brush_block7);
			}
			break;
		default:
			break;
		}
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));//设置字体
		SetBkColor(hdc,RGB(0,0,0));
		SetTextColor(hdc, RGB(255, 0, 0));//设置文本颜色
		cat::String s;
		s = TEXT("Rule  : TGM Rotation");
		TextOut(hdc, X, Y + 4 * (block_size + 1), s.c_str(), s.size());
		switch (state_total)
		{
		case 0:
			s = TEXT("State : Ready");
			break;
		case 1:
			s = TEXT("State : Running");
			break;
		case 2:
			s = TEXT("State : Pause");
			break;
		case 3:
			s = TEXT("State : Game Over");
			break;
		default:
			break;
		}
		TextOut(hdc, X, Y + 5 * (block_size + 1), s.c_str(), s.size());
		cat::Ostringstream oss1;
		oss1 << level;
		s = TEXT("Level : ");
		s += oss1.str();
		SetTextColor(hdc, RGB(0, 255, 0));//设置文本颜色
		TextOut(hdc, X, Y + 7 * (block_size + 1), s.c_str(), s.size());
		cat::Ostringstream oss2;
		s = TEXT("Speed : ");
		oss2 << sqrt(double(level));
		s = s + oss2.str() + TEXT(" /Sec");
		TextOut(hdc, X, Y + 8 * (block_size + 1), s.c_str(), s.size());
		s = TEXT("Lines : ");
		if (state_total != 0)
		{
			cat::Ostringstream oss;
			oss << total_number;
			s += oss.str();
		}
		TextOut(hdc, X, Y + 9 * (block_size + 1), s.c_str(), s.size());
		s = TEXT("Scroe : ");
		if (state_total != 0)
		{
			cat::Ostringstream oss;
			oss << score;
			s += oss.str();
		}
		TextOut(hdc, X, Y + 10 * (block_size + 1), s.c_str(), s.size());
		SetTextColor(hdc, RGB(0, 0, 255));//设置文本颜色
		s = TEXT("Enter : ");
		switch (state_total)
		{
		case 0:
			s += TEXT("Start");
			break;
		case 1:
			s += TEXT("Stop");
			break;
		case 2:
			break;
		case 3:
			s += TEXT("Continue");
			break;
		default:
			break;
		}
		TextOut(hdc, X, Y + 12 * (block_size + 1), s.c_str(), s.size());
		s = TEXT("A     : Level up");
		TextOut(hdc, X, Y + 13 * (block_size + 1), s.c_str(), s.size());
		s = TEXT("S     : Level down");
		TextOut(hdc, X, Y + 14 * (block_size + 1), s.c_str(), s.size());
		SetTextColor(hdc, RGB(0, 255, 255));//设置文本颜色
		s = TEXT("Esc   : Quit");
		TextOut(hdc, X, Y + 18 * (block_size + 1), s.c_str(), s.size());
	}
	//////////////////////////////////////////////////////////////////////////
	void Tetris::level_up()
	{
		if (state_total == 0 && level < 0xffffffff)
		{
			++level;
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
	/////////////////////////////////////////////////////////////////////////
	void Tetris::level_down()
	{
		if (state_total == 0 && level>1)
		{
			--level;
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Tetris::running(UINT message)
	{
		if (state_total != 2)
		{
			if (get_TGM_block())
			{
				insert_block();
				return;
			}
			if (down_shift()){ return; }//若下移成功则此次循环执行完毕直接退出
			find_lines();
		}
		else if (message == WM_TIMER)//只有消行延时计时器能触发消行处理
		{
			remove_lines();
		}
	}
}
