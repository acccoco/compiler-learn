#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Token.h"
#include "TokenError.h"

/*
 词法分析有穷自动机类
*/
class LexicalDFA
{
    /*
     状态机的类型枚举
    */
    enum class StateEnum {
        START,                  // 初始状态
        END,                    // 终止状态

        SYMBOL_1,               // 识别symbol的symbol_1出现了错误
        SYMBOL_2,               // 识别symbol的symbol_2出现了错误
        SYMBOL_3,               // 识别symbol的symbol_3出现了错误

        IDENTIFIER_1,           // 识别identifier的identifier_1出现了错误
        IDENTIFIER_2,           // 识别identifier的identifier_2出现了错误

        NUMBER_1,               // 识别integerLiteral的状态出现了错误
    };

public:
    shared_ptr<Token> GetToken() {
        return _token;
    }
    /*
     返回的错误信息包括index指向的那个字符
     */
    std::string GetError() {
        return _errStr;
    }
    /*
     获得单例实例
     */
    static shared_ptr<LexicalDFA> GetInstance() {
        if (_instance == NULL)
            _instance = shared_ptr<LexicalDFA>(new LexicalDFA());
        return _instance;
    }
    /*
     销毁单例实例
     */
    static void DestroyInstance() {
        if (_instance != NULL) _instance = NULL;
    }
    /*
     输入一行字符串，从curIndex处开始解析
     解析出一个就返回
     引用参数：如果有错，curIndex指向错误位置；如果没有错误，curIndex指向下一个将被处理的位置
     返回：返回具体的错误代码
    */
    TokenErrorType Parse(std::string rawStr, size_t& curIndex);

private:
    static shared_ptr<LexicalDFA> _instance;       // 单例实例
    shared_ptr<Token> _token;           // 识别出的 Token
    std::string _errStr;                // 错误的上下文

protected:
    LexicalDFA(): _token(NULL), _errStr("") {}  // 不允许调用构造函数
};

