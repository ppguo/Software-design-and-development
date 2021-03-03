#include <math.h>
#include "Graph.h"
#include "Simple_window.h"
//#include <utility>
#include "GUI.h"

#define pi 3.14159265

using std::pair;
namespace Graph_lib {
	struct bad_graphing_range{};
	struct non_positive_graphing_count{};
	struct Binary_function : Shape {
		int x_count;//ÿ�����߻���x�����
		Binary_function(double f(double, double), // ��Ԫ���� 
						Point orig, // ԭ��λ�� 
						double x1, double x2, // x��Χ 
						double y1, double y2, // y��Χ 
						int xcount = 100, // xȡ������ 
						int ycount = 100, // yȡ������ 
						double scale = 25, // �������� 
						double yslope = 45 // y����б�Ƕ� 
			); 
		void draw_lines(Graphics& g) const //�޸�֮ǰ�����߷�ʽ����ȥ������֮��������
		{
			if (color().visibility() && 1 < number_of_points())	
				for (unsigned int i = 1; i < number_of_points(); ++i) {
					if (i%x_count)//�����ڵĵ������
						g.line(point(i - 1), point(i));
				}					
		}
	};


	Binary_function::Binary_function(double f(double, double), // ��Ԫ���� 
									 Point orig, // ԭ��λ�� 
									 double x1, double x2, // x��Χ 
									 double y1, double y2, // y��Χ 
									 int xcount , // xȡ������ 
									 int ycount , // yȡ������ 
									 double scale , // �������� 
									 double yslope  // y����б�Ƕ� 
										)
	{
		x_count = xcount;
		if (x2 - x1 <= 0 || y2 - y1 <= 0) throw bad_graphing_range{};
		if (xcount <= 0 || ycount <= 0) throw non_positive_graphing_count{};
		double xdist = (x2 - x1) / xcount; //x��ÿ�����
		double ydist = (y2 - y1) / ycount; //y��ÿ�����
		auto rad_yslope = yslope / 180 * pi; //��б�ǶȻ���ֵ

		auto y_real = y1; //yɨ�迪ʼ��
		//��ʱy��ͼ�϶�Ӧ�����ص�
		auto y_now = Point{ int(orig.x + y_real * sin(rad_yslope) * scale),int(orig.y - y_real * cos(rad_yslope) * scale) };
		for (int i = 0; i < ycount; i++) {//�̶�y,����z,x����
			auto r = x1;
			for (int j = 0; j < xcount; ++j) {
				auto p_tmp = Point{ int(y_now.x + r * scale),int(y_now.y + (f(0,y_real) - f(r,y_real)) * scale) };//��Ӧͼλ��
				add(p_tmp);
				r += xdist;
			}
			y_real += ydist;
			//y_nowΪ��Ӧ��0,y_real,f(0,y_real))
			y_now = Point{ int(orig.x + y_real * sin(rad_yslope) * scale),int(orig.y - (y_real * cos(rad_yslope) -f(0,y1)+f(0,y_real)) * scale) };
		}
	}
}
//�����Ԫ����
double COS(double x,double y) { return cos(x*y); }
double SIN_2_Y(double x, double y) { return sin(2 * y); }
double SIN_EXP(double x, double y) {
	return (sin(x * x + y * y) * exp(-(x * x + y * y) / 10) * 2);
}

int main() {
	auto a = cos(3.14/2);
	const int xmax = 600;    // window size
	const int ymax = 400;

	const int x_orig = xmax / 2;    // position of (0,0) is center of window
	const int y_orig = ymax / 2;
	const Point orig = { x_orig,y_orig };

	const double x_min = -5;    // range [-10:11) 
	const double x_max = 5;
	const double y_min = -8;    // range [-10:11) 
	const double y_max = 8;
	const int n_points = 400;    // number of points used in range

	const double y_scale = 30;
	Simple_window win0 = { {100,100},xmax,ymax,"COSXY graphing" };
	Simple_window win1 = { {100,100},xmax,ymax,"SIN2Y graphing" };
	Simple_window win2 = { {100,100},xmax,ymax,"SIN_EXP graphing" };

	Binary_function s0 = { COS,orig,x_min,x_max,y_min,y_max };
	Binary_function s1 = { SIN_2_Y,orig,x_min,x_max,y_min,y_max };
	Binary_function s2 = { SIN_EXP,orig,x_min,x_max,y_min,y_max };

	win0.attach(s0);
	win1.attach(s1);
	win2.attach(s2);

	win0.wait_for_button();
	win1.wait_for_button();
	win2.wait_for_button();
}