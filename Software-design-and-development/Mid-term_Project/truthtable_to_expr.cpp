#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <tuple>
#include "function.h"

using namespace std;
#define precision 1e-6

struct Prime_implicant {//������ṹ
	map<int, int> combination_production; //�洢������Щ��С�<��С����ţ�0��1>
	//dcba
	string value;//�������ַ������ʽ����0��1��-���
	int one_num = 0;//�ַ����еĸ���
	bool prime_flag = true;//�Ƿ�Ϊ���̺�

	//�����˼Ӻţ�ʹ����������ĺϲ�����
	Prime_implicant operator+(Prime_implicant PR) {
		Prime_implicant tmp;
		for (int i = 0; i < this->value.length(); i++) {
			if (this->value[i] == '0' && PR.value[i] == '1' ||
				this->value[i] == '1' && PR.value[i] == '0')
				tmp.value = tmp.value + "-";
			else
				tmp.value = tmp.value + this->value[i];
		}
		tmp.one_num = get_one_num(tmp.value);
		for (int i = 0; PR.combination_production.count(i); i++) {
			if (this->combination_production[i] == 1 || PR.combination_production[i] == 1)
				tmp.combination_production[i] = 1;
			else
				tmp.combination_production[i] = 0;
		}
		return tmp;
	}

	int get_one_num(const string& s) {
		int k = 0;
		for (int i = 0; i < s.length(); i++)
			if (s[i] == '1')
				k++;
		return k;
	}
};
struct Petrick_type {
	//petrick�㷨�����ڵõ���С���ǵ��߼�ʽ����ı��ṹ
	vector<string> temple; 

	//�����˼ӷ���Ϊ�����ַ����ļ��ĺϲ�
	Petrick_type operator+(Petrick_type& PR) {
		Petrick_type tmp;
		for (int i = 0; i < this->temple.size(); i++)
			tmp.temple.push_back(this->temple[i]);
		for (int j = 0; j < PR.temple.size(); j++)
			tmp.temple.push_back(PR.temple[j]);
		return tmp;
	}
	//�����˳˷����������
	Petrick_type operator*(Petrick_type& PR) {
		Petrick_type tmp;
		for (int i = 0; i < this->temple.size(); i++) {
			for (int j = 0; j < PR.temple.size(); j++) {
				tmp.temple.push_back(multiply(this->temple[i], PR.temple[j]));
			}
		}
		return tmp;
	}
	string multiply(const string& s0, const string& s1) {
		string s;
		for (int i = 0; i < s0.length(); i++) {
			if (s0[i] == '1' || s1[i] == '1')
				s = s + "1";
			else s = s + "-";

		}
		return s;
	}
};
struct Prime_implicant_list {
	//���̺���ϵı����ʽ
	vector<Prime_implicant> list;
	//��¼���̺������ÿ����С��ĸ���
	map<int, int> count;
};
struct Pri_and_pet {
	//Ϊ�����̺������Petrick type���ɵ�ʱ�����ʹ�õĻ�Ͻṹ
	Prime_implicant Pri;
	Petrick_type Pre;
};

class Find_Prime_implicant_list {
public:
	void Build_sum_of_minterm(const string& truthtable) {
		//������ֵ��ת��Ϊ��ʼ����С��֮�͵���ʽ
		len = truthtable.length();
		l_int = int(log2(len));
		for (int i = 0; i < len; i++) {
			if (truthtable[i] == '1') {
				Prime_implicant m;
				for (int j = 0; j < len; j++) {
					m.combination_production[j] = 0;
				}
				m.combination_production[len - i - 1] = 1;
				m.value = get_bit_form(len - i - 1);
				m.one_num = get_one_num(m.value);
				V0.push_back(m);
			}
		}
	}
	Prime_implicant_list generate_prime_implicant_list() {
		//���ε����������յ����̺�����
		vector<vector<Prime_implicant>> list_implicant(l_int + 1, vector<Prime_implicant>(0));//�洢ÿ�ε��������ĸ���1�ĸ�������õ��б�
		for (int i = 0; i < V0.size(); i++)
			list_implicant[V0[i].one_num].push_back(V0[i]);//����ʼ��С���ת��Ϊ�����ṹ
		bool continue_flag = true;//�ж��Ƿ����µ��б����
		while (continue_flag) {
			continue_flag = false;
			vector<vector<Prime_implicant>> list_implicant_tmp(l_int + 1, vector < Prime_implicant>(0));
			for (int i = 0; i < l_int; i++) {
				for (int j = 0; j < list_implicant[i].size(); j++) {
					for (int m = 0; m < list_implicant[i + 1].size(); m++) {
						//���ʺϵļ�����Ѱ���Ƿ����ܺϲ�����
						if (can_combine(list_implicant[i][j].value, list_implicant[i + 1][m].value)) {
							//�ܺϲ����ϲ������µ��б�
							auto pl = list_implicant[i][j] + list_implicant[i + 1][m];
							//���жϺϲ����ɵ����Ƿ��Ѿ����ڣ������ھͲ��ټ���
							//�˲����������ظ������̺�������ɣ��ұ������м��б�ռ���ڴ����
							bool existed_flag = false;
							for (int k = 0; k < list_implicant_tmp[pl.one_num].size(); k++) {
								if (list_implicant_tmp[pl.one_num][k].value == pl.value)
									existed_flag = true;
							}
							if (!existed_flag)
								list_implicant_tmp[pl.one_num].push_back(pl);

							list_implicant[i][j].prime_flag = false;
							list_implicant[i + 1][m].prime_flag = false;
							continue_flag = true;
						}
					}
						//���һ����û���ҵ��ܺϲ����������������̺���
					if (list_implicant[i][j].prime_flag) {
						PIL.list.push_back(list_implicant[i][j]);
						for (int y = 0; y < len; y++) {
							if (list_implicant[i][j].combination_production[y])
								PIL.count[y] = PIL.count[y] + 1;
						}
					}
				}

			}
			//�����б�����l_int��1���ַ�������������߽�����
			for (int j = 0; j < list_implicant[l_int].size(); j++) {
				if (list_implicant[l_int][j].prime_flag) {
					PIL.list.push_back(list_implicant[l_int][j]);
					for (int y = 0; y < len; y++) {
						if (list_implicant[l_int][j].combination_production[y])
							PIL.count[y] = PIL.count[y] + 1;
					}
				}
			}
			//������һ���б��ɨ��
			list_implicant = list_implicant_tmp;
		}
		return PIL;
	}
private:
	int len = 0;//��ֵ���ȣ���ͬ��С����ܸ���
	int l_int = 0;//�߼����ʽ�б�������
	vector<Prime_implicant> V0;//��ʼ��������ֵ���Ӧ��ʵ����С���
	Prime_implicant_list PIL;//���̺����ϱ��

	string get_bit_form(int i) {
		//value is abcd
		int k = 0;
		string s;
		while (k < l_int) {
			if (i % 2)
				s = s + "1";
			else
				s = s + "0";
			i /= 2;
			k++;
		}
		return s;
	}
	bool can_combine(string& s1, string& s2) {
		bool k = true;
		int num_diff = 0;
		for (int i = 0; i < s1.length(); i++) {
			if (s1[i] != s2[i]) {
				num_diff++;
				if (s1[i] == '-' || s2[i] == '-')
					k = false;
			}
		}
		if (num_diff > 1)
			k = false;
		return k;
	}
	int get_one_num(const string& s) {
		int k = 0;
		for (int i = 0; i < s.length(); i++)
			if (s[i] == '1')
				k++;
		return k;
	}

};
class Petrick {
public:
	void Get_and_delete_essential_prime_implicant(const Prime_implicant_list& P, int len, int l) {
		//���ʵ�����̺�����������̺��ı����ɾ�����Ӧ�кͶ�Ӧ��
		PIL = P;
		len_com = len;
		l_int = l;
		for (int i = 0; i < len_com; i++) {
			//�ж��Ƿ�Ϊʵ�����̺���
			if (PIL.count[i] == 1) {
				for (int j = 0; j < PIL.list.size(); j++) {
					//�ж��Ƿ�ɾ���ͺ���ʵ�����̺���
					if (PIL.list[j].combination_production[i] && PIL.list[j].prime_flag) {
						essential_prime_implicant.push_back(PIL.list[j].value);/*����ʵ�����̺���*/
						PIL.list[j].prime_flag = false;/*�ڱ����ɾȥ��*/
						//�ڱ����ɾȥ��
						for (int m = 0; m < len_com; m++) {
							if (PIL.list[j].combination_production[m])
								PIL.count[m] = 0;
						}
					}
				}
			}
		}
	}
	void build_min_cover() {
		//��ɾȥʵ�����̺��Ļ����ϣ�Ѱ����С����
		vector<Pri_and_pet> P;
		int k = 0;
		int length_string = 0;
		//���ݱ����������õ������Ʊ�ŵĳ���
		for (int i = 0; i < PIL.list.size(); i++) {
			if (PIL.list[i].prime_flag)
				length_string++;
		}

		for (int i = 0; i < PIL.list.size(); i++) {
			//�ж��Ƿ�ɾȥ
			if (PIL.list[i].prime_flag) {
				//���нṹ��ת�������ں�������
				Pri_and_pet p0;
				p0.Pri = PIL.list[i];
				p0.Pre.temple.push_back(get_string(k, length_string));
				k++;
				P.push_back(p0);
			}
		}
		//�����߼���ϵ�����ı��ʽ
		Petrick_type m0 = calculate_algebraic_expression(P, length_string);
		//�������ս����ɨ��õ�1���ٵ������ԭ�õ���С����
		mincover_prime_implicant = get_min_cover_by_algebraic_result(m0, P, length_string);
	}
	void combine_essential_mincover() {
		//�ѱ�Ҫ���̺������С�����ҵ����̺���ϲ��������������򣨷�����ԣ�
		combined_essential_mincover.insert(combined_essential_mincover.end(), essential_prime_implicant.begin(), essential_prime_implicant.end());
		combined_essential_mincover.insert(combined_essential_mincover.end(), mincover_prime_implicant.begin(), mincover_prime_implicant.end());
		//���������Ϊ�ַ���Ϊ����������-����-1�������Ϊ���λ�����ұ�λ���λ����λֵ����ֵ�󣬷���Ƚϵ�λ
			for (int j = 0; j < combined_essential_mincover.size() - 1; j++) {
				for (int m = j + 1; m < combined_essential_mincover.size(); m++) {
					if (larger(combined_essential_mincover[m],combined_essential_mincover[j])) {
						auto tmp = combined_essential_mincover[m];
						combined_essential_mincover[m] = combined_essential_mincover[j];
						combined_essential_mincover[j] = tmp;
					}
				}
			}
	}
	string get_expr() {
		//������յı��ʽ
		int size = combined_essential_mincover.size();
		if (size == 1) {
			int len = combined_essential_mincover[0].length();
			for (int i = 0; i < len; i++) {
				if (combined_essential_mincover[0][i] == '1') {
					char c0 = 'A' + i;
					expr += c0;
					expr = expr + "&";
				}
				else if (combined_essential_mincover[0][i] == '0') {
					char c0 = 'A' + i;
					expr = expr + "~";
					expr += c0;
					expr = expr + "&";
				}
			}
			if (expr.length() && expr[expr.length() - 1] == '&')
				expr.erase(expr.length() - 1);
		}
		else {
			for (int i = 0; i < size; i++) {
				int len = combined_essential_mincover[i].length();
				for (int j = 0; j < len; j++) {
					if (combined_essential_mincover[i][j] == '1') {
						char c0 = 'A' + j;
						expr += c0;
						expr = expr + "&";
					}
					else if (combined_essential_mincover[i][j] == '0') {
						char c0 = 'A' + j;
						expr = expr + "~";
						expr += c0;
						expr = expr + "&";
					}	
				}
				if (expr.length() && expr[expr.length() - 1] == '&')
					expr.erase(expr.length()-1);
				if (i < size - 1)
					expr = expr + "|";
			}
		}
		return expr;
	}

private:
	Prime_implicant_list PIL;//���̺����ϱ��
	vector<string> essential_prime_implicant, mincover_prime_implicant, combined_essential_mincover;
	//ʵ�����̺����С���ǲ���������ߵĺϲ���string��0��1��-��ʾ�ĺ����
	string expr;
	//����string�ļ���ת�������յı������ʽ
	int len_com = 0;
	//��ֵ���ȣ���С�����
	int l_int = 0;
	//��������

	string get_string(int k, int length_string) {
		//��ÿ�����̺���һ����ţ�����petrick�ı��ʽ����
		string s;
		for (int i = 0; i < length_string; i++) {
			if (i == k)
				s = s + "1";
			else
				s = s + "-";
		}
		return s;
	}

	Petrick_type simplify(const Petrick_type& m0) {
		//��ÿ�����Ľ��ʹ��(X+XY=X)���л���
		int len = m0.temple.size();
		vector<bool> delete_flag(len, false);//�����ж��Ƿ���Ҫɾ��
		for (int i = 0; i < m0.temple.size() - 1; i++) {
			for (int j = i + 1; j < m0.temple.size(); j++) {
				string s = multiply(m0.temple[i], m0.temple[j]);
				//�������X��XY�������ǵ�multiply���ֵ��XY��ͬ
				if (s == m0.temple[i]) {
					delete_flag[i] = true;
				}
				else if (s == m0.temple[j]) {
					delete_flag[j] = true;
				}
			}
		}
		Petrick_type m1;
		for (int i = 0; i < m0.temple.size(); i++) {
			//������ɾ����ֵ����
			if (!delete_flag[i]) {
				m1.temple.push_back(m0.temple[i]);
			}
		}
		return m1;
	}

	Petrick_type calculate_algebraic_expression(vector<Pri_and_pet> P, int length_string) {
		Petrick_type m0;
		m0.temple.push_back(get_string(-1, length_string));//Ϊ�˼�㣬��ʼm0Ϊȫ-���ַ���ʹ���κ�ֵ������*Ϊ����
		for (int i = 0; i < len_com; i++) {
			Petrick_type m1;
			//���ñ����ȡ���ʽ��Ϣ
			if (PIL.count[i]) {
				for (int j = 0; j < P.size(); j++) {
					if (P[j].Pri.combination_production[i])
						m1 = m1 + P[j].Pre;
				}
				m0 = m0 * m1;
				m0 = simplify(m0);
			}
		}
		return m0;
	}

	vector<string> get_min_cover_by_algebraic_result(Petrick_type m0, vector<Pri_and_pet> P, int length_string) {
		//���ݱ��ʽ���Ľ���õ���С����
		vector<string> min_cover;
		int min = length_string;
		string tmp_simplist;
		for (int i = 0; i < m0.temple.size(); i++) {
			int tmp_one_num = get_one_num(m0.temple[i]);
			if (tmp_one_num <= min) {
				min = tmp_one_num;
				tmp_simplist = m0.temple[i];
			}
		}
		for (int i = 0; i < tmp_simplist.size(); i++) {
			if (tmp_simplist[i] == '1') {
				min_cover.push_back(P[i].Pri.value);
			}
		}
		return min_cover;
	}

	bool larger(const string& s0, const string& s1) {
		//ASCII����1>0>-
		for (int i = 0; i < s0.length(); i++) {
			if (s0[i] > s1[i])
				return true;
			else if (s0[i] < s1[i])
				return false;
		}
		return false;
	}

	int get_one_num(const string& s) {
		//�õ��ַ�����1�ĸ���
		int k = 0;
		for (int i = 0; i < s.length(); i++)
			if (s[i] == '1')
				k++;
		return k;
	}
	string multiply(const string& s0, const string& s1) {
		//Ϊ�˼�㶨���������ַ������������
		string s;
		for (int i = 0; i < s0.length(); i++) {
			if (s0[i] == '1' || s1[i] == '1')
				s = s + "1";
			else s = s + "-";

		}
		return s;
	}
};
int Verify_input_truthtable(const string& truthtable) {
	if (!truthtable.length())
		throw empty_expression{};
	int len = truthtable.length();
	auto l = log2(len);
	auto l_int = int(l);
	//��������쳣
	if (abs(l - l_int) > precision)
		throw wrong_expression_length{};
	for (int i = 0; i < len; i++) {
		if (truthtable[i] != '0' && truthtable[i] != '1')
			throw wrong_expression_invalidword{};
	}
	return l_int;
}
string truthtable_to_expr(const string& truthtable) {

	int len = truthtable.length();
	auto l_int = Verify_input_truthtable(truthtable);//��������ļ�飬���쳣�õ���������
	//��ȫ0��ȫ1����������Ĵ���
	string full_1(len, '1');
	string full_0 (len, '0');
	if (!l_int)
		return truthtable;
	else if (full_0 == truthtable)
		return "0";
	else if (full_1 == truthtable)
		return "1";

	//�������̺����ϱ��׶�
	Find_Prime_implicant_list PL;
	PL.Build_sum_of_minterm(truthtable);//������ֵ��õ���С��֮��
	auto PIL = PL.generate_prime_implicant_list();//�������̺����
	

	//Ѱ����С���ǲ��õ������ʽ���
	Petrick Petr;
	Petr.Get_and_delete_essential_prime_implicant(PIL, len, l_int);//�õ�ʵ�����̺����ϣ������ڱ����ɾȥ��Ӧ�кͶ�Ӧ��
	Petr.build_min_cover();//��ɾȥʵ�����̺����ҵ���С���ǵ����̺�����
	Petr.combine_essential_mincover();//��ʵ������С�����ҵ��ļ��Ϻϲ�������һ���������򣨱��ڲ��ԣ�
	return(Petr.get_expr());//���ϲ���ļ���ת��Ϊ�߼����ʽ����ʽ���
}




