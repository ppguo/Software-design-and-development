#include <iostream>
#include <vector>
#include "rational.h"

using namespace std;
#define N 30
int pt[N + 1][N + 2] = { 0 };
#define comb(m, n) pt[m][n+1]

void gen_comb()		/* 生成组合数 */
{
	int i, j;
	comb(0, 0) = 1;
	for (i = 1; i <= N; i++)
		for (j = 0; j <= i; j++)
			comb(i, j) = comb(i - 1, j - 1) + comb(i - 1, j);
}

int main() {
	gen_comb();
	int n;
	cin >> n;
	vector<Rational> bn;
	bn.push_back(Rational{ 1 }); 
	for (int m = 1; m <= n; ++m) { 
		Rational s; 
		for (int k = 0; k < m; ++k) 
			s += comb(m + 1, k)*bn[k]; 
		bn.push_back(s*Rational{ -1, m + 1 }); 
	}
	for (int i=0;i<=n;i++)
		cout <<"B"<<i<<" = "<<bn[i]<<"\n";
}