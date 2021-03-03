using namespace std;
#include <iostream>
#include <tuple>
#include <algorithm>
#include "simple_test.h"
template < typename Iter, // Iter models BidirectionalIterator 
		typename Pred> // Pred models UnaryPredicate 
std::pair<Iter, Iter> 
gather(Iter first, Iter last, Iter pivot, Pred pred) {
	auto head = stable_partition(first, pivot, [pred](auto k) {return !pred(k); });
	auto end = stable_partition(pivot, last, pred);
	return make_pair(head, end);
}
int  test_main() {
	int a[10] = { 3,5,8,4,9,1,0,7,2,6 }; 
	pair<int*, int*> k = gather(a, a + 10, a + 6, [](auto i) {return i >= 5; });
	int a_after[10] = { 3,4,1,5,8,9,7,6,0,2 };
	for (int i = 0; i < 10;i++) {
		CHECK_EQUAL(a[i], a_after[i]);
	}
	CHECK_EQUAL(a + 3, k.first);
	CHECK_EQUAL(a + 8, k.second);
	//末尾边界
	int b[10] = { 7,5,8,4,9,1,0,7,2,6 };
	pair<int*, int*> k_b = gather(b, b + 10, b + 9, [](auto i) {return i >= 5; });
	int b_after[10] = {4,1,0,2,7,5,8,9,7,6  };
	for (int i = 0; i < 10; i++) {
		CHECK_EQUAL(b[i], b_after[i]);
	}
	CHECK_EQUAL(b + 4, k_b.first);
	CHECK_EQUAL(b + 10, k_b.second);
	//开头边界
	int c[10] = { 7,5,8,4,9,1,0,7,2,6 };
	pair<int*, int*> k_c = gather(c, c +10, c , [](auto i) {return i >= 5; });
	int c_after[10] = { 7,5,8,9,7,6,4,1,0,2};
	for (int i = 0; i < 10; i++) {
		CHECK_EQUAL(c[i], c_after[i]);
	}
	CHECK_EQUAL(c, k_c.first);
	CHECK_EQUAL(c+6, k_c.second);
	return 0;
}