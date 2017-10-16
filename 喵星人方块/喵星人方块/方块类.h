#ifndef 方块类_H
#define 方块类_H
/*
* 本人喜欢用标准库的东西，因为标准库的字符处理类针对宽字符（UNICODE）和单字节字符（ASCII）是区分开的，
* 为了头文件的可移植性，在这里将几个涉及到字符处理的标准库类进行宏定义。在编写客户代码时使用这里重新
* 定义过的类型，编译器在编译器时就能根据框架是否定义了UNICODE宏来自动选择使用标准库相关类型的宽字符版本。
* 至于常规字符数组和字符串常量的问题，则用框架自身提供的TCHAR类型和TEXT宏解决，这两个也是和UNICODE宏关联的。
*/
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
namespace cat//嗯，吧这些新类型定义在私有的名字空间中是个不错的选择，这样就完全杜绝了在使用时和其它库类冲突的可能性
{
#ifdef UNICODE//若有定义UNICODE，则编译器会采用宽字符版本的API，否则会采用单字节字符版本的API，我们的新类型也根据这个宏来选择对应的标准库类。
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
}//嗯，到这里为止，上面就是对标准库的类进行了宏定义
#include <Windows.h>
#include <vector>
#include <random>
namespace cat//这里编写方块类
{
	class Tetris{
	public:
		explicit Tetris(HWND);//构造函数
		Tetris(const Tetris&) = delete;
		Tetris& operator=(const Tetris&) = delete;
		~Tetris();//析构函数，用于释放自定义画笔等
		bool down_shift();//积木下落一格的处理
		bool left_shift();//积木左移一格的处理
		bool right_shift();//积木右移一格的处理
		void stop();//停止
		void start();//开始
		void get_client_size(unsigned,unsigned);//获取窗口客户区像素点为单位的尺寸
		void display(HDC);
		void level_up();//提高游戏等级，只有在就绪状态下有效
		void level_down();//降低游戏等级，只有在就绪状态下有效
		void quit();//游戏结束时进行必要的清理工作
		unsigned get_state();//返回游戏状态
		bool TGM_rotation();//积木顺时针旋转一次的处理
		void running(UINT);//游戏主循环
	private:
		bool get_TGM_block();//随机的获得一个积木
		void set_TGM_block();//按照积木的类型和积木的状态设置block和color_block
		bool insert_block();//将获得的当前积木插入到框架中
		void clear_block();//擦除积木在框架中对应的位置
		bool find_lines();//检查行寻找是否有满格可消除的行
		unsigned remove_lines();//执行消耗处理，并更改积分等数据成员
		void clear();//清空游戏数据
	private:
		std::vector<std::vector<unsigned>> frame;//这里定义一个二维无符号4字节整形vector容器,为毛这里不用普通二维数组呢，因为vector相比普通数组在消行处理上更方便，它更易于管理。
		int block[4][2];//代表各种形状的积木的数组，4代表组成积木的4个方块，2代表每个方块在框架中的位置
		unsigned state_total;//代表俄罗斯方块的状态，0代表就绪，1代表正在运行，2代表正在消行，3代表终止
		unsigned state_block;//代表积木的状态，0代表没有活跃积木，1代表当前有一个活动的积木，2代表当前积木被固定
		unsigned type_current, type_next;//代表当前活动积木和下个积木的类型
		unsigned direction_block;//代表当前正在活动积木的朝向1,2,3,4分别代表4种朝向（有的积木可能变换时不会发生改变，或者朝向形态少于4种，这不是问题）
		unsigned total_number, temp_number;//前面的代表一场游戏消除的总行数，后面的用于临时计数，当达到或者超过30行时它将对30进行求模运算，并让等级提高1。
		unsigned client_x, client_y, block_size;//这两个变量用于保存窗口客户区的像素点尺寸,第3个保存计算得来的小方块的尺寸,用于绘图函数
		unsigned score;//这就是积分啦
		unsigned level;//代表游戏等级，它决定积木每秒下移格数
		HWND hWnd;//游戏窗口句柄
		unsigned timer;//游戏循环计时器
		std::default_random_engine e;//C++11新增的随机数引擎
		std::uniform_int_distribution<unsigned> u;//C++11新增的随机数分布类型
		HPEN pen_frame1, pen_frame2;//画框架用的画笔
		HBRUSH brush_block1, brush_block2, brush_block3, brush_block4, brush_block5, brush_block6, brush_block7;//画方块用的画刷
	};
}
#endif