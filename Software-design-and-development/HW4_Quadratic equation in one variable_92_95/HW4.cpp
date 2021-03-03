#include <iostream>
#include "simple_test.h"
#include <math.h>
#define precision 1e-6 /*根据单精度浮点数的标准，两数相差1e-6则认为相同*/
using namespace std;

struct a_equal_0{};
struct no_real_solution{};
struct solution_equation {
	float small_root;
	float large_root;
};

solution_equation Solve_Equation(float a, float b, float c) {
	if (abs(a) < precision) throw a_equal_0{};
	float delta = b * b - 4 * a * c;
	if (delta < 0) throw no_real_solution{};
	float delta_float = sqrt(delta);
	solution_equation s;
	if (a > 0) {
		s.small_root = (-b - delta_float) / (2 * a);
		s.large_root = (-b + delta_float) / (2 * a);
	}
	else {
		s.small_root = (-b + delta_float) / (2 * a);
		s.large_root = (-b - delta_float) / (2 * a);
	}
	return s;
}
int test_main() {
	//因为本身函数结构比较简单，覆盖了语句覆盖标准，路径覆盖，分支覆盖，条件覆盖，条件覆盖等多个标准
	CHECK_THROW(Solve_Equation(0, 1, 1), a_equal_0);
	CHECK_THROW(Solve_Equation(1, 2, 1.0001), no_real_solution);
	CHECK(abs(Solve_Equation(4, 5, 1).small_root -( -1)) < precision && abs(Solve_Equation(4, 5, 1).large_root -(- 0.25)) < precision);
	CHECK(abs(Solve_Equation(-4, 5, -1).small_root - (0.25)) < precision && abs(Solve_Equation(-4, 5, -1).large_root - (1)) < precision)
	return 0;

}


