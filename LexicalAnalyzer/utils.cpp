#include <set>
#include "utils.h"
using namespace std;

/*
 关键字集合。特别注意：并没有包括"System.out.println"
 */
const set<string> keywords = { "class", "public", "static", "void", "main", "String", "extends",
						"return", "int", "boolean",  "if", "else", "while" ,
						"length", "true", "false", "this", "new" };
/*
 判断一个字符串在不在关键字集合里面
 */
bool IsKeyword(const string tokenStr) {
	return keywords.find(tokenStr) != keywords.end();
}



/*
 符号集合
 并没有包含&&，而是包含了&
*/
const set<char> symbols = { '[', ']', '(', ')', '{', '}', ',', ';', '=', '&', '<', '+', '-', '*', '.', '!' };

/*
 判断一个字符的类型：下划线，字符，符号，数字
 */
CharTypeEnum JudgeCharType(const char c) {
	if (c == '_')
		return CharTypeEnum::UNDERLINE;
	if (c == ' ' || c == '\t')
		return CharTypeEnum::SPACE;
	if ('0' <= c && c <= '9')
		return CharTypeEnum::DIGIT;
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
		return CharTypeEnum::LETTER;
	if (symbols.find(c) != symbols.end())
		return CharTypeEnum::SYMBOL;
	return CharTypeEnum::INVILID;
}