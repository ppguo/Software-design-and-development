#include <string>
#include <map>
using namespace std;
struct empty_expression {};
struct wrong_expression_length {};
struct wrong_expression_for_not {};
struct wrong_expression_before_binary_oper {};
struct wrong_expression_after_binary_oper {};
struct wrong_expression_lack_left_bracket {};
struct wrong_expression_bracket {};
struct wrong_expression_n {};
struct wrong_expression_binary_oper{};
struct wrong_expression_invalidword{};

/*string Simplify_Expression_Not(string s);
string Simplify_Expression_Binary_Oper(const string& s, const string& oper);
string Get_expression_value_with_bracket(string s0);
map<char, char> Build_map(int i, int n);
string Get_specific_variable_value(map<char, char> dic, string s);
string Get_variable_in_expression(const string& s);*/

/*void Verify_expression_with_binary_oper(const string& s0);*/
string expr_to_truthtable(int n, const string& s);
string truthtable_to_expr(const string& truthtable);


