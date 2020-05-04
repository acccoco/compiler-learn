#pragma once
#include "../LexicalAnalyzer/Token.h"

/*
 词法token类型，对原 Token 类进行扩展，添加了行号和列号的字段
*/
class EToken {
private:

    TokenTypeEnum _type;    // Token 的类型
    int _numValue;          // Token 的数字值，与字符串值二选一
    string _strValue;       // Token 的字符串值，与数字值二选一

    int _lineNum;      // Token的行号
    int _column;        // Token在行中所处的位置

public:

    /* 通过一个token对象来构建Etoken对象，需要提供行号和列号 */
    EToken(TokenPtr token, int line, int column):
        _lineNum(line),
        _column(column),
        _type(token->type),
        _numValue(token->numValue),
        _strValue(token->strValue) {}

    Property<int> LineNum = Property<int>(_lineNum);    // Token的行号
    ROProperty<int> NumValue = ROProperty<int>(_numValue);  // token的数字值，与字符出串值二选一
    ROProperty<string> StrValue = ROProperty<string>(_strValue);    // token的字符串值，与数字值二选一
    ROProperty<TokenTypeEnum> TokenType = ROProperty<TokenTypeEnum>(_type); // token的类型
};

/* 扩展的词法token类型 */
typedef std::shared_ptr<EToken> ETokenPtr;