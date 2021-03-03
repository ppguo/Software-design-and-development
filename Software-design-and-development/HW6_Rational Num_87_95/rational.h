#include <iostream>
using namespace std;
class Rational {
public:
	struct div0 {};
	Rational(int nn = 0, int dd = 1);//��������Ϊ - �� - ��ʼ�մ��ڷ���
	//��÷��ӻ��ĸ
	int nominator() const{
		return n;
	}
	int denomi() const{
		return d;
	}
	//��Ԫ���
	Rational& operator +=(const Rational& r1) {
		this->n = this->n * r1.d + this->d * r1.n;
		this->d = this->d * r1.d;
		int k = gcd(this->d, this->n);
		this->n /= k;
		this->d /= k;
		return *this;
	}
	friend Rational operator+(Rational lhs, const Rational& rhs);
	Rational& operator -=(const Rational& r1) {
		this->n = this->n * r1.d - this->d * r1.n;
		this->d = this->d * r1.d;
		int k = gcd(this->n, this->d);
		this->n /= k;
		this->d /= k;
		return *this;
	}
	friend Rational operator-(Rational lhs, const Rational& rhs);
	Rational& operator *=(const Rational& r1) {
		this->n = this->n * r1.n;
		this->d = this->d * r1.d;
		int k = gcd(this->n, this->d);
		this->n /= k;
		this->d /= k;
		return *this;
	}
	friend Rational operator*(Rational lhs, const Rational& rhs);
	Rational& operator /=(const Rational& r1) {
		if (r1.n == 0)
			throw div0{};
		this->n = this->n * r1.d;
		this->d = this->d * r1.n;
		int k = gcd(this->n, this->d);
		this->n /= k;
		this->d /= k;
		return *this;
	}
	friend Rational operator/(Rational lhs, const Rational& rhs);

	//�ȽϷ���
	friend bool operator<(const Rational& R1, const Rational& R2);
	friend bool operator>(const Rational& R1, const Rational& R2);
	friend bool operator>=(const Rational& R1, const Rational& R2);
	friend bool operator<=(const Rational& R1, const Rational& R2);
	friend bool operator==(const Rational& R1, const Rational& R2);
	friend bool operator!=(const Rational& R1, const Rational& R2);
	//�������
	friend std::istream& operator >>(std::istream& s, Rational& r);
	friend std::ostream& operator <<(std::ostream& s, Rational& r);
private:
	int n, d;
	int gcd(int m, int n)		/* ���Լ�� */
	{
		if (m > 0 && n < 0 || m < 0 && n>0)
			m = -m; // ʹ��m��nͬ��,����������Ϊ-��-��ʽ�д��ڷ���
		while (true) {
			if (m == 0) return n;
			n %= m;
			if (n == 0) return m;
			m %= n;
		}
	}
};
Rational::Rational(int nn, int dd)//��������Ϊ - �� - ��ʼ�մ��ڷ���
	:d{ dd }, n{ nn }
{if (d == 0)
throw div0{};
int k = gcd(nn, dd);
d /= k;
n /= k;
}

//��Ԫ���

Rational operator+(Rational lhs, const Rational& rhs) {
	lhs += rhs;
	return lhs;
}

Rational operator-(Rational lhs, const Rational& rhs) {
	lhs -= rhs;
	return lhs;
}

Rational operator*(Rational lhs, const Rational& rhs) {
	lhs *= rhs;
	return lhs;
}

Rational operator/(Rational lhs, const Rational& rhs) {
	lhs /= rhs;
	return lhs;
}

//�ȽϷ���
bool operator<(const Rational& R1, const Rational& R2) {
	if ((R1 - R2).n < 0)
		return true;
	else
		return false;

}

bool operator>(const Rational& R1, const Rational& R2) {
	return R2 < R1;
}

bool operator>=(const Rational& R1, const Rational& R2) {
	return !(R1 < R2);
}

bool operator<=(const Rational& R1, const Rational& R2) {
	return !(R2 < R1);
}

bool operator==(const Rational& R1, const Rational& R2) {
	return !((R1 - R2).n);
}

bool operator!=(const Rational& R1, const Rational& R2) {
	return !(R1 == R2);
}

//�������
std::istream& operator >>(std::istream& s, Rational& r) {
	int n, d; char c;
	s >> n >> c >> d;
	r = Rational(n, d);
	return s;
}

std::ostream& operator <<(std::ostream& s, Rational& r) {
	char c = '/';
	if (r.d == 1)
		s << r.n;
	else
		s << r.n << c << r.d;
	return s;
}

	