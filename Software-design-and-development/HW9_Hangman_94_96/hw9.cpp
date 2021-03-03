#include "GUI.h"
#include "Graph.h" 

#include <vector>
#include <string>
#include <ctype.h>
#include <random>
#include <fstream>
using namespace Graph_lib;
using namespace std;

struct open_error{};

struct Hangman_model {/*�жϳ�������״̬�Լ���������Ľṹ*/
	Hangman_model(); //���캯��
	void reset();//��Ϸ���¿�ʼ�ĳ�ʼ��
	string current_label;//��ǰӦ����ʾ�Ĳ²����ַ� ��sc_re
	string word;//��ʱ�²�Ĵ���
	vector<string> wordlist; //���캯��ʱ������ȡ�Ĵ��Ｏ
	int current_state = 1;//��ǰ��Ϸ״̬��0����ɹ���1-6������Ϸ�м�״̬��7����ʧ��
	void model_judge(char key);//�����ť����ж�
};

Hangman_model::Hangman_model() {//����ʿ⣬���ѡȡ����������ǰ��ʾ���ַ�
		string file_name = "words.txt";
		ifstream ist(file_name.c_str());
		if (!ist) throw open_error{};
		string word_tmp;
		while (ist >> word_tmp) wordlist.push_back(word_tmp);

		reset();
}

void Hangman_model::reset() {//������������������ʾ�ַ���
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(0, wordlist.size());
		int rd_num = dis(gen);
		word = wordlist[rd_num];

		current_label = "";
		for (int i = 0; i < word.length(); i++)
			current_label += "_ ";//���Ͽո�ʹ����ҿ����жϳ���
		current_state = 1;
}

void Hangman_model::model_judge(char key) {//���ݵ��������ַ�����״̬
	bool not_found = true;
	bool win = true;
	for (unsigned int i = 0; i < word.length(); i++) {
		if (word[i] == key) {//�ж�Ӧ�ַ�����ʾ
			current_label[2 * i] = key;
			not_found = false;
		}
	}
	if (not_found) {//�²�ʧ�ܣ���һ��
		current_state++;
		win = false;
	}
	else {
		for (unsigned int i = 0; i < current_label.length(); i++) {
			if (current_label.substr(i, 1) == "_")
				win = false;
		}
	}

	if (win)
		current_state = 0;
}

class HangmanWin : public Window {
public:
	HangmanWin();
	
private:
	Hangman_model model;//�߼�ģ��
	Button quit_button,start_button;//��ʼ���˳���ť
	vector<Button*> buttons;//���̰�ť����
	Canvas man_fig;//��ʾС�˻���
	Image man_img = { {0,0},"1.bmp" };//�����ϵ�ͼƬ
	Label result;//�²�����е��ַ���
	Button& create_button(char key);//�������̰�ť
	void key_pressed(char key);//���̰�ť�ص�����
	void start_function();//��ʼ��ť�ص�����
};

void HangmanWin::start_function() {//���¿���
	//��ʾ���м��̰�ť
	for (int i = 0; i < 26; i++) {
		buttons[i]->show();
	}
	//�߼�ģ������
	model.reset();
	//������ʾ��ʼ�ַ���,��"___"
	result.set(model.current_label);
	//��ʾ��ʼͼƬ
	man_img.open("1.bmp");
	man_fig.redraw();
}

HangmanWin::HangmanWin() :Window{ 600,400, "Hangman" },
	quit_button{ "Quit", Exit },
	start_button{ "Start",[this] {start_function(); } },
	result{ "______" }//���ڹ����Լ����ֺ���
{
	div("vert<weight=300 <draw> <button grid = [7,4]> >" 
		"vert<weight=50<word>>"
		"<controll gap=20 margin=5>");
    place("draw", man_fig);
	man_fig.attach(man_img);
	for (int i = 0; i < 26; i++)
		place("button", create_button('a' + i));
	place("word",result);
	result.set(model.current_label);
	place("controll", start_button);
	place("controll", quit_button);
	collocate();
}

Button& HangmanWin::create_button(char key) {
	auto lbl = string(1, key);
	auto& kb = *new Button{ lbl, [key, this] { key_pressed(key); } };
	buttons.push_back(&kb);
	attach(kb);
	kb.set_font({ "", 12u });
	return kb;
}

void HangmanWin::key_pressed(char key) {
	buttons[key-'a']->hide();//���������
	model.model_judge(key);//����ģ���д��������ַ��ĺ���
	man_img.open(to_string(model.current_state)+".bmp" );
	man_fig.redraw();//����ģ�ʹ�����״̬������ʾͼƬ
	result.set(model.current_label);//������ʾ�ַ���
	if (model.current_state == 7 || model.current_state == 0) {//�ɹ�����ʧ�ܺ��������м��̣���ʾ�²ⵥ��
		for (int i = 0; i < 26; i++) {
			buttons[i]->hide();
		}
		result.set(model.word);
	}
}


int main()
{
	HangmanWin win;
	gui_main(win);
}
