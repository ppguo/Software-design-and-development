#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <stack>
#include "function.h"

using namespace std;



string Simplify_Expression_Not(string s) {
	//����ԭ�����ڽ������~~�����
	for (int i = s.length(); i >= 0; i--) {
	//�����Ǿͽ��������ŵ�ֵȡ������ȥ������Ǻ�
		if (s[i] == '~') {
			if (s.length() == i + 1 || (s[i + 1] != '0' && s[i + 1] != '1'))
				throw wrong_expression_for_not{}; /*~Ϊ�ַ������һ���ַ������δ����ֵ*/
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
	string s0;/*���ڱ����������ַ���*/
	int k;
	for (int i = 0; i < s.length(); i++) {
		k = s0.length();
		if (s[i] == oper[0]) {
					i++;
					//������ֵ��ϵ������ֲ����ʽ��ֵ
					//�˴����ݹ�ϵ�����˻���ֻ�ڷ���ǰ��ֵ��Ҫ�ı��ʱ����иı�
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
			s0 = s0 + s[i];//����Ҫ����ֲ����ʽʱ��ֱ�ӱ����S0
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
	/*���շ������ȼ����μ򻯱��ʽ*/
	string s0;
	s0 = Simplify_Expression_Not(s);
	Verify_expression_with_binary_oper(s0);
	s0 = Simplify_Expression_Binary_Oper(s0, "&");
	s0 = Simplify_Expression_Binary_Oper(s0, "^");
	s0 = Simplify_Expression_Binary_Oper(s0, "|");
	return s0;

}
string Get_expression_value_with_bracket(string s0) {

	stack <int> left_pare_position;/*�洢������λ�õ�ջ*/
	for (int i = 0; i < s0.length(); i++) {
		if (s0[i] == '(')
			left_pare_position.push(i);/*ɨ�赽�����ź󣬴���ջ*/
		if (s0[i] == ')')
			//ɨ�赽�����ź󣬴�ջ��ȡ����Ӧ�����ŵ�ֵ
			if (!left_pare_position.empty()) {
				int left_position = left_pare_position.top();
				//������֮��ı��ʽȡ������֮���滻ԭʼ�������ŵı��ʽ
				s0.replace(left_position, i-left_position+1, Basic_convertor(s0.substr(left_position + 1, i - (left_position + 1))));
				i = left_position;/*����i��λ��*/
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
	/*�����������Ӧֵ��ӳ���ϵ*/
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
	/*���ַ����ı����滻Ϊֵ*/
	for (int i = 0; i < s.length(); i++) {
		if ((s[i] - 'A' < 0 || s[i] - 'A' >= 26) && s[i] != '&' && s[i] != '|' && s[i] != '~' && s[i] != '^'
			&&s[i]!='('&&s[i]!=')')
			throw wrong_expression_invalidword{};
		if (dic.count(s[i]))
			s[i] = dic[s[i]];
		else if (s[i] - 'A' < 26 && s[i] - 'A' >= 0)
			throw wrong_expression_n{};/*n����ʽ����*/
	}
	return s;
}

string Get_variable_in_expression(const string& s) {
	/*����ַ�����ʵ�ʺ��еı�������*/
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
	/*��ùؼ�������Ӧ��ֵ*/
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
	s1 = Get_variable_in_expression(s);/*��ñ��ʽ��ʵ��ӵ�еı���*/
	for (int i = 0; i < pow(2, n); i++) {
		auto value_map = Build_map(pow(2, n) - i - 1, n);/*��ô˴α�����Ӧ��ֵ*/
		auto key_value = Get_key_value_of_variable(value_map, s1);/*���ʵ��ӵ�еı�����ʱ��Ӧ��ֵ*/
		//���ݱ��ʽ��ʵ�ʴ��ڵı�����Ӧ��ֵ���ж��Ƿ��Ѿ���֮ǰ�������ʽ�����ŵȼ۹�ϵ����ͬ��key_value)
		if (key_value_map.count(key_value)) {
			truthtable += key_value_map[key_value]; //�Ѿ��������ֱ������ӳ��
		}
		else {
			s0 = Get_specific_variable_value(value_map, s);/*�����ʽÿ����������ֵ*/
			auto s2 = Get_expression_value_with_bracket(s0);/*������ʽ��ֵ*/
			truthtable = truthtable + s2;
			key_value_map[key_value] = s2;/*����ؼ�ֵ�Ͷ�Ӧ���ʽֵ��ӳ��*/
		}
	}
	return truthtable;
}


