#pragma once
#include <string>
/***************************
 辅助模块：
   枚举：字符类型
   函数：判断字符的类型
   函数：字符串是否是关键字
 *************************/



/*
 字符类型枚举
 */
enum class CharTypeEnum {
	DIGIT,			    // 数字： 0-9
	LETTER,			    // 字母： A-Za-z
	UNDERLINE,		    // 下划线 '_'
	SYMBOL,			    // 符号：位于符号集合中
	SPACE,			    // 空白字符：空格，制表符
	INVILID,			// 非法符号
};

/*
 判断一个字符的类型：下划线，字符，符号，数字
 */
CharTypeEnum JudgeCharType(const char c);

/*
 判断一个字符串在不在关键字集合里面
 */
bool IsKeyword(const std::string tokenStr);