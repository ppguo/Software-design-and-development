#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <stack>
#include "function.h"

using namespace std;



string Simplify_Expression_Not(string s) {
	//倒序原因：用于解决连续~~的情况
	for (int i = s.length(); i >= 0; i--) {
	//遇到非就将其后面跟着的值取反，并去掉这个非号
		if (s[i] == '~') {
			if (s.length() == i + 1 || (s[i + 1] != '0' && s[i + 1] != '1'))
				throw wrong_expression_for_not{}; /*~为字符串最后一个字符，或后未跟真值*/
			else {
				if (s[i + 1] == '0')
					s[i] = '1';
				else s[i] = '0';
				s.erase(i + 1, 1);
			}
		}
	}
	return s;
}


string Simplify_Expression_Binary_Oper(const string& s, const string& oper) {
	string s0;/*用于保存运算后的字符串*/
	int k;
	for (int i = 0; i < s.length(); i++) {
		k = s0.length();
		if (s[i] == oper[0]) {
					i++;
					//根据真值关系，计算局部表达式的值
					//此处根据关系进行了化简，只在符号前的值需要改变的时候进行改变
					if (oper == "&") {
						if (s0[k - 1] == '1' && s[i] == '0')
							s0[k - 1] = '0';
					}
					else if (oper == "|") {
						if (s0[k - 1] == '0' && s[i] == '1')
							s0[k - 1] = '1';
					}
					else {
						if (s0[k - 1] == '0' && s[i] == '1')
							s0[k - 1] = '1';
						else if (s0[k - 1] == '1' && s[i] == '1')
							s0[k - 1] = '0';
					}
		}
		else
			s0 = s0 + s[i];//不需要计算局部表达式时，直接保存进S0
	}
	return s0;
}

void Verify_expression_with_binary_oper(const string& s0) {
	int l = s0.length();
	if (l % 2==0)
		throw wrong_expression_binary_oper{};
	else {
		for (int i = 0; i < l - 1; i=i+2) {
			if (s0[i]!='0' && s0[i]!='1'||s0[i+1]!='&'&&s0[i+1]!='|'&&s0[i+1]!='^')
				throw wrong_expression_binary_oper{};
		}
		if (s0[l - 1] != '0' && s0[l - 1] != '1')
			throw wrong_expression_binary_oper{};
	}
	return;
}
string Basic_convertor(const string& s) {
	/*按照符号优先级依次简化表达式*/
	string s0;
	s0 = Simplify_Expression_Not(s);
	Verify_expression_with_binary_oper(s0);
	s0 = Simplify_Expression_Binary_Oper(s0, "&");
	s0 = Simplify_Expression_Binary_Oper(s0, "^");
	s0 = Simplify_Expression_Binary_Oper(s0, "|");
	return s0;

}
string Get_expression_value_with_bracket(string s0) {

	stack <int> left_pare_position;/*存储左括号位置的栈*/
	for (int i = 0; i < s0.length(); i++) {
		if (s0[i] == '(')
			left_pare_position.push(i);/*扫描到左括号后，存入栈*/
		if (s0[i] == ')')
			//扫描到右括号后，从栈顶取出对应左括号的值
			if (!left_pare_position.empty()) {
				int left_position = left_pare_position.top();
				//将括号之间的表达式取出计算之后替换原始包含括号的表达式
				s0.replace(left_position, i-left_position+1, Basic_convertor(s0.substr(left_position + 1, i - (left_position + 1))));
				i = left_position;/*更新i的位置*/
				left_pare_position.pop();
			}
			else
				throw wrong_expression_bracket{};
	}
	if (!left_pare_position.empty())
		throw wrong_expression_bracket{};
	return(Basic_convertor(s0));
}
map<char, char> Build_map(int i, int n) {
	/*产生变量与对应值的映射关系*/
	int k = 0;
	map<char, char> variable_value;
	while (k<n) {
		variable_value['A' + k] = i % 2+48;
		i /= 2;
		k++;
	}
	return variable_value;
}

string Get_specific_variable_value(map<char, char> dic, string s) {
	/*将字符串的变量替换为值*/
	for (int i = 0; i < s.length(); i++) {
		if ((s[i] - 'A' < 0 || s[i] - 'A' >= 26) && s[i] != '&' && s[i] != '|' && s[i] != '~' && s[i] != '^'
			&&s[i]!='('&&s[i]!=')')
			throw wrong_expression_invalidword{};
		if (dic.count(s[i]))
			s[i] = dic[s[i]];
		else if (s[i] - 'A' < 26 && s[i] - 'A' >= 0)
			throw wrong_expression_n{};/*n与表达式不符*/
	}
	return s;
}

string Get_variable_in_expression(const string& s) {
	/*获得字符串中实际含有的变量集合*/
	int l = s.length();
	map<char, int> string_letter;
	string s0;
	for (int i = 0; i < l; i++) {
		if (s[i] - 'A' < 26 && s[i] - 'A' >= 0) {
			string_letter[s[i]] = string_letter[s[i]] + 1;
		}
	}
	for (int i = 0; i < 26; i++) {
		if (string_letter.count('A' + i)) {
			s0 += ('A' + i);
		}
	}
	return s0;
}

string Get_key_value_of_variable(map<char, char> value_map, const string& s1) {
	/*获得关键变量对应的值*/
	string s0;
	for (int i = 0; i < s1.length(); i++) {
		s0 += value_map[s1[i]];
	}
	return s0;
}

string expr_to_truthtable(int n, const string& s) {
	string s0,s1;
	string truthtable;
	map<string, string> key_value_map;
	if (!s.length())
		throw empty_expression{};
	s1 = Get_variable_in_expression(s);/*获得表达式中实际拥有的变量*/
	for (int i = 0; i < pow(2, n); i++) {
		auto value_map = Build_map(pow(2, n) - i - 1, n);/*获得此次变量对应的值*/
		auto key_value = Get_key_value_of_variable(value_map, s1);/*获得实际拥有的变量此时对应的值*/
		//根据表达式中实际存在的变量对应的值，判断是否已经和之前计算过的式子有着等价关系（相同的key_value)
		if (key_value_map.count(key_value)) {
			truthtable += key_value_map[key_value]; //已经计算过，直接套用映射
		}
		else {
			s0 = Get_specific_variable_value(value_map, s);/*给表达式每个变量赋予值*/
			auto s2 = Get_expression_value_with_bracket(s0);/*计算表达式的值*/
			truthtable = truthtable + s2;
			key_value_map[key_value] = s2;/*存入关键值和对应表达式值得映射*/
		}
	}
	return truthtable;
}


