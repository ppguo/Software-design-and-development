#include <iostream>
#include "rational.h"
#include "simple_test.h"
using namespace std;

void CHECK_equal(const Rational& r1, int n, int d) {
	CHECK(r1.nominator() == n && r1.denomi() == d);
}
int test_main() {
	
	//检测异常是否正常抛出
	CHECK_THROW((Rational{ 5,0 }), Rational::div0);//构造时异常
	CHECK_THROW((Rational{ 2,1 } / Rational{ 0,5 }), Rational::div0);//相除的时候异常

	//检测比较关系式
	CHECK_EQUAL((Rational{ 2,1 } == Rational{ 2,1 }), true);//相同
	CHECK_EQUAL((Rational{ 2,1 } == Rational{ 2,5 }), false);//分母不同
	CHECK_EQUAL((Rational{ 4,5 } == Rational{ 2,5 }), false);//分子不同
	CHECK_EQUAL((Rational{ 2,4 } == Rational{ 1,2 }), true);//分子分母均不同，但约分后相同

	CHECK_EQUAL((Rational{ 4,5 } <= Rational{ 3,5 }), false);//不满足比较条件
	CHECK_EQUAL((Rational{ 4,10 } <= Rational{ 2,5 }), true);//满足比较条件（下同）

	CHECK_EQUAL((Rational{ 4,5 } >= Rational{ 3,5 }), true);
	CHECK_EQUAL((Rational{ 4,10 } >= Rational{ 3,5 }), false);

	CHECK_EQUAL((Rational{ 4,5 } > Rational{ 3,5 }), true);
	CHECK_EQUAL((Rational{ 4,10 } > Rational{ 2,5 }), false);

	CHECK_EQUAL((Rational{ 3,5 } < Rational{ 4,5 }), true);
	CHECK_EQUAL((Rational{ 4,10 } < Rational{ 2,5 }), false);

	CHECK_EQUAL((Rational{ 4,5 } != Rational{ 3,5 }), true);
	CHECK_EQUAL((Rational{ 4,10 } != Rational{ 2,5 }), false);

	//检测二元表达符
	//在相等比较符测试无误的基础上测试，掺杂部分使用函数对比，防止存在没有测试到==错误的情况
	//每类都选取了边界值测试
	CHECK_EQUAL((Rational{ 1,2 }+Rational{ 1,2 }), (Rational{ 1,1 }));
	CHECK_EQUAL((Rational{ 0,5 }*Rational{ 7,11 }), (Rational{ 0,1 }));
	CHECK_equal((Rational{ 11,5 }*Rational{ 7,11 }), 7, 5);
	CHECK_EQUAL((Rational{ 1,2 }-Rational{ 2,4 }), (Rational{ 0,1 }));
	CHECK_equal((Rational{ 1,2 }*Rational{ 2,1 }), 1, 1);
	CHECK_EQUAL((Rational{ 3,5 } / Rational{ 6,10 }), (Rational{ 1,1 }));
	CHECK_EQUAL((Rational{ 1,2 }+=Rational{ 1,2 }), (Rational{ 1,1 }));
	CHECK_EQUAL((Rational{ 0,5 }*=Rational{ 7,11 }), (Rational{ 0,1 }));
	CHECK_EQUAL((Rational{ 1,2 }-=Rational{ 2,4 }), (Rational{ 0,1 }));
	CHECK_EQUAL((Rational{ 3,5 } /= Rational{ 6,10 }), (Rational{ 1,1 }));

	//输出流的测试
	Rational a{ 2,1 };
	Rational b{ 0,1 };
	Rational c{ 5,4 };
	cout <<"2 = "<<a << "\n";
	cout <<"0 = "<<b <<"\n";
	cout <<"5/4 = "<<c<<"\n";
	return 0;
}