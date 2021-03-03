#include "simple_test.h"
struct NULL_ERROR {
};
const char* find(const char* s, const char* x) {
	if (s == NULL || x == NULL)
		throw NULL_ERROR{};
	const char* x0 = x;
	const char* s0 = s;
	while (*s0 != '\0') {
		auto s1 = s0;
		while (*s1 == *x0 && *x0 != '\0') {
			s1++;
			x0++;
		 }
		if (*x0 == '\0') {
			return s0;
		}
		x0 = x;
		s0++;
	}
	return nullptr;
}
int test_main() {
	char s[] = "12345";
	char x[] = "23";
	char x1[] = "45";
	char x2[] = "8";
	auto y0 = s;
	//���Բ����Ƿ���ȷ
	y0++;
	CHECK_EQUAL(find(s,x),y0);
	y0++;
	y0++;
	CHECK_EQUAL(find(s, x1), y0);
	//�����ܷ����޷��ҵ�
	CHECK_EQUAL(find(s, x2), (char*)nullptr);
	CHECK_EQUAL(find(x2, s), (char*)nullptr);
	//���Կ�ָ���쳣
	CHECK_THROW(find(s, nullptr), NULL_ERROR);
	CHECK_THROW(find(nullptr, nullptr), NULL_ERROR);
	CHECK_THROW(find(nullptr, x), NULL_ERROR);
	return 0;
}