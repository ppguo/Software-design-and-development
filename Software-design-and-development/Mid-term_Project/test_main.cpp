#include <iostream>
#include "function.h"
#include "simple_test.h"
using namespace std;

int test_main() {
//表达式转换为真值表

	//空字符串
	CHECK_THROW(expr_to_truthtable(2, ""), empty_expression);
	//存在无效字符
	CHECK_THROW(expr_to_truthtable(2, "a|B"), wrong_expression_invalidword);
	CHECK_THROW(expr_to_truthtable(2, "A*B"), wrong_expression_invalidword);
	//n与表达式不匹配
	CHECK_THROW(expr_to_truthtable(2, "A|B|C"), wrong_expression_n);
	//括号不匹配
	CHECK_THROW(expr_to_truthtable(4, "(A|B)&C)"), wrong_expression_bracket);
	CHECK_THROW(expr_to_truthtable(4, "(A|B&C"), wrong_expression_bracket);
	//与非相关的运算存在问题
	CHECK_THROW(expr_to_truthtable(4, "(A|B)~&C"), wrong_expression_for_not);
	CHECK_THROW(expr_to_truthtable(4, "(A|B)&C~"), wrong_expression_for_not);
	CHECK_THROW(expr_to_truthtable(4, "(A|B~)&C"), wrong_expression_for_not);
	//与二元运算符相关的运算存在问题
	CHECK_THROW(expr_to_truthtable(4, "(A|B|)&C"), wrong_expression_binary_oper);
	CHECK_THROW(expr_to_truthtable(4, "(A|B&)&~C"), wrong_expression_binary_oper);
	CHECK_THROW(expr_to_truthtable(4, "(A|B^)&C"), wrong_expression_binary_oper);
	//输出测试
	CHECK_EQUAL(expr_to_truthtable(1, "A"), "10");    //边界值 
	CHECK_EQUAL(expr_to_truthtable(8, "A"), 
		"1010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010"); //边界值 
	CHECK_EQUAL(expr_to_truthtable(3, "~A"), "01010101");  //边界值 
	CHECK_EQUAL(expr_to_truthtable(3, "~A|~~~~B&C"), "11010101");
	CHECK_EQUAL(expr_to_truthtable(4, "A&B&D|((~B&C)&~D|~A&B&~D)|A&~C"), "1000101001111110");//正确但较为特殊的表达方式
	CHECK_EQUAL(expr_to_truthtable(3, "A|B^C"), "10111110");

//真值表转换为表达式
	//空输入异常
	CHECK_THROW(truthtable_to_expr(""), empty_expression);
	//无效输入字符
	CHECK_THROW(truthtable_to_expr("01210100"), wrong_expression_invalidword);
	CHECK_THROW(truthtable_to_expr("01*10100"), wrong_expression_invalidword);
	//无效输入长度
	CHECK_THROW(truthtable_to_expr("0101111"), wrong_expression_length);
	CHECK_THROW(truthtable_to_expr("01011111111"), wrong_expression_length);

	//真值表的特殊情况
	CHECK_EQUAL(truthtable_to_expr("0"),"0");
	CHECK_EQUAL(truthtable_to_expr("11111111"), "1");
	//正常值测试
	CHECK_EQUAL(truthtable_to_expr("1000101001111110"), "A&B&D|A&~C|~A&B&~D|~B&C&~D");
	CHECK_EQUAL(truthtable_to_expr("11100111"), "A&C|~A&B|~B&~C");
	CHECK_EQUAL(truthtable_to_expr("11111111111111111111101010101010"),"A|C&D|E");
	CHECK_EQUAL(truthtable_to_expr("11110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111"), 
		"C&D&E&F&G|~C&~D&~E&~F&~G");
	CHECK_EQUAL(truthtable_to_expr("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110"),
		"A|B|C|D|E|F|G");
	CHECK_EQUAL(truthtable_to_expr("11111111111111111000101001111110"), "A&B&D|A&~C|~A&B&~D|~B&C&~D|E");

	//CHECK_EQUAL(truthtable_to_expr(expr_to_truthtable(8, "A|B|C|D|E|F|G")), "A|B|C|D|E|F|G");
	//需要经过较长时间，默认屏蔽
	
	return 0;
}