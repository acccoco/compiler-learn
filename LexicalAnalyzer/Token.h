/*************************************************
  Token相关的定义：
    enum class TokenTypeEnum 定义了Token的类型
    class Token 定义了Token的格式
***********************************************/
#pragma once
#include <string>
#include <memory>
#include <iostream>
using namespace std;





/*
 Token类型枚举
 */
enum class TokenTypeEnum {
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INTEGER,
};

/*
 Token对象
 */
struct Token {

    TokenTypeEnum type;
    int numValue = 0;
    string strValue = "";


    Token(TokenTypeEnum type, int numValue)
        : type(type), numValue(numValue) {}
    Token(TokenTypeEnum type, string strValue)
        : type(type), strValue(strValue) {}
    Token(const Token& token)
        :type(token.type), numValue(token.numValue), strValue(token.strValue) {}
    ~Token() {}
};
typedef std::shared_ptr<Token> TokenPtr;
