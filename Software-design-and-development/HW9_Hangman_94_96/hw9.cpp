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

struct Hangman_model {/*判断程序运行状态以及处理输入的结构*/
	Hangman_model(); //构造函数
	void reset();//游戏重新开始的初始化
	string current_label;//当前应该显示的猜测结果字符 如sc_re
	string word;//此时猜测的词语
	vector<string> wordlist; //构造函数时创建读取的词语集
	int current_state = 1;//当前游戏状态，0代表成功，1-6代表游戏中间状态，7代表失败
	void model_judge(char key);//点击按钮后的判断
};

Hangman_model::Hangman_model() {//读入词库，随机选取，并产生当前显示的字符
		string file_name = "words.txt";
		ifstream ist(file_name.c_str());
		if (!ist) throw open_error{};
		string word_tmp;
		while (ist >> word_tmp) wordlist.push_back(word_tmp);

		reset();
}

void Hangman_model::reset() {//重新随机词语，并更新显示字符串
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(0, wordlist.size());
		int rd_num = dis(gen);
		word = wordlist[rd_num];

		current_label = "";
		for (int i = 0; i < word.length(); i++)
			current_label += "_ ";//加上空格使得玩家可以判断长度
		current_state = 1;
}

void Hangman_model::model_judge(char key) {//根据点击输入的字符更新状态
	bool not_found = true;
	bool win = true;
	for (unsigned int i = 0; i < word.length(); i++) {
		if (word[i] == key) {//有对应字符，显示
			current_label[2 * i] = key;
			not_found = false;
		}
	}
	if (not_found) {//猜测失败，加一笔
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
	Hangman_model model;//逻辑模型
	Button quit_button,start_button;//开始和退出按钮
	vector<Button*> buttons;//键盘按钮集合
	Canvas man_fig;//显示小人画布
	Image man_img = { {0,0},"1.bmp" };//画布上的图片
	Label result;//猜测过程中的字符串
	Button& create_button(char key);//创建键盘按钮
	void key_pressed(char key);//键盘按钮回调函数
	void start_function();//开始按钮回调函数
};

void HangmanWin::start_function() {//重新开局
	//显示所有键盘按钮
	for (int i = 0; i < 26; i++) {
		buttons[i]->show();
	}
	//逻辑模型重置
	model.reset();
	//重新显示初始字符串,即"___"
	result.set(model.current_label);
	//显示初始图片
	man_img.open("1.bmp");
	man_fig.redraw();
}

HangmanWin::HangmanWin() :Window{ 600,400, "Hangman" },
	quit_button{ "Quit", Exit },
	start_button{ "Start",[this] {start_function(); } },
	result{ "______" }//窗口构造以及布局函数
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
	buttons[key-'a']->hide();//点击后隐藏
	model.model_judge(key);//调用模型中处理输入字符的函数
	man_img.open(to_string(model.current_state)+".bmp" );
	man_fig.redraw();//根据模型处理后的状态更新显示图片
	result.set(model.current_label);//更新显示字符串
	if (model.current_state == 7 || model.current_state == 0) {//成功或者失败后隐藏所有键盘，显示猜测单词
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
