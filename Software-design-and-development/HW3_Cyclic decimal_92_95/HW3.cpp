#include <iostream>
#include <vector>
using namespace std;

void calculate_and_print_Decimal(int n, int d) {
	vector<int> r;/*��¼����*/
	vector<int> decimal_part;/*��¼С������*/
	int k = -1;/*k����ָʾ��ͬ��������һ�γ��ֵ�����*/
	//һֱѭ����ֱ������Ϊ0��������ظ�������
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
	//����Ϊ0����ӡ����С��
	if (!n) {
		for (auto i : decimal_part) {
			cout << i;
		}
		cout << "\n";
	}
	//�������ѭ����
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
		//���������Ϊ0�������С������
		if (r) {
			cout << '.';
			calculate_and_print_Decimal(r, d);

		}
		else cout << "/n";
	}
	return 0;

}