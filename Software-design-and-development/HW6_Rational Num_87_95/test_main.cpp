#include <iostream>
#include "rational.h"
#include "simple_test.h"
using namespace std;

void CHECK_equal(const Rational& r1, int n, int d) {
	CHECK(r1.nominator() == n && r1.denomi() == d);
}
int test_main() {
	
	//����쳣�Ƿ������׳�
	CHECK_THROW((Rational{ 5,0 }), Rational::div0);//����ʱ�쳣
	CHECK_THROW((Rational{ 2,1 } / Rational{ 0,5 }), Rational::div0);//�����ʱ���쳣

	//���ȽϹ�ϵʽ
	CHECK_EQUAL((Rational{ 2,1 } == Rational{ 2,1 }), true);//��ͬ
	CHECK_EQUAL((Rational{ 2,1 } == Rational{ 2,5 }), false);//��ĸ��ͬ
	CHECK_EQUAL((Rational{ 4,5 } == Rational{ 2,5 }), false);//���Ӳ�ͬ
	CHECK_EQUAL((Rational{ 2,4 } == Rational{ 1,2 }), true);//���ӷ�ĸ����ͬ����Լ�ֺ���ͬ

	CHECK_EQUAL((Rational{ 4,5 } <= Rational{ 3,5 }), false);//������Ƚ�����
	CHECK_EQUAL((Rational{ 4,10 } <= Rational{ 2,5 }), true);//����Ƚ���������ͬ��

	CHECK_EQUAL((Rational{ 4,5 } >= Rational{ 3,5 }), true);
	CHECK_EQUAL((Rational{ 4,10 } >= Rational{ 3,5 }), false);

	CHECK_EQUAL((Rational{ 4,5 } > Rational{ 3,5 }), true);
	CHECK_EQUAL((Rational{ 4,10 } > Rational{ 2,5 }), false);

	CHECK_EQUAL((Rational{ 3,5 } < Rational{ 4,5 }), true);
	CHECK_EQUAL((Rational{ 4,10 } < Rational{ 2,5 }), false);

	CHECK_EQUAL((Rational{ 4,5 } != Rational{ 3,5 }), true);
	CHECK_EQUAL((Rational{ 4,10 } != Rational{ 2,5 }), false);

	//����Ԫ����
	//����ȱȽϷ���������Ļ����ϲ��ԣ����Ӳ���ʹ�ú����Աȣ���ֹ����û�в��Ե�==��������
	//ÿ�඼ѡȡ�˱߽�ֵ����
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

	//������Ĳ���
	Rational a{ 2,1 };
	Rational b{ 0,1 };
	Rational c{ 5,4 };
	cout <<"2 = "<<a << "\n";
	cout <<"0 = "<<b <<"\n";
	cout <<"5/4 = "<<c<<"\n";
	return 0;
}