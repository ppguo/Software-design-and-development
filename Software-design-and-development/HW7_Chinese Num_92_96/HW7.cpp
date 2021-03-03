#include <iostream>
#include <string>
#include "simple_test.h"
using namespace std;
namespace Chinese_num {
	const string a[10] = { "��","һ","��","��","��","��","��","��","��","��" };//��ֵ
	const string b[3] = { "ǧ","��","ʮ" };//��λ
	class Chinese_convert {
	public:
		string Num_to_Chinese(unsigned int k);/*ת���ӿ�*/
	private:
		string get_length_4(const string& s, const bool& begin_with_ten);/*4λ����ת��*/
		string get_length_8(const string& s, bool begin_with_ten);/*8λ����ת��*/
		string get_length_12(const string& s);/*12λ����ת��*/
		string delete_duplicate_zero(const string& s);/*ɾ�����ֱ��ʽ�ж������*/
	};
	string Chinese_convert::Num_to_Chinese(unsigned int k) {
		string s;
		s = to_string(k);/*ת��Ϊ�ַ������ڴ���*/
		string convert_s;
		//���ݲ�ͬλ�����в�ͬ�Ĵ���
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
		//ɾ�����ܻ�����Ķ������
		convert_s = delete_duplicate_zero(convert_s);

		return convert_s;
	}
	string Chinese_convert::get_length_4(const string& s, const bool& begin_with_ten) {
		//����4λ�����֣�Ϊ��λ�Ļ������
		//�������
		//ȫ0
		if (s == "0000")
			return a[0];
		//10λΪ1��ͷ���������
		if (s.substr(0, 3) == "001" && begin_with_ten) {
			if (s[3] != '0')
				return "��ʮ" + a[s[3] - 48];
			else
				return "��ʮ";//��Ϊ10�����������
		}

		string s0;
		//����ͷ��0��ֻ����һ��
		int k = 0;
		if (s[k] == '0') {
			s0 += "��";
			while (s[k] == '0')
				k++;
		}
		//�ӷ�0λ��ʼ����ֵת��
		for (int i = k; i < 3; i++) {
			if (s[i] != '0')
				s0 = s0 + a[s[i] - 48] + b[i];
			else
				s0 = s0 + a[s[i] - 48];
		}
		//��λ
		if (s[3] != '0')
			s0 = s0 + a[s[3] - 48];
		//ת����ɺ���ȥĩλ����
		int l = s0.length() - 2;
		while (l >= 0 && s0.substr(l, 2) == "��")
			l -= 2;
		return s0.substr(0, l + 2);
	}
	string Chinese_convert::get_length_8(const string& s, bool begin_with_ten) {
		//����8λ���ֵ����
		string convert_s;
		auto s0_tmp = s.substr(0, 4);
		auto s1_tmp = s.substr(4, 4);
		//����λ�Ĵ���
		if (s0_tmp != "0000")
			convert_s = get_length_4(s0_tmp, begin_with_ten) + "��";
		else
			convert_s = get_length_4(s0_tmp, begin_with_ten);
		//����λ�Ĵ���
		convert_s += get_length_4(s1_tmp, false);
		return convert_s;
	}
	string Chinese_convert::get_length_12(const string& s) {
		//����12λ���ֵ����
		string convert_s;
		auto s0_tmp = s.substr(0, 4);
		auto s1_tmp = s.substr(4, 8);
		//����λ�Ĵ���
		if (s0_tmp != "0000")
			convert_s = get_length_4(s0_tmp, true) + "��";
		convert_s += get_length_8(s1_tmp, false);
		return convert_s;
	}
	string Chinese_convert::delete_duplicate_zero(const string& s) {
		//ǰ��Ĵ����ֶλ���һ���ֶ������
		//��Ҫ�����ڣ�1����ͷ ��2��ĩβ ��3���м��������������Σ�������Σ�
		//��Ҫע����Ǵ˴�һ�����ֻ�ռ�����ַ�
		if (s == "��")
			return s;
		//ɾ����ͷ��β����
		int head = 0;
		int tail = int(s.length()) - 2;
		while (head < tail - 1 && s.substr(head, 2) == "��")
			head = head + 2;
		while (tail >= 0 && s.substr(tail, 2) == "��")
			tail = tail - 2;
		auto s0 = s.substr(head, tail - head + 2);
		//�滻�м������2����Ϊһ����
		for (int i = 0; i < int(s0.length()) - 3; i = i + 2) {
			if (s0.substr(i, 2) == "��" && s0.substr(i + 2, 2) == "��")
				s0.replace(i, 4, "��");
		}
		return s0;
	}
};

int test_main() {
	Chinese_num::Chinese_convert s;
	//С�ڵ�����λ
	CHECK_EQUAL(s.Num_to_Chinese(0), "��");
	CHECK_EQUAL(s.Num_to_Chinese(10), "ʮ");
	CHECK_EQUAL(s.Num_to_Chinese(100), "һ��");
	CHECK_EQUAL(s.Num_to_Chinese(1000), "һǧ");
	CHECK_EQUAL(s.Num_to_Chinese(2010), "��ǧ��һʮ");
	CHECK_EQUAL(s.Num_to_Chinese(9999), "��ǧ�Űپ�ʮ��");
	CHECK_EQUAL(s.Num_to_Chinese(1001), "һǧ��һ");

	//��λ����λ
	CHECK_EQUAL(s.Num_to_Chinese(10001), "һ����һ");
	CHECK_EQUAL(s.Num_to_Chinese(100010), "ʮ����һʮ");
	CHECK_EQUAL(s.Num_to_Chinese(1000100), "һ������һ��");
	CHECK_EQUAL(s.Num_to_Chinese(10001000), "һǧ��һǧ");
	CHECK_EQUAL(s.Num_to_Chinese(10000001), "һǧ����һ");

	//��λ�����ֵ
	CHECK_EQUAL(s.Num_to_Chinese(100001001), "һ����һǧ��һ");
	CHECK_EQUAL(s.Num_to_Chinese(101001010), "һ����һ����һǧ��һʮ");
	CHECK_EQUAL(s.Num_to_Chinese(1000001000), "ʮ����һǧ");
	CHECK_EQUAL(s.Num_to_Chinese(1234567809), "ʮ������ǧ�İ���ʮ������ǧ�˰����");
	return 0;

}