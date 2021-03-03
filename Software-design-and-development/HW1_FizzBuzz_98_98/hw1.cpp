#include <iostream>
using namespace std;
void main() {
	int k;
	for (int i = 1; i <= 100; i++) {
		k = 0;
		if (i % 3 == 0) {
			cout << "Fizz";
			k = 1;
		}
		if (i % 5 == 0) {
			cout << "Buzz";
			k = 1;
		}
		if (k == 0) cout << i;
		cout << endl;

	}
}