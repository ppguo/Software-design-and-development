#include <iostream>
#include <string>
#include "simple_test.h"
using namespace std;
namespace Chinese_num {
	const string a[10] = { "零","一","二","三","四","五","六","七","八","九" };//数值
	const string b[3] = { "千","百","十" };//单位
	class Chinese_convert {
	public:
		string Num_to_Chinese(unsigned int k);/*转换接口*/
	private:
		string get_length_4(const string& s, const bool& begin_with_ten);/*4位数字转换*/
		string get_length_8(const string& s, bool begin_with_ten);/*8位数字转换*/
		string get_length_12(const string& s);/*12位数字转换*/
		string delete_duplicate_zero(const string& s);/*删除文字表达式中多余的零*/
	};
	string Chinese_convert::Num_to_Chinese(unsigned int k) {
		string s;
		s = to_string(k);/*转换为字符串便于处理*/
		string convert_s;
		//根据不同位数进行不同的处理
		if (s.length() <= 4) {
			while (s.length() < 4)
				s = "0" + s;
			convert_s = get_length_4(s, true);
		}
		else if (s.length() <= 8) {
			while (s.length() < 8)
				s = "0" + s;
			convert_s = get_length_8(s, true);
		}
		else {
			while (s.length() < 12)
				s = "0" + s;
			convert_s = get_length_12(s);
		}
		//删除可能会产生的多余的零
		convert_s = delete_duplicate_zero(convert_s);

		return convert_s;
	}
	string Chinese_convert::get_length_4(const string& s, const bool& begin_with_ten) {
		//处理4位的数字，为多位的基本情况
		//特殊情况
		//全0
		if (s == "0000")
			return a[0];
		//10位为1开头的情况处理
		if (s.substr(0, 3) == "001" && begin_with_ten) {
			if (s[3] != '0')
				return "零十" + a[s[3] - 48];
			else
				return "零十";//若为10后续不需加零
		}

		string s0;
		//处理开头的0，只保留一个
		int k = 0;
		if (s[k] == '0') {
			s0 += "零";
			while (s[k] == '0')
				k++;
		}
		//从非0位开始的数值转换
		for (int i = k; i < 3; i++) {
			if (s[i] != '0')
				s0 = s0 + a[s[i] - 48] + b[i];
			else
				s0 = s0 + a[s[i] - 48];
		}
		//个位
		if (s[3] != '0')
			s0 = s0 + a[s[3] - 48];
		//转换完成后消去末位的零
		int l = s0.length() - 2;
		while (l >= 0 && s0.substr(l, 2) == "零")
			l -= 2;
		return s0.substr(0, l + 2);
	}
	string Chinese_convert::get_length_8(const string& s, bool begin_with_ten) {
		//处理8位数字的情况
		string convert_s;
		auto s0_tmp = s.substr(0, 4);
		auto s1_tmp = s.substr(4, 4);
		//高四位的处理
		if (s0_tmp != "0000")
			convert_s = get_length_4(s0_tmp, begin_with_ten) + "万";
		else
			convert_s = get_length_4(s0_tmp, begin_with_ten);
		//低四位的处理
		convert_s += get_length_4(s1_tmp, false);
		return convert_s;
	}
	string Chinese_convert::get_length_12(const string& s) {
		//处理12位数字的情况
		string convert_s;
		auto s0_tmp = s.substr(0, 4);
		auto s1_tmp = s.substr(4, 8);
		//高四位的处理
		if (s0_tmp != "0000")
			convert_s = get_length_4(s0_tmp, true) + "亿";
		convert_s += get_length_8(s1_tmp, false);
		return convert_s;
	}
	string Chinese_convert::delete_duplicate_zero(const string& s) {
		//前面的处理手段会有一部分多余的零
		//主要存在于（1）开头 （2）末尾 （3）中间零连续出现两次（最多两次）
		//需要注意的是此处一个汉字会占两个字符
		if (s == "零")
			return s;
		//删除开头结尾的零
		int head = 0;
		int tail = int(s.length()) - 2;
		while (head < tail - 1 && s.substr(head, 2) == "零")
			head = head + 2;
		while (tail >= 0 && s.substr(tail, 2) == "零")
			tail = tail - 2;
		auto s0 = s.substr(head, tail - head + 2);
		//替换中间的连续2个零为一个零
		for (int i = 0; i < int(s0.length()) - 3; i = i + 2) {
			if (s0.substr(i, 2) == "零" && s0.substr(i + 2, 2) == "零")
				s0.replace(i, 4, "零");
		}
		return s0;
	}
};

int test_main() {
	Chinese_num::Chinese_convert s;
	//小于等于四位
	CHECK_EQUAL(s.Num_to_Chinese(0), "零");
	CHECK_EQUAL(s.Num_to_Chinese(10), "十");
	CHECK_EQUAL(s.Num_to_Chinese(100), "一百");
	CHECK_EQUAL(s.Num_to_Chinese(1000), "一千");
	CHECK_EQUAL(s.Num_to_Chinese(2010), "二千零一十");
	CHECK_EQUAL(s.Num_to_Chinese(9999), "九千九百九十九");
	CHECK_EQUAL(s.Num_to_Chinese(1001), "一千零一");

	//四位到八位
	CHECK_EQUAL(s.Num_to_Chinese(10001), "一万零一");
	CHECK_EQUAL(s.Num_to_Chinese(100010), "十万零一十");
	CHECK_EQUAL(s.Num_to_Chinese(1000100), "一百万零一百");
	CHECK_EQUAL(s.Num_to_Chinese(10001000), "一千万一千");
	CHECK_EQUAL(s.Num_to_Chinese(10000001), "一千万零一");

	//八位到最大值
	CHECK_EQUAL(s.Num_to_Chinese(100001001), "一亿零一千零一");
	CHECK_EQUAL(s.Num_to_Chinese(101001010), "一亿零一百万一千零一十");
	CHECK_EQUAL(s.Num_to_Chinese(1000001000), "十亿零一千");
	CHECK_EQUAL(s.Num_to_Chinese(1234567809), "十二亿三千四百五十六万七千八百零九");
	return 0;

}