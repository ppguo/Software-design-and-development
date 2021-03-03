#include <iostream>
using namespace std;
class Rational {
public:
	struct div0 {};
	Rational(int nn = 0, int dd = 1);//若有理数为 - ， - 号始终处于分子
	//获得分子或分母
	int nominator() const{
		return n;
	}
	int denomi() const{
		return d;
	}
	//二元算符
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

	//比较符号
	friend bool operator<(const Rational& R1, const Rational& R2);
	friend bool operator>(const Rational& R1, const Rational& R2);
	friend bool operator>=(const Rational& R1, const Rational& R2);
	friend bool operator<=(const Rational& R1, const Rational& R2);
	friend bool operator==(const Rational& R1, const Rational& R2);
	friend bool operator!=(const Rational& R1, const Rational& R2);
	//输入输出
	friend std::istream& operator >>(std::istream& s, Rational& r);
	friend std::ostream& operator <<(std::ostream& s, Rational& r);
private:
	int n, d;
	int gcd(int m, int n)		/* 最大公约数 */
	{
		if (m > 0 && n < 0 || m < 0 && n>0)
			m = -m; // 使得m和n同号,且若有理数为-，-号式中处于分子
		while (true) {
			if (m == 0) return n;
			n %= m;
			if (n == 0) return m;
			m %= n;
		}
	}
};
Rational::Rational(int nn, int dd)//若有理数为 - ， - 号始终处于分子
	:d{ dd }, n{ nn }
{if (d == 0)
throw div0{};
int k = gcd(nn, dd);
d /= k;
n /= k;
}

//二元算符

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

//比较符号
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

//输入输出
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

	