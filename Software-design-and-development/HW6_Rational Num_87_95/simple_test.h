// This is a simple testing facility for the SFDD lesson.
// Written by Zhou XueGong.

#ifndef SIMPLE_TEST_H
#define SIMPLE_TEST_H

#include <iostream>

#define ERROR(msg) \
	{ SimpleTest::checkPoint(__FILE__, __LINE__); SimpleTest::reportError((msg)); }

#define FAIL(msg) \
	{ SimpleTest::checkPoint(__FILE__, __LINE__); SimpleTest::reportCritical((msg)); }

#define CHECK(exp) \
	{ SimpleTest::checkPoint(__FILE__, __LINE__); if (!(exp)) SimpleTest::checkError(#exp); }

#define REQUIRE(exp) \
	{ SimpleTest::checkPoint(__FILE__, __LINE__); if (!(exp)) SimpleTest::checkCritical(#exp); }

#define CHECK_EQUAL(exp1, exp2) \
	{ SimpleTest::checkPoint(__FILE__, __LINE__); SimpleTest::checkEqual(#exp1, #exp2, (exp1), (exp2)); }

#define REQUIRE_EQUAL(exp1, exp2) \
	{ SimpleTest::checkPoint(__FILE__, __LINE__); SimpleTest::checkEqualCritical(#exp1, #exp2, (exp1), (exp2)); }

#define CHECK_THROW(S, E) \
	{ SimpleTest::checkPoint(__FILE__, __LINE__); try { S; SimpleTest::throwError(#S, #E); } catch(E) {} }

namespace SimpleTest {
	const int exitSuccess = 0;
	const int exitTestFailure = 201;

	static int errorCnt = 0;
	static int checkLine = 0;
	static const char* checkFile = 0;

	class CriticalError {};

	inline void checkPoint(const char* file, int line) { checkFile = file; checkLine = line; }

	inline void Error() { ++errorCnt; std::cerr << checkFile << "(" << checkLine << ")"; }

	inline void reportError(const char* msg) { Error(); std::cerr << ": " << msg << std::endl; }

	inline void reportCritical(const char* msg) { reportError(msg); throw CriticalError(); }

	inline void checkError(const char* exp) { Error(); std::cerr << ": test " << exp << " failed" << std::endl; }

	inline void checkCritical(const char* exp) { checkError(exp); throw CriticalError(); }

	template<typename T1, typename T2>
	inline bool checkEqual(const char* exp1, const char* exp2, T1 val1, T2 val2) {
		if (val1 == val2) return false;
		Error();
		std::cerr << ": test " << exp1 << " == " << exp2 << " failed [" << val1 << " != " << val2 << "]" << std::endl;
		return true;
	}

	template<typename T1, typename T2>
	inline void checkEqualCritical(const char* exp1, const char* exp2, T1 val1, T2 val2) {
		if (checkEqual(exp1, exp2, val1, val2))
			throw CriticalError();
	}

	inline void throwError(const char* stmt, const char* ex) {
		Error();
		std::cerr << ": test " << stmt << " failed: exception " << ex << " expected." << std::endl;
	}

	inline void exceptionError() {
		std::cerr << "unexpected exception caught. last checkpoint: ";
		Error();
		std::cerr << std::endl;
	}
}

int main() {
	using namespace SimpleTest;
	int test_main();

	try {
		int result = test_main();
		CHECK_EQUAL(result, 0);
	}
	catch (CriticalError) {
		std::cerr << "Critical error occurred. Testing aborted.\n";
	}
	catch (...) {
		exceptionError();
	}
	switch (errorCnt) {
		case 0:  std::cout << " **** no errors detected\n"; return exitSuccess;
		case 1:  std::cerr << " **** 1 error detected\n"; break;
		default: std::cerr << " **** " << errorCnt << " errors detected\n"; break;
	}
	return exitTestFailure;
}

#endif