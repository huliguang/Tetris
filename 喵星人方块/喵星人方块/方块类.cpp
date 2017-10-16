#include "stdafx.h"
#include "������.h"
namespace cat{
	Tetris::Tetris(HWND hwnd) ://Ĭ�Ϲ��캯���������г�Ա���г�ʼ��
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
		u(1,7)//������ֲ����ͳ�ʼ��Ϊ1��7���ֱ����7�ֻ�ľ
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		e.seed(sys.wMilliseconds);//ʹ��ϵͳʱ��ĺ���λ��ʼ�����������
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
	Tetris::~Tetris()//�����������ͷ��Զ��廭�ʵ�
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
		if (state_total == 1 && state_block == 0 && type_next == 0){//����Ϸ��һ�ο�ʼ,��ʱ�¸���ľ������Ϊ0
			state_total = 1;//����Ϸ״̬����Ϊ���ڽ���
			type_current = u(e);//ʹ����������һ����ľ���Ͳ�������type_current��Ա��
			type_next = u(e);//���¸���ľ�����ͱ�����type_next��
			set_TGM_block();//���û�ľ����
			return true;
		}
		else if (state_total == 1 && state_block == 0)
		{
			type_current = type_next;//ʹ���¸���ľ������
			type_next = u(e);//�ٴλ���¸���ľ�����Ͳ�������type_next��
			set_TGM_block();//���û�ľ����
			return true;
		}
		return false;
	}
////////////////////////////////////////////////////////////////////////////////////////////////
	void Tetris::set_TGM_block()
	{
		switch (type_current)
		{
		case 1://����1����Ϊ����
			block[0][0] = 3; block[0][1] = 0; 
			block[1][0] = 4; block[1][1] = 0;
			block[2][0] = 5; block[2][1] = 0;
			block[3][0] = 6; block[3][1] = 0;
			break;
		case 2://����2����ΪƷ��
			block[0][0] = 3; block[0][1] = 0;
			block[1][0] = 4; block[1][1] = 0;
			block[2][0] = 5; block[2][1] = 0;
			block[3][0] = 4; block[3][1] = 1;
			break;
		case 3://����3����ΪƷ��
			block[0][0] = 3; block[0][1] = 1;
			block[1][0] = 3; block[1][1] = 0;
			block[2][0] = 4; block[2][1] = 0;
			block[3][0] = 5; block[3][1] = 0;
			break;
		case 4://����4����ΪƷ��
			block[0][0] = 3; block[0][1] = 0;
			block[1][0] = 4; block[1][1] = 0;
			block[2][0] = 5; block[2][1] = 0;
			block[3][0] = 5; block[3][1] = 1;
			break;
		case 5://����5����Ϊ��Z
			block[0][0] = 3; block[0][1] = 1;
			block[1][0] = 4; block[1][1] = 1;
			block[2][0] = 4; block[2][1] = 0;
			block[3][0] = 5; block[3][1] = 0;
			break;
		case 6://����6����ΪZ
			block[0][0] = 3; block[0][1] = 0;
			block[1][0] = 4; block[1][1] = 0;
			block[2][0] = 4; block[2][1] = 1;
			block[3][0] = 5; block[3][1] = 1;
			break;
		case 7://����7����Ϊ��
			block[0][0] = 4; block[0][1] = 1;
			block[1][0] = 4; block[1][1] = 0;
			block[2][0] = 5; block[2][1] = 0;
			block[3][0] = 5; block[3][1] = 1;
			break;
		default:break;
		}
		direction_block = 1;//����ǰ���ľ�ĳ�������Ϊ1��Ҳ������һ�γ��ֵĳ���
	}
/////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::insert_block()
	{
		if (state_total == 1 && state_block == 0)//�����ж���Ϸ�Ƿ����ڽ������Լ���ǰ�Ƿ����»�ȡ��δ����Ļ�ľ
		{ 
			for (int n(0); n != 4; ++n)
			{
				if (frame[block[n][1]][block[n][0]] != 0)
				{ 
					state_total = 3;
					KillTimer(hWnd, timer--);//ɱ����ʱ��
					InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
					return false;
				}//�жϻ�ľ��һ�γ��ֵ�λ���Ƿ��Ѿ��з����ˣ���������Ϸ״̬����Ϊʧ�ܲ�����
			}
			for (int n(0); n != 4; ++n)
			{
				frame[block[n][1]][block[n][0]] = type_current;//ȷ����λ�ò����ִ�в������
			}
			state_block = 1;//����ľ��״̬����Ϊ��ǰ���л�Ծ��ľ
			InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
			return true;
		}
		return false;
	}
//////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::down_shift()
	{
		if (state_total == 1 && state_block == 1)//�����ж���Ϸ�Ƿ����ڽ����в����Ƿ���һ����Ļ�ľ
		{
			for (int n(0); n != 4; ++n)//�ж��Ƿ�߱���������
			{
				if (block[n][1] >= 19){ state_block = 2; return false; }//�ж��Ƿ��Ѿ����ף������򷵻�����ʧ��
				bool b(0);
				for (int i(0); i != 4; ++i)
				{
					if (block[n][0] == block[i][0] && (block[n][1] + 1) == block[i][1])//�ж�����ĵ��Ƿ����ڵ�ǰ��ľ
					{
						b = true;
					}
				}
				if (frame[block[n][1] + 1][block[n][0]] != 0 && b == false){ state_block = 2; return false; }//�ж�����λ���Ƿ��Ѿ��з����ˣ������Ҳ����ڵ�ǰ��ľ�򷵻�����ʧ��
			}
			for (int n(0); n != 4; ++n)//ִ���������
			{
				frame[block[n][1]][block[n][0]] = 0;//����ľ�ĵ�ǰλ�����
			}
			for (int n(0); n != 4; ++n)//ִ���������
			{
				frame[++block[n][1]][block[n][0]] = type_current;//����ľ���������1������Ӧ��λ�õ���������Ϊ��ǰ��ľ������
			}
			InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½��棬��ʾ�ƶ�Ч��
			return true;//���Ƴɹ�����
		}
		return false;//����ʧ�ܷ���
	}
////////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::left_shift()
	{
		if (state_total == 1 && state_block == 1)//�����ж���Ϸ�Ƿ����ڽ����в����Ƿ���һ����Ļ�ľ
		{
			for (int n(0); n != 4; ++n)//�ж��Ƿ�߱���������
			{
				if (block[n][0] <= 0){ return false; }//�ж��Ƿ��Ѿ����ߣ������򷵻�����ʧ��(ע��*����һ��Ҫ���Ƿ��з���֮ǰ�ж�)
				bool b(0);
				for (int i(0); i != 4; ++i)
				{
					if ((block[n][0] - 1) == block[i][0] && block[n][1] == block[i][1])//�ж���ߵĵ��Ƿ����ڵ�ǰ��ľ
					{
						b = true;
					}
				}
				if (frame[block[n][1]][block[n][0] - 1] != 0 && b == false){ return false; }//�ж�����λ���Ƿ��Ѿ��з����ˣ������򷵻�����ʧ��
			}
			for (int n(0); n != 4; ++n)//ִ�����Ʋ���
			{
				frame[block[n][1]][block[n][0]] = 0;//����ľ�ĵ�ǰλ�����
			}
			for (int n(0); n != 4; ++n)//ִ�����Ʋ���
			{
				frame[block[n][1]][--block[n][0]] = type_current;//����ľ�ĺ������1������Ӧ��λ�õ���ɫ����Ϊ��ľ������
			}
			InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½��棬��ʾ�ƶ�Ч��
			return true;//���Ƴɹ�����
		}
		return false;//����ʧ�ܷ���
	}
//////////////////////////////////////////////////////////////////////////////////////////////
	bool Tetris::right_shift()
	{
		if (state_total == 1 && state_block == 1)//�����ж���Ϸ�Ƿ����ڽ����в����Ƿ���һ����Ļ�ľ
		{
			for (int n(0); n != 4; ++n)//�ж��Ƿ�߱���������
			{
				if (block[n][0] >= 9){ return false; }//�ж��Ƿ��Ѿ����ߣ������򷵻�����ʧ��(ע��*����һ��Ҫ���Ƿ��з���֮ǰ�ж�)
				bool b(0);
				for (int i(0); i != 4; ++i)
				{
					if ((block[n][0] + 1) == block[i][0] && block[n][1] == block[i][1])//�ж��ұߵĵ��Ƿ����ڵ�ǰ��ľ
					{
						b = true;
					}
				}
				if (frame[block[n][1]][block[n][0] + 1] != 0 && b == false){ return false; }//�ж�����λ���Ƿ��Ѿ��з����ˣ������򷵻�����ʧ��
			}
			for (int n(0); n != 4; ++n)//ִ�����Ʋ���
			{
				frame[block[n][1]][block[n][0]] = 0;//����ľ�ĵ�ǰλ�����
			}
			for (int n(0); n != 4; ++n)//ִ�����Ʋ���
			{
				frame[block[n][1]][++block[n][0]] = type_current;//����ľ�ĺ������1������Ӧ��λ�õ���ɫ����Ϊ��ľ������
			}
			InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½��棬��ʾ�ƶ�Ч��
			return true;//���Ƴɹ�����
		}
		return false;//����ʧ�ܷ���
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
		if (state_total == 1 && state_block == 1)//�����ж���Ϸ�Ƿ����ڽ����в����Ƿ���һ����Ļ�ľ
		{
			switch (type_current)
			{
			case 1:
				switch (direction_block)
				{
				case 1:
					if (block[0][1] <= 0 || block[0][1] >= 18){ return false; }//�жϳ���ת��ʱ�Ƿ��Խ��
					if (frame[block[2][1] - 1][block[2][0]] != 0 ||
						frame[block[2][1] + 1][block[2][0]] != 0 ||
						frame[block[2][1] + 2][block[2][0]] != 0){ return false; }//�жϳ���ת��ʱ�Ƿ���ص�
					clear_block();//������ǰλ��
					frame[--block[0][1]][block[0][0] += 2] = type_current;
					frame[block[1][1]][++block[1][0]] = type_current;
					frame[++block[2][1]][block[2][0]] = type_current;
					frame[block[3][1] += 2][--block[3][0]] = type_current;
					direction_block = 2;
					InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
					return true;
					break;
				case 2:
					if (block[0][0] >= 9 || block[0][0] <= 1){ return false; }//�жϳ���ת��ʱ�Ƿ��Խ��
					if (frame[block[1][1]][block[1][0] - 2] != 0 ||
						frame[block[1][1]][block[1][0] - 1] != 0 ||
						frame[block[1][1]][block[1][0] + 1] != 0){ return false; }//�жϳ���ת��ʱ�Ƿ���ص�
					clear_block();//������ǰλ��
					frame[++block[0][1]][block[0][0] -= 2] = type_current;
					frame[block[1][1]][--block[1][0]] = type_current;
					frame[--block[2][1]][block[2][0]] = type_current;
					frame[block[3][1] -= 2][++block[3][0]] = type_current;
					direction_block = 1;
					InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
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
					if (block[1][1] <= 0){ return false; }//�жϻ�����ת��Խ��
					if (frame[block[1][1] - 1][block[1][0]] != 0){ return false; }//�жϻ�����ת��ռλ
					clear_block();//������ǰλ��
					frame[--block[0][1]][++block[0][0]] = type_current;
					frame[block[1][1]][block[1][0]] = type_current;
					frame[++block[2][1]][--block[2][0]] = type_current;
					frame[--block[3][1]][--block[3][0]] = type_current;
					direction_block = 2;
					InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
					return true;
					break;
				case 2:
					if (block[0][0]<9 &&//�жϻ�����ת��Խ��
						frame[block[2][1]][block[2][0] - 1] == 0 &&//�жϻ�����ת��ռλ
						frame[block[2][1]][block[2][0] + 1] == 0)//�жϻ�����ת��ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] += 2][++block[0][0]] = type_current;
						frame[++block[1][1]][block[1][0]] = type_current;
						frame[block[2][1]][--block[2][0]] = type_current;
						frame[block[3][1]][++block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[0][0]<8 &&//�ж�������ת��Խ��
						frame[block[1][1]][block[1][0] + 1] == 0 &&//�ж�������ת��ռλ
						frame[block[2][1]][block[2][0] + 1] == 0 &&//�ж�������ת��ռλ
						frame[block[2][1]][block[2][0] + 2] == 0)//�ж�������ת��ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] += 2][block[0][0] += 2] = type_current;
						frame[++block[1][1]][++block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[block[3][1]][block[3][0] += 2] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[3][0]>0 &&//�ж�������ת��Խ��
						frame[block[2][1]][block[2][0] - 1] == 0 &&//�ж�������ת��ռλ
						frame[block[2][1]][block[2][0] - 2] == 0)//�ж�������ת��ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] += 2][block[0][0]] = type_current;
						frame[++block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1]][block[2][0] -= 2] = type_current;
						frame[block[3][1]][block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				case 3:
					if (block[3][1] <= 0 || frame[block[3][1] - 1][block[3][0]] != 0){ return false; }//�ж��Ƿ�߱���ת��������
					if (frame[block[0][1] - 1][block[0][0]] == 0)//�жϻ�����ת��ռλ
					{ 
						clear_block();//������ǰλ��
						frame[block[0][1]][--block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0]] = type_current;
						frame[block[2][1] -= 2][++block[2][0]] = type_current;
						frame[block[3][1]][++block[3][0]] = type_current;
						direction_block = 4;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (frame[block[3][1] - 1][block[3][0] - 1] == 0 &&//�ж�������ת��ռλ
						frame[block[3][1]][block[3][0] - 1] == 0)//�ж�������ת��ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1]][block[0][0] -= 2] = type_current;
						frame[--block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1] -= 2][block[2][0]] = type_current;
						frame[block[3][1]][block[3][0]] = type_current;
						direction_block = 4;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				case 4:
					if (block[0][0]>0 &&//�жϻ�����ת��Խ��
						frame[block[1][1]][block[1][0] - 1] == 0)//�жϻ�����ת��ռλ
					{
						clear_block();//������ǰλ��
						frame[--block[0][1]][--block[0][0]] = type_current;
						frame[block[1][1]][block[1][0]] = type_current;
						frame[++block[2][1]][++block[2][0]] = type_current;
						frame[++block[3][1]][--block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[3][0]<9 &&//�ж�������ת��Խ��
						frame[block[3][1]][block[3][0] + 1] == 0 &&//�ж�������ת��ռλ
						frame[block[3][1] + 1][block[3][0]] == 0)//�ж�������ת��ռλ
					{
						clear_block();//������ǰλ��
						frame[--block[0][1]][block[0][0]] = type_current;
						frame[block[1][1]][++block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0] += 2] = type_current;
						frame[++block[3][1]][block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
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
					if (block[1][1] <= 0 ||//�ж���ת�Ƿ�Խ��
						frame[block[0][1]][block[0][0] + 1] != 0 ||//�ж���ת�Ƿ�߱���������
						frame[block[2][1] - 1][block[2][0]] != 0)//�ж���ת�Ƿ�߱���������
					{
						return false;
					}
					if (frame[block[1][1] - 1][block[1][0]] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] -= 2][block[0][0]] = type_current;
						frame[--block[1][1]][++block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[++block[3][1]][--block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (frame[block[3][1] - 1][block[3][0]] == 0 &&//�ж����ƶ���ת�Ƿ�ռλ
						frame[block[3][1] + 1][block[3][0]] == 0)//�ж����ƶ���ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] -= 2][++block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0] += 2] = type_current;
						frame[block[2][1]][++block[2][0]] = type_current;
						frame[++block[3][1]][block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				case 2:
					if (block[1][0] < 9 &&//�жϻ�����ת�Ƿ�Խ��
						frame[block[3][1]][block[3][0] - 1] == 0 &&//�жϻ�����ת�Ƿ�ռλ
						frame[block[3][1]][block[3][0] + 1] == 0 &&//�жϻ�����ת�Ƿ�ռλ
						frame[block[2][1]][block[2][0] + 1] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[++block[0][1]][block[0][0] += 2] = type_current;
						frame[block[1][1] += 2][++block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0]] = type_current;
						frame[block[3][1]][--block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[1][0] < 8 &&//�ж�������ת�Ƿ�Խ��
						frame[block[3][1]][block[3][0] + 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[3][1]][block[3][0] + 2] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[2][1]][block[2][0] + 2] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[++block[0][1]][block[0][0] += 3] = type_current;
						frame[block[1][1] += 2][block[1][0] += 2] = type_current;
						frame[++block[2][1]][++block[2][0]] = type_current;
						frame[block[3][1]][block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[0][0] > 0 &&//�ж�������ת�Ƿ�Խ��
						frame[block[3][1]][block[3][0] - 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[3][1]][block[3][0] - 2] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[++block[0][1]][++block[0][0]] = type_current;
						frame[block[1][1] += 2][block[1][0]] = type_current;
						frame[++block[2][1]][--block[2][0]] = type_current;
						frame[block[3][1]][block[3][0] -= 2] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				case 3:
					if (block[0][1]>0 &&//�ж���ת�Ƿ�Խ��
						frame[block[2][1] - 1][block[2][0]] == 0 &&//�ж���ת�Ƿ�ռλ
						frame[block[2][1] - 2][block[2][0]] == 0)//�ж���ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[++block[0][1]][block[0][0]] = type_current;
						frame[block[1][1]][--block[1][0]] = type_current;
						frame[--block[2][1]][block[2][0]] = type_current;
						frame[block[3][1] -= 2][++block[3][0]] = type_current;
						direction_block = 4;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				case 4:
					if (block[1][0]>0 &&//�жϻ�����ת�Ƿ�Խ��
						frame[block[1][1]][block[1][0] - 1] == 0 &&//�жϻ�����ת�Ƿ�ռλ
						frame[block[2][1]][block[2][0] - 1] == 0 &&//�жϻ�����ת�Ƿ�ռλ
						frame[block[0][1] - 1][block[0][0]] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1]][block[0][0] -= 2] = type_current;
						frame[--block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[++block[3][1]][++block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[0][0]<9 &&//�ж�������ת�Ƿ�Խ��
						frame[block[2][1]][block[2][0] + 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[2][1]][block[2][0] + 2] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1]][--block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0]] = type_current;
						frame[block[2][1]][++block[2][0]] = type_current;
						frame[++block[3][1]][block[3][0] += 2] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[1][0]>1 &&//�ж�������ת�Ƿ�Խ��
						frame[block[2][1]][block[2][0] - 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[2][1]][block[2][0] - 2] == 0 &&
						frame[block[1][1]][block[1][0] - 2] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1]][block[0][0] -= 3] = type_current;
						frame[--block[1][1]][block[1][0] -= 2] = type_current;
						frame[block[2][1]][--block[2][0]] = type_current;
						frame[++block[3][1]][block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
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
					if (block[0][1] <= 0 ||//�ж���ת�Ƿ�Խ��
						frame[block[1][1] - 1][block[1][0]] != 0 ||//�ж���ת�Ƿ�߱�����
						frame[block[1][1] + 1][block[1][0]] != 0)//�ж���ת�Ƿ�߱�����
					{
						return false;
					}
					if (frame[block[0][1] + 1][block[0][0]] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[--block[0][1]][++block[0][0]] = type_current;
						frame[block[1][1]][block[1][0]] = type_current;
						frame[++block[2][1]][--block[2][0]] = type_current;
						frame[block[3][1]][block[3][0] -= 2] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (frame[block[2][1] - 1][block[2][0]] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[--block[0][1]][block[0][0] += 2] = type_current;
						frame[block[1][1]][++block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0]] = type_current;
						frame[block[3][1]][--block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				case 2:
					if (block[0][0]<9 &&//�жϻ�����ת�Ƿ�Խ��
						frame[block[3][1] - 1][block[3][0]] == 0 &&//�жϻ�����ת�Ƿ�ռλ
						frame[block[2][1]][block[2][0] + 1] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] += 2][++block[0][0]] = type_current;
						frame[++block[1][1]][block[1][0]] = type_current;
						frame[block[2][1]][--block[2][0]] = type_current;
						frame[--block[3][1]][block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[0][0]<8 &&//�ж�������ת�Ƿ�Խ��
						frame[block[2][1]][block[2][0] + 2] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[2][1]][block[2][0] + 1] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] += 2][block[0][0] += 2] = type_current;
						frame[++block[1][1]][++block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[--block[3][1]][++block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[3][0]>0 &&//�ж�������ת�Ƿ�Խ��
						frame[block[1][1]][block[1][0] - 2] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[3][1]][block[3][0] - 1] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] += 2][block[0][0]] = type_current;
						frame[++block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1]][block[2][0] -= 2] = type_current;
						frame[--block[3][1]][--block[3][0]] = type_current;
						direction_block = 3;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				case 3:
					if (block[3][1]<=0 ||//�ж���ת�Ƿ�Խ��
						frame[block[1][1] - 1][block[1][0]] != 0 ||//�ж���ת�Ƿ�ռλ
						frame[block[1][1] - 2][block[1][0]] != 0)//�ж���ת�Ƿ�ռλ
					{
						return false;
					}
					if (frame[block[0][1] - 2][block[0][0]] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1]][--block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0]] = type_current;
						frame[block[2][1] -= 2][++block[2][0]] = type_current;
						frame[--block[3][1]][block[3][0] += 2] = type_current;
						direction_block = 4;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (frame[block[3][1] - 1][block[3][0]] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1]][block[0][0] -= 2] = type_current;
						frame[--block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1] -= 2][block[2][0]] = type_current;
						frame[--block[3][1]][++block[3][0]] = type_current;
						direction_block = 4;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				case 4:
					if (block[0][0]>0 &&//�жϻ�����ת�Ƿ�Խ��
						frame[block[1][1]][block[1][0] - 1] == 0 &&//�жϻ�����ת�Ƿ�ռλ
						frame[block[1][1]][block[1][0] + 1] == 0 && //�жϻ�����ת�Ƿ�ռλ
						frame[block[0][1]][block[0][0] + 1] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[--block[0][1]][--block[0][0]] = type_current;
						frame[block[1][1]][block[1][0]] = type_current;
						frame[++block[2][1]][++block[2][0]] = type_current;
						frame[block[3][1] += 2][block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[3][0]<9 &&//�ж�������ת�Ƿ�Խ��
						frame[block[1][1]][block[1][0] + 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[1][1]][block[1][0] + 2] == 0 && //�ж�������ת�Ƿ�ռλ
						frame[block[0][1]][block[0][0] + 2] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[--block[0][1]][block[0][0]] = type_current;
						frame[block[1][1]][++block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0] += 2] = type_current;
						frame[block[3][1] += 2][++block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[0][0]>1 &&//�ж�������ת�Ƿ�Խ��
						frame[block[1][1]][block[1][0] - 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[1][1]][block[1][0] - 2] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[--block[0][1]][block[0][0] -= 2] = type_current;
						frame[block[1][1]][--block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0]] = type_current;
						frame[block[3][1] += 2][--block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
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
					if (block[2][1] <= 0){ return false; }//�ж���ת�Ƿ�Խ��
					if (frame[block[0][1] - 1][block[0][0]] == 0 &&//�жϻ�����ת�Ƿ�ռλ
						frame[block[0][1] - 2][block[0][0]] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] -= 2][block[0][0]] = type_current;
						frame[--block[1][1]][--block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[++block[3][1]][--block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (frame[block[2][1] - 1][block[2][0]] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[3][1] + 1][block[3][0]] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] -= 2][++block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0]] = type_current;
						frame[block[2][1]][++block[2][0]] = type_current;
						frame[++block[3][1]][block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[0][0]>0 &&//�ж�������ת�Ƿ�Խ��
						frame[block[0][1] - 2][block[0][0] - 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[0][1] - 1][block[0][0] - 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[0][1] - 1][block[0][0]] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] -= 2][--block[0][0]] = type_current;
						frame[--block[1][1]][block[1][0] -= 2] = type_current;
						frame[block[2][1]][--block[2][0]] = type_current;
						frame[++block[3][1]][block[3][0] -= 2] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				case 2:
					if (block[2][0]<9 &&//�жϻ�����ת�Ƿ�Խ��
						frame[block[2][1]][block[2][0] + 1] == 0 &&//�жϻ�����ת�Ƿ�ռλ
						frame[block[1][1] + 1][block[1][0]] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] += 2][block[0][0]] = type_current;
						frame[++block[1][1]][++block[1][0]] = type_current;
						frame[block[2][1]][block[2][0]] = type_current;
						frame[--block[3][1]][++block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[2][0]<8 &&//�ж�������ת�Ƿ�Խ��
						frame[block[2][1]][block[2][0] + 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[2][1]][block[2][0] + 2] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[3][1]][block[3][0] + 1] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] += 2][++block[0][0]] = type_current;
						frame[++block[1][1]][block[1][0] += 2] = type_current;
						frame[block[2][1]][++block[2][0]] = type_current;
						frame[--block[3][1]][block[3][0] += 2] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[0][0]>0 &&//�ж�������ת�Ƿ�Խ��
						frame[block[3][1]][block[3][0] - 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[3][1]][block[3][0] - 2] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[block[0][1] += 2][--block[0][0]] = type_current;
						frame[++block[1][1]][block[1][0]] = type_current;
						frame[block[2][1]][--block[2][0]] = type_current;
						frame[--block[3][1]][block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
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
					if (block[0][1] <= 0){ return false; }//�ж���ת�Ƿ�Խ��
					if (frame[block[3][1] - 1][block[3][0]] == 0 &&//�жϻ�����ת�Ƿ�ռλ
						frame[block[3][1] - 2][block[3][0]] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[--block[0][1]][block[0][0] += 2] = type_current;
						frame[block[1][1]][++block[1][0]] = type_current;
						frame[--block[2][1]][block[2][0]] = type_current;
						frame[block[3][1]][--block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[3][0]<9 &&//�ж�������ת�Ƿ�Խ��
						frame[block[3][1] - 1][block[3][0]] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[3][1] - 1][block[3][0] + 1] == 0 &&
						frame[block[3][1] - 2][block[3][0] + 1] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[--block[0][1]][block[0][0] += 3] = type_current;
						frame[block[1][1]][block[1][0] += 2] = type_current;
						frame[--block[2][1]][++block[2][0]] = type_current;
						frame[block[3][1]][block[3][0]] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (frame[block[1][1] - 1][block[1][0]] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[0][1] + 1][block[0][0]] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[--block[0][1]][++block[0][0]] = type_current;
						frame[block[1][1]][block[1][0]] = type_current;
						frame[--block[2][1]][--block[2][0]] = type_current;
						frame[block[3][1]][block[3][0] -= 2] = type_current;
						direction_block = 2;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				case 2:
					if (block[2][0] > 0 &&//�жϻ�����ת�Ƿ�Խ��
						frame[block[2][1]][block[2][0] - 1] == 0 &&//�жϻ�����ת�Ƿ�ռλ
						frame[block[1][1] + 1][block[1][0]] == 0)//�жϻ�����ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[++block[0][1]][block[0][0] -= 2] = type_current;
						frame[block[1][1]][--block[1][0]] = type_current;
						frame[++block[2][1]][block[2][0]] = type_current;
						frame[block[3][1]][++block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[0][0] < 9 &&//�ж�������ת�Ƿ�Խ��
						frame[block[3][1]][block[3][0] + 2] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[1][1] + 1][block[1][0]] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[++block[0][1]][--block[0][0]] = type_current;
						frame[block[1][1]][block[1][0]] = type_current;
						frame[++block[2][1]][++block[2][0]] = type_current;
						frame[block[3][1]][block[3][0] += 2] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					if (block[2][0] > 1 &&//�ж�������ת�Ƿ�Խ��
						frame[block[3][1]][block[3][0] - 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[2][1]][block[2][0] - 1] == 0 &&//�ж�������ת�Ƿ�ռλ
						frame[block[2][1]][block[2][0] - 2] == 0)//�ж�������ת�Ƿ�ռλ
					{
						clear_block();//������ǰλ��
						frame[++block[0][1]][block[0][0] -= 3] = type_current;
						frame[block[1][1]][block[1][0] -= 2] = type_current;
						frame[++block[2][1]][--block[2][0]] = type_current;
						frame[block[3][1]][block[3][0]] = type_current;
						direction_block = 1;
						InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½���
						return true;
					}
					break;
				default:
					break;
				}
				break;
			case 7:
				return true;//���ֻ�ľ���ǿ���ת����ֱ�ӷ���
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
		if (state_total == 1 && state_block == 2)//ֻ��������Ϸ�����ɻ�ľ���������̶�ʱ�ż��
		{
			int temp(0);
			for (auto &n : frame)
			{
				if (std::find(n.cbegin(),n.cend(),0)==n.cend())//����������
				{
					++temp;
					for (auto &m : n){ m = 8; }//�����еķ�����������Ϊ8����������Ѿ��з��鲢������
				}
			}
			if (temp){ 
				if (timer){ KillTimer(hWnd, timer--); }//ɱ��ѭ����ʱ��
				InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½��棬��ʾ����Ч��
				SetTimer(hWnd, ++timer, 500, NULL);
				state_total = 2;
				return true; 
			}//����true��ʾ���п�������
		}
		state_block = 0;//��û����Ҫ��������
		if (get_TGM_block())//�����������µĻ�ľ��������ɲ�ϧҪ��ͣ��
		{
			insert_block();
		}
		return false;//���򷵻�û��
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	unsigned Tetris::remove_lines()
	{
		if (state_total == 2 && state_block == 2)//ֻ��������Ϸ�����ɻ�ľ���������̶�ʱ�ſ�������
		{
			unsigned temp(0);
			for (std::vector<std::vector<unsigned>>::iterator ite = frame.begin(); ite != frame.end();)
			{
				if ((*ite)[0] == 8)//�жϸ����Ƿ񱻱����
				{
					++temp; 
					ite = frame.erase(ite);//��������
					continue;
				}
				++ite;
			}
			std::reverse(frame.begin(), frame.end());//�ţ�û�滮�õ������ڲ��뵰ʹ
			for (unsigned i(0); i != temp; ++i)//�ţ�û�滮�õ������ڲ��뵰ʹ
			{
				frame.push_back(std::vector<unsigned>(10, 0));
			}
			std::reverse(frame.begin(), frame.end());//�ţ�û�滮�õ������ڲ��뵰ʹ
			if (temp)//����������
			{ 
				score += (1 << (temp - 1)) * 100;//���»���
				total_number += temp;//�����������ۼ�����
				temp_number += temp;//���µ�ǰ�ȼ�������������
				if (level<0xFFFFFFFF){ level += (temp_number / 30); }//���ﵽ�򳬹�30���������Ϸ�ȼ�
				temp_number %= 30;//����ÿ�ּ���
				state_block = 0;//����ľ״̬����Ϊû�л�Ծ�Ļ�ľ���Ա�get_TGM_block()�����������ȷ�Ĵ����ľ�Ļ�ȡ
				state_total = 1;//������Ϻ�ָ���Ϸ״̬
				if (get_TGM_block())//�����к�Ӧ����������µĻ�ľ��������Ϸ�У������ճɲ���Ҫ��ͣ��
				{
					insert_block();
				}
				InvalidateRect(hWnd, NULL, TRUE);//����ˢ�½��棬��ʾ����Ч��
				KillTimer(hWnd, timer--);//ɱ�����ж�ʱ��
				SetTimer(hWnd, ++timer, double(1000) / (sqrt(double(level))), NULL);//���յ�ǰ��Ϸ�ȼ����ü�ʱ�����
				return temp;//���ش˴�����������
			}
		}
		return 0;//���������򷵻�0����ʾ������0��
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
		state_total = 0;//����Ϸ�ָ�������״̬
		state_block = 0;//���������ǰ��Ϸ����
		type_current = 0;
		type_next = 0;
		direction_block = 0;
		total_number = 0;
		temp_number = 0;
		score = 0;
		level = 1;
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		e.seed(sys.wMilliseconds);//ʹ��ϵͳʱ��ĺ���λ��ʼ�����������
		if (timer){ KillTimer(hWnd, timer--); }//ֹͣ��ʱ��������ʱ������Ϊû��
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
		if (state_total == 0)//ֻ�д��ڴ���״̬���ܿ�ʼ
		{
			state_total = 1;
			timer = 1;//����ʱ������Ϊ1����ʾ��ǰ�����˶�ʱ��
			SetTimer(hWnd, timer, double(1000) / (sqrt(double(level))), NULL);//���յ�ǰ��Ϸ�ȼ����ü�ʱ�����
		}

	}
	//////////////////////////////////////////////////////
	void Tetris::quit()
	{
		if (timer){ KillTimer(hWnd, timer); }//�����˳���Ϸʱ��ʱ�����ڣ���ɱ����
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
		block_size = 0;//ÿ�μ���ǰ����ֵ�ȹ���
		for (;;++block_size)//���㵱ǰ�ͻ����ߴ�����ķ���ߴ�����ֵ
		{
			if (((16 + 19 * block_size) > client_x) || ((21 + 24 * block_size) > client_y))
			{
				if (block_size != 0){ --block_size; }//ֻҪ���ǵ�һ��ѭ���ͳ����Ϳ����Լ�
				break; 
			}

		}
	}
	///////////////////////////////////////////////////////
	void Tetris::display(HDC hdc)
	{
		int x(0), y(0);
		x = (client_x - (16 + 19 * block_size)) / 2 + 2 * block_size;//���û�ͼ����������
		y = (client_y - (21 + 24 * block_size)) / 2 + 2 * block_size;//���û�ͼ����������
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
				SetRect(&rect, x + 1 + m*(block_size + 1), y + 1 + i*(block_size + 1), x + (m + 1)*(block_size + 1), y + (i + 1)*(block_size + 1));//���þ���
				switch (frame[i][m])//�жϿ����ÿ��С�������
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
		switch (type_next)//��С����ﻭ�¸���ľ
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
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));//��������
		SetBkColor(hdc,RGB(0,0,0));
		SetTextColor(hdc, RGB(255, 0, 0));//�����ı���ɫ
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
		SetTextColor(hdc, RGB(0, 255, 0));//�����ı���ɫ
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
		SetTextColor(hdc, RGB(0, 0, 255));//�����ı���ɫ
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
		SetTextColor(hdc, RGB(0, 255, 255));//�����ı���ɫ
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
			if (down_shift()){ return; }//�����Ƴɹ���˴�ѭ��ִ�����ֱ���˳�
			find_lines();
		}
		else if (message == WM_TIMER)//ֻ��������ʱ��ʱ���ܴ������д���
		{
			remove_lines();
		}
	}
}
