#include <iostream>
#include <string>
#include <random>
#include <tuple>

using namespace std;

string generate_random_number() {
	string s;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 9);
	int t = 0;
	s = s + to_string(dis(gen));
	while (t < 3) {
		string c = to_string(dis(gen));
		if (s.find(c) == s.npos) {
			t++;
			s = s + c;
		}		
	}
	//cout << s << "\n";
	return s;
}
bool Verify_Input(const string& s) {
	int l = s.length();
	if (l != 4) return false;
	for (int i = 0; i < 4; i++)
		if (s[i] < '0' || s[i] > '9') return false;
	return true;
}

tuple<int,int> get_feedback(const string& a, const string& b) {
	int m = 0;
	int n = 0;
	for (int i = 0; i < 4; i++) {
		if (a[i] == b[i])
			m++;
		else {
			if (a.find(b[i]) != a.npos)
				n++;
		}
	}
	return { m,n };

}
int main() {
	string number,random_num;
	int m, n,start_sign;
	start_sign = 0;
	//use start to restart the whole game, while use quit to end the game(can't get in again)!
	cout << "plz input start to get started\n";
	while (true) {
		cin >> number;
		if (number == "start") {
			cout << "Start! Plz input the 4-bit number you guess!!\n";
			random_num = generate_random_number();
			start_sign = 1;
		}
		else if (number == "quit")
			break;
		else if (Verify_Input(number) && start_sign) {
			tie(m,n) = get_feedback(random_num, number);
			cout << m << "A" << n << "B\n";
			if (m == 4) {
				cout << "You win!!Input start to restart!!\n";
			}
		}
		else {
			cout << "error input! plz input again\n";

		}
	}
	return 0;
}