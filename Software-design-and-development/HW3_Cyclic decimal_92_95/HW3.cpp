#include <iostream>
#include <vector>
using namespace std;

void calculate_and_print_Decimal(int n, int d) {
	vector<int> r;/*记录余数*/
	vector<int> decimal_part;/*记录小数部分*/
	int k = -1;/*k用于指示相同的余数第一次出现的坐标*/
	//一直循环，直到余数为0，或出现重复的余数
	while (n && k<0) {
		r.push_back(n);
		decimal_part.push_back(n * 10 / d);
		n = n * 10 % d;
		if (n) {
			for (int i = 0; i < r.size(); i++) {
				if (r[i] == n) {
					k = i;
					break;
				}
			}
		}
	}
	//余数为0，打印已有小数
	if (!n) {
		for (auto i : decimal_part) {
			cout << i;
		}
		cout << "\n";
	}
	//否则输出循环节
	else {
		for (int i = 0; i < k; i++)
			cout << decimal_part[i];
		cout << '(';
		for (int i = k; i < decimal_part.size(); i++)
			cout << decimal_part[i];
		cout << ")\n";
	}
}

int main() {
	cout << "Please input n and d \n" ;
	cout << "input 0 to finish \n";

	while (true) {
		int n, d, r;
		int Int_part;
		cin >> n;
		if (!n)
			break;
		cin >> d;
		Int_part = n / d;
		r = n % d;
		cout << Int_part;
		//如果余数不为0，则计算小数部分
		if (r) {
			cout << '.';
			calculate_and_print_Decimal(r, d);

		}
		else cout << "/n";
	}
	return 0;

}