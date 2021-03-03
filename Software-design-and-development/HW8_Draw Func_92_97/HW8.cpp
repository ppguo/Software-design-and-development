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
		int x_count;//每条曲线画的x点个数
		Binary_function(double f(double, double), // 二元函数 
						Point orig, // 原点位置 
						double x1, double x2, // x范围 
						double y1, double y2, // y范围 
						int xcount = 100, // x取样点数 
						int ycount = 100, // y取样点数 
						double scale = 25, // 比例因子 
						double yslope = 45 // y轴倾斜角度 
			); 
		void draw_lines(Graphics& g) const //修改之前的连线方式，减去两曲线之间点的连接
		{
			if (color().visibility() && 1 < number_of_points())	
				for (unsigned int i = 1; i < number_of_points(); ++i) {
					if (i%x_count)//曲线内的点才连接
						g.line(point(i - 1), point(i));
				}					
		}
	};


	Binary_function::Binary_function(double f(double, double), // 二元函数 
									 Point orig, // 原点位置 
									 double x1, double x2, // x范围 
									 double y1, double y2, // y范围 
									 int xcount , // x取样点数 
									 int ycount , // y取样点数 
									 double scale , // 比例因子 
									 double yslope  // y轴倾斜角度 
										)
	{
		x_count = xcount;
		if (x2 - x1 <= 0 || y2 - y1 <= 0) throw bad_graphing_range{};
		if (xcount <= 0 || ycount <= 0) throw non_positive_graphing_count{};
		double xdist = (x2 - x1) / xcount; //x轴每点距离
		double ydist = (y2 - y1) / ycount; //y轴每点距离
		auto rad_yslope = yslope / 180 * pi; //倾斜角度弧度值

		auto y_real = y1; //y扫描开始点
		//此时y在图上对应的像素点
		auto y_now = Point{ int(orig.x + y_real * sin(rad_yslope) * scale),int(orig.y - y_real * cos(rad_yslope) * scale) };
		for (int i = 0; i < ycount; i++) {//固定y,产生z,x曲线
			auto r = x1;
			for (int j = 0; j < xcount; ++j) {
				auto p_tmp = Point{ int(y_now.x + r * scale),int(y_now.y + (f(0,y_real) - f(r,y_real)) * scale) };//对应图位置
				add(p_tmp);
				r += xdist;
			}
			y_real += ydist;
			//y_now为对应（0,y_real,f(0,y_real))
			y_now = Point{ int(orig.x + y_real * sin(rad_yslope) * scale),int(orig.y - (y_real * cos(rad_yslope) -f(0,y1)+f(0,y_real)) * scale) };
		}
	}
}
//各类二元函数
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