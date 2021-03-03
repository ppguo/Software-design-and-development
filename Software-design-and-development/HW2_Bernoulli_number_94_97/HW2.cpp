#include <iostream>
#include <vector>
using namespace std;

typedef struct rational_t {
	int n;		/* ���� */
	int d;		/* ��ĸ */
} rational;		/* ������ */


//�õ����Լ��
int gcd(int i, int j) {
	while (i != j) {
		if (i > j) i -= j;
		if (i < j) j -= i;
	}
	return i;
}

//�򻯷���
rational Simplify(rational s) {
	if (s.n == 0) {
		s.d = 1;
		return s;
	}
	int k = 1;
	if (s.n < 0) {
		s.n = -s.n;
		k = -k;
	}
	if (s.d < 0) {
		s.d = -s.d;
		k = -k;
	}
	int c;
	c = gcd(s.n, s.d);
	s.n /= c;
	s.d /= c;
	s.n *= k;
	return s;
}

//������������ӵĺ�
rational temp_plus(rational s, rational c) {
	rational k;
	k.n = s.n * c.d + c.n * s.d;
	k.d = s.d * c.d;
	return Simplify(k);
}

int main() {
	int m,i,j;
	rational s,c;
	cin >> m;
	vector<vector<int> > get_C(m+2, vector<int>(m+2));
	vector<rational> B(m + 1);
	B[0].n = 1;
	B[0].d = 1;
	cout << "B0:" << ' ' << '1' << '\n';

	//ʹ���������ʽC(n,k) = C(n-1,k-1) + C(n-1,k)�õ����������
	//�������
	for (i = 1; i <= m + 1; i++) {
		for (j = 0; j <= i; j++) {
			if (j == 0 || i == j) get_C[i][j] = 1;
			else get_C[i][j] = get_C[i - 1][j - 1] + get_C[i - 1][j];
		}
	}
	//ÿ��ѭ��ʹ����֪��B0-Bi-1���õ�Bi
	for (int i = 1; i<= m; i++) {
		s.n = 0; s.d = 1;
		for (int j = 0; j < i; j++) {
			c.n = B[j].n * get_C[i + 1][j];
			c.d = B[j].d;
			s = temp_plus(s,c);
		}
		s.n = -1 * s.n;
		s.d = s.d * get_C[i + 1][i];
		B[i] = Simplify(s);
		if (B[i].n == 0) cout << "B"<<i<<":"<<' '<<'0' << '\n';
			else cout << "B" << i << ":" << ' '<< B[i].n << '/' << B[i].d << '\n';
	}
	
}