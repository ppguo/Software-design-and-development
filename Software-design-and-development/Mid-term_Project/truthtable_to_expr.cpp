#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <tuple>
#include "function.h"

using namespace std;
#define precision 1e-6

struct Prime_implicant {//函数项结构
	map<int, int> combination_production; //存储含有哪些最小项，<最小项序号，0或1>
	//dcba
	string value;//函数的字符表达形式，由0，1，-组成
	int one_num = 0;//字符串中的个数
	bool prime_flag = true;//是否为质蕴涵

	//重载了加号，使得两函数项的合并便利
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
	//petrick算法中用于得到最小覆盖的逻辑式计算的表格结构
	vector<string> temple; 

	//重载了加法，为两个字符串的集的合并
	Petrick_type operator+(Petrick_type& PR) {
		Petrick_type tmp;
		for (int i = 0; i < this->temple.size(); i++)
			tmp.temple.push_back(this->temple[i]);
		for (int j = 0; j < PR.temple.size(); j++)
			tmp.temple.push_back(PR.temple[j]);
		return tmp;
	}
	//重载了乘法用于与操作
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
	//质蕴涵项集合的表格形式
	vector<Prime_implicant> list;
	//记录质蕴涵项集合中每个最小项的个数
	map<int, int> count;
};
struct Pri_and_pet {
	//为了质蕴涵项集合向Petrick type过渡的时候便利使用的混合结构
	Prime_implicant Pri;
	Petrick_type Pre;
};

class Find_Prime_implicant_list {
public:
	void Build_sum_of_minterm(const string& truthtable) {
		//根据真值表转换为初始的最小项之和的形式
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
		//依次迭代产生最终的质蕴涵集合
		vector<vector<Prime_implicant>> list_implicant(l_int + 1, vector<Prime_implicant>(0));//存储每次迭代产生的根据1的个数分类好的列表
		for (int i = 0; i < V0.size(); i++)
			list_implicant[V0[i].one_num].push_back(V0[i]);//将初始最小项集合转换为上述结构
		bool continue_flag = true;//判断是否有新的列表产生
		while (continue_flag) {
			continue_flag = false;
			vector<vector<Prime_implicant>> list_implicant_tmp(l_int + 1, vector < Prime_implicant>(0));
			for (int i = 0; i < l_int; i++) {
				for (int j = 0; j < list_implicant[i].size(); j++) {
					for (int m = 0; m < list_implicant[i + 1].size(); m++) {
						//在适合的集合里寻找是否有能合并的项
						if (can_combine(list_implicant[i][j].value, list_implicant[i + 1][m].value)) {
							//能合并，合并加入新的列表
							auto pl = list_implicant[i][j] + list_implicant[i + 1][m];
							//下判断合并生成的项是否已经存在，若存在就不再加入
							//此操作避免了重复的质蕴涵项的生成，且避免了中间列表占用内存过大
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
						//如果一个项没有找到能合并的其他项，其就是质蕴涵项
					if (list_implicant[i][j].prime_flag) {
						PIL.list.push_back(list_implicant[i][j]);
						for (int y = 0; y < len; y++) {
							if (list_implicant[i][j].combination_production[y])
								PIL.count[y] = PIL.count[y] + 1;
						}
					}
				}

			}
			//对于列表中有l_int个1的字符串的情况处理，边界条件
			for (int j = 0; j < list_implicant[l_int].size(); j++) {
				if (list_implicant[l_int][j].prime_flag) {
					PIL.list.push_back(list_implicant[l_int][j]);
					for (int y = 0; y < len; y++) {
						if (list_implicant[l_int][j].combination_production[y])
							PIL.count[y] = PIL.count[y] + 1;
					}
				}
			}
			//进入下一个列表的扫描
			list_implicant = list_implicant_tmp;
		}
		return PIL;
	}
private:
	int len = 0;//真值表长度，不同最小项的总个数
	int l_int = 0;//逻辑表达式中变量个数
	vector<Prime_implicant> V0;//初始建立的真值表对应的实际最小项集合
	Prime_implicant_list PIL;//质蕴涵集合表格

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
		//获得实质质蕴涵项，并且在质蕴涵的表格中删除其对应行和对应列
		PIL = P;
		len_com = len;
		l_int = l;
		for (int i = 0; i < len_com; i++) {
			//判断是否为实质质蕴涵项
			if (PIL.count[i] == 1) {
				for (int j = 0; j < PIL.list.size(); j++) {
					//判断是否被删除和含有实质质蕴涵项
					if (PIL.list[j].combination_production[i] && PIL.list[j].prime_flag) {
						essential_prime_implicant.push_back(PIL.list[j].value);/*保存实质质蕴涵项*/
						PIL.list[j].prime_flag = false;/*在表格中删去行*/
						//在表格中删去列
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
		//在删去实质质蕴涵的基础上，寻找最小覆盖
		vector<Pri_and_pet> P;
		int k = 0;
		int length_string = 0;
		//根据表格的行数，得到二进制编号的长度
		for (int i = 0; i < PIL.list.size(); i++) {
			if (PIL.list[i].prime_flag)
				length_string++;
		}

		for (int i = 0; i < PIL.list.size(); i++) {
			//判断是否被删去
			if (PIL.list[i].prime_flag) {
				//进行结构的转换，用于后续计算
				Pri_and_pet p0;
				p0.Pri = PIL.list[i];
				p0.Pre.temple.push_back(get_string(k, length_string));
				k++;
				P.push_back(p0);
			}
		}
		//计算逻辑关系产生的表达式
		Petrick_type m0 = calculate_algebraic_expression(P, length_string);
		//根据最终结果，扫描得到1最少的项，并还原得到最小覆盖
		mincover_prime_implicant = get_min_cover_by_algebraic_result(m0, P, length_string);
	}
	void combine_essential_mincover() {
		//把必要质蕴涵项和最小覆盖找到的蕴涵项合并，并按规则排序（方便测试）
		combined_essential_mincover.insert(combined_essential_mincover.end(), essential_prime_implicant.begin(), essential_prime_implicant.end());
		combined_essential_mincover.insert(combined_essential_mincover.end(), mincover_prime_implicant.begin(), mincover_prime_implicant.end());
		//排序规则，认为字符串为三进制数，-代表-1，最左边为最高位，最右边位最低位，高位值大则值大，否则比较低位
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
		//获得最终的表达式
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
	Prime_implicant_list PIL;//质蕴涵集合表格
	vector<string> essential_prime_implicant, mincover_prime_implicant, combined_essential_mincover;
	//实质质蕴涵项，最小覆盖产生的项，两者的合并（string是0，1，-表示的函数项）
	string expr;
	//根据string的集合转换的最终的变量表达式
	int len_com = 0;
	//真值表长度，最小项个数
	int l_int = 0;
	//变量个数

	string get_string(int k, int length_string) {
		//给每个质蕴涵项一个编号，用于petrick的表格式计算
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
		//对每次与后的结果使用(X+XY=X)进行化简
		int len = m0.temple.size();
		vector<bool> delete_flag(len, false);//用于判断是否需要删除
		for (int i = 0; i < m0.temple.size() - 1; i++) {
			for (int j = i + 1; j < m0.temple.size(); j++) {
				string s = multiply(m0.temple[i], m0.temple[j]);
				//如果存在X和XY，则他们的multiply后的值和XY相同
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
			//将不用删除的值留下
			if (!delete_flag[i]) {
				m1.temple.push_back(m0.temple[i]);
			}
		}
		return m1;
	}

	Petrick_type calculate_algebraic_expression(vector<Pri_and_pet> P, int length_string) {
		Petrick_type m0;
		m0.temple.push_back(get_string(-1, length_string));//为了简便，初始m0为全-的字符，使得任何值与他相*为本身
		for (int i = 0; i < len_com; i++) {
			Petrick_type m1;
			//利用表格，提取表达式信息
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
		//根据表达式最后的结果得到最小覆盖
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
		//ASCII码中1>0>-
		for (int i = 0; i < s0.length(); i++) {
			if (s0[i] > s1[i])
				return true;
			else if (s0[i] < s1[i])
				return false;
		}
		return false;
	}

	int get_one_num(const string& s) {
		//得到字符串中1的个数
		int k = 0;
		for (int i = 0; i < s.length(); i++)
			if (s[i] == '1')
				k++;
		return k;
	}
	string multiply(const string& s0, const string& s1) {
		//为了简便定义的两编号字符串相与的运算
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
	//检测输入异常
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
	auto l_int = Verify_input_truthtable(truthtable);//对输入项的检查，无异常得到变量个数
	//对全0和全1的特殊情况的处理
	string full_1(len, '1');
	string full_0 (len, '0');
	if (!l_int)
		return truthtable;
	else if (full_0 == truthtable)
		return "0";
	else if (full_1 == truthtable)
		return "1";

	//建立质蕴涵集合表格阶段
	Find_Prime_implicant_list PL;
	PL.Build_sum_of_minterm(truthtable);//根据真值表得到最小项之和
	auto PIL = PL.generate_prime_implicant_list();//生成质蕴涵表格
	

	//寻找最小覆盖并得到最后表达式结果
	Petrick Petr;
	Petr.Get_and_delete_essential_prime_implicant(PIL, len, l_int);//得到实质质蕴涵集合，并且在表格中删去对应列和对应行
	Petr.build_min_cover();//在删去实质质蕴涵后，找到最小覆盖的质蕴涵项结合
	Petr.combine_essential_mincover();//将实质与最小覆盖找到的集合合并，并按一定规则排序（便于测试）
	return(Petr.get_expr());//将合并后的集合转换为逻辑表达式的形式输出
}




