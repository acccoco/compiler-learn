#pragma once
#include <string>
#include <sstream>
using namespace std;


/*
 词法分析错误类型定义
*/
enum class TokenErrorType {
	OK,							// 正常完成
	SPACE,						// 全是空格

	INVILID_CHAR,				// 非法字符
	FLOAT_ERROR_SYMB_3,			// 浮点错误   .23
	UNEXPECTED_CHAR_SYMB_1,		// 符号错误   &x
	UNEXPECTED_CHAR_IDEN_2,		// 标识符错误 acc_x
	FLOAT_ERROR_NUM1,			// 浮点错误  23.
	UNEXPECTED_CHAR_NUM1,		// 数字错误  23x
	UNEXPECTED_CHAR_BEGIN,		// 开始错误  _
};


/*
 词法分析错误对象
*/
struct TokenError {
	int lineNum;				// 错误出现的行号
	int column;					// 错误出现的位置
	TokenErrorType tokenType;	// 错误类型
	string errMsg;				// 错误信息

	TokenError(int lineNum, int column, TokenErrorType errType, string errContex) {
		this->lineNum = lineNum;
		this->column = column;
		this->tokenType = errType;


		/*
		 构建错误对象
		 at line 23, column : xxx.
		 */
		ostringstream oss;
		oss << "at line " << lineNum << ", " << "column " << column << ": ";
		switch (errType)
		{
		case TokenErrorType::INVILID_CHAR:
			oss << "非法字符 ";
			break;
		case TokenErrorType::FLOAT_ERROR_NUM1:
		case TokenErrorType::FLOAT_ERROR_SYMB_3:
			oss << "不支持浮点数 ";
			break;
		case TokenErrorType::UNEXPECTED_CHAR_SYMB_1:
			oss << "错误的符号 ";
			break;
		case TokenErrorType::UNEXPECTED_CHAR_IDEN_2:
			oss << "错误的标识符 ";
			break;
		case TokenErrorType::UNEXPECTED_CHAR_NUM1:
			oss << "错误的整型 ";
			break;
		case TokenErrorType::UNEXPECTED_CHAR_BEGIN:
			oss << "错误的开始字符 ";
			break;
		}
		oss << "\"" << errContex << "\"";
		errMsg = oss.str();
	}
};