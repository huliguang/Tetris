#ifndef ������_H
#define ������_H
/*
* ����ϲ���ñ�׼��Ķ�������Ϊ��׼����ַ���������Կ��ַ���UNICODE���͵��ֽ��ַ���ASCII�������ֿ��ģ�
* Ϊ��ͷ�ļ��Ŀ���ֲ�ԣ������ｫ�����漰���ַ�����ı�׼������к궨�塣�ڱ�д�ͻ�����ʱʹ����������
* ����������ͣ��������ڱ�����ʱ���ܸ��ݿ���Ƿ�����UNICODE�����Զ�ѡ��ʹ�ñ�׼��������͵Ŀ��ַ��汾��
* ���ڳ����ַ�������ַ������������⣬���ÿ�������ṩ��TCHAR���ͺ�TEXT������������Ҳ�Ǻ�UNICODE������ġ�
*/
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
namespace cat//�ţ�����Щ�����Ͷ�����˽�е����ֿռ����Ǹ������ѡ����������ȫ�ž�����ʹ��ʱ�����������ͻ�Ŀ�����
{
#ifdef UNICODE//���ж���UNICODE�������������ÿ��ַ��汾��API���������õ��ֽ��ַ��汾��API�����ǵ�������Ҳ�����������ѡ���Ӧ�ı�׼���ࡣ
	typedef std::wistream Istream;
	typedef std::wostream Ostream;
	typedef std::wstring String;
	typedef std::wstringstream Stringstream;
	typedef std::wistringstream Istringstream;
	typedef std::wostringstream Ostringstream;
	typedef std::wfstream Fstream;
	typedef std::wifstream Ifstream;
	typedef std::wofstream Ofstream;
#else
	typedef std::istream Istream;
	typedef std::ostream Ostream;
	typedef std::string String;
	typedef std::stringstream Stringstream;
	typedef std::istringstream Istringstream;
	typedef std::ostringstream Ostringstream;
	typedef std::fstream Fstream;
	typedef std::ifstream Ifstream;
	typedef std::ofstream Ofstream;
#endif
}//�ţ�������Ϊֹ��������ǶԱ�׼���������˺궨��
#include <Windows.h>
#include <vector>
#include <random>
namespace cat//�����д������
{
	class Tetris{
	public:
		explicit Tetris(HWND);//���캯��
		Tetris(const Tetris&) = delete;
		Tetris& operator=(const Tetris&) = delete;
		~Tetris();//���������������ͷ��Զ��廭�ʵ�
		bool down_shift();//��ľ����һ��Ĵ���
		bool left_shift();//��ľ����һ��Ĵ���
		bool right_shift();//��ľ����һ��Ĵ���
		void stop();//ֹͣ
		void start();//��ʼ
		void get_client_size(unsigned,unsigned);//��ȡ���ڿͻ������ص�Ϊ��λ�ĳߴ�
		void display(HDC);
		void level_up();//�����Ϸ�ȼ���ֻ���ھ���״̬����Ч
		void level_down();//������Ϸ�ȼ���ֻ���ھ���״̬����Ч
		void quit();//��Ϸ����ʱ���б�Ҫ��������
		unsigned get_state();//������Ϸ״̬
		bool TGM_rotation();//��ľ˳ʱ����תһ�εĴ���
		void running(UINT);//��Ϸ��ѭ��
	private:
		bool get_TGM_block();//����Ļ��һ����ľ
		void set_TGM_block();//���ջ�ľ�����ͺͻ�ľ��״̬����block��color_block
		bool insert_block();//����õĵ�ǰ��ľ���뵽�����
		void clear_block();//������ľ�ڿ���ж�Ӧ��λ��
		bool find_lines();//�����Ѱ���Ƿ����������������
		unsigned remove_lines();//ִ�����Ĵ��������Ļ��ֵ����ݳ�Ա
		void clear();//�����Ϸ����
	private:
		std::vector<std::vector<unsigned>> frame;//���ﶨ��һ����ά�޷���4�ֽ�����vector����,Ϊë���ﲻ����ͨ��ά�����أ���Ϊvector�����ͨ���������д����ϸ����㣬�������ڹ���
		int block[4][2];//���������״�Ļ�ľ�����飬4������ɻ�ľ��4�����飬2����ÿ�������ڿ���е�λ��
		unsigned state_total;//�������˹�����״̬��0���������1�����������У�2�����������У�3������ֹ
		unsigned state_block;//�����ľ��״̬��0����û�л�Ծ��ľ��1����ǰ��һ����Ļ�ľ��2����ǰ��ľ���̶�
		unsigned type_current, type_next;//����ǰ���ľ���¸���ľ������
		unsigned direction_block;//����ǰ���ڻ��ľ�ĳ���1,2,3,4�ֱ����4�ֳ����еĻ�ľ���ܱ任ʱ���ᷢ���ı䣬���߳�����̬����4�֣��ⲻ�����⣩
		unsigned total_number, temp_number;//ǰ��Ĵ���һ����Ϸ�������������������������ʱ���������ﵽ���߳���30��ʱ������30������ģ���㣬���õȼ����1��
		unsigned client_x, client_y, block_size;//�������������ڱ��洰�ڿͻ��������ص�ߴ�,��3��������������С����ĳߴ�,���ڻ�ͼ����
		unsigned score;//����ǻ�����
		unsigned level;//������Ϸ�ȼ�����������ľÿ�����Ƹ���
		HWND hWnd;//��Ϸ���ھ��
		unsigned timer;//��Ϸѭ����ʱ��
		std::default_random_engine e;//C++11���������������
		std::uniform_int_distribution<unsigned> u;//C++11������������ֲ�����
		HPEN pen_frame1, pen_frame2;//������õĻ���
		HBRUSH brush_block1, brush_block2, brush_block3, brush_block4, brush_block5, brush_block6, brush_block7;//�������õĻ�ˢ
	};
}
#endif