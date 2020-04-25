#include "LexicalDFA.h"
#include "utils.h"
using namespace std;

shared_ptr<LexicalDFA> LexicalDFA::_instance = NULL;


TokenErrorType LexicalDFA::Parse(string rawStr, int& curIndex) {
    // 清空信息
    _token = NULL;
    _errStr = "";

    // 初始化
    int strLen = rawStr.length();
    StateEnum curState = StateEnum::START;
    string curTokenStr = "";

    // Lambda：状态流转的过程
    auto stateTurn = [&curIndex, &curState, &curTokenStr](char curChar, StateEnum state) {
        ++curIndex;
        curTokenStr += curChar;
        curState = state;
    };

    while (curIndex != strLen) {
        char curChar = rawStr[curIndex];
        CharTypeEnum curCharType = JudgeCharType(curChar);

        // 首先统一排除INVILID错误
        if (curCharType == CharTypeEnum::INVILID) {
            _errStr = curTokenStr + curChar;
            return TokenErrorType::INVILID_CHAR;
        }

        // 状态机过程
        switch (curState)
        {

            /********************  状态START  *****************************/
        case LexicalDFA::StateEnum::START:
            switch (curCharType)
            {
            case CharTypeEnum::DIGIT:   // 识别数字
                stateTurn(curChar, StateEnum::NUMBER_1);
                break;
            case CharTypeEnum::LETTER:  // 识别identifier
                stateTurn(curChar, StateEnum::IDENTIFIER_1);
                break;
            case CharTypeEnum::UNDERLINE:   // 错误的开始字符
                _errStr = "_";
                return TokenErrorType::UNEXPECTED_CHAR_BEGIN;
            case CharTypeEnum::SYMBOL:  // 识别symbol，需要详细判断字符类型
                if (curChar == '&')
                    stateTurn(curChar, StateEnum::SYMBOL_1);
                else if (curChar == '.')
                    stateTurn(curChar, StateEnum::SYMBOL_3);
                else
                    stateTurn(curChar, StateEnum::SYMBOL_2);
                break;
            case CharTypeEnum::SPACE:   // 空转
                ++curIndex;
                break;
            }
            break;

            /*********************  状态SYMBOL_1  *****************************/
        case LexicalDFA::StateEnum::SYMBOL_1:
            if (curChar == '&') {
                stateTurn(curChar, StateEnum::SYMBOL_2);
            }
            else {
                _errStr = curTokenStr + curChar;
                return TokenErrorType::UNEXPECTED_CHAR_SYMB_1;
            }
            break;

            /*********************  状态SYMBOL_2  *****************************/
        case LexicalDFA::StateEnum::SYMBOL_2:
            _token = shared_ptr<Token>(new Token(TokenTypeEnum::SYMBOL, curTokenStr));
            return TokenErrorType::OK;

            /*********************  状态SYMBOL_3  *****************************/
        case LexicalDFA::StateEnum::SYMBOL_3:
            if (curCharType == CharTypeEnum::DIGIT) {   // 浮点错误
                _errStr = curTokenStr + curChar;
                return TokenErrorType::FLOAT_ERROR_SYMB_3;
            }
            else {      // 接受
                _token = shared_ptr<Token>(new Token(TokenTypeEnum::SYMBOL, curTokenStr));
                return TokenErrorType::OK;
            }
            break;

            /*********************  状态IDENTIFIER_1  *****************************/
        case LexicalDFA::StateEnum::IDENTIFIER_1:
            switch (curCharType)
            {
            case CharTypeEnum::DIGIT:       // 状态不变
            case CharTypeEnum::LETTER:
                stateTurn(curChar, StateEnum::IDENTIFIER_1);
                break;
            case CharTypeEnum::UNDERLINE:   // 状态流转
                stateTurn(curChar, StateEnum::IDENTIFIER_2);
                break;
            case CharTypeEnum::SYMBOL:      // 返回标识符
            case CharTypeEnum::SPACE:
                _token = shared_ptr<Token>(new Token(TokenTypeEnum::IDENTIFIER, curTokenStr));
                return TokenErrorType::OK;
            }
            break;

            /*********************  状态IDENTIFIER_2  *****************************/
        case LexicalDFA::StateEnum::IDENTIFIER_2:
            switch (curCharType)
            {
            case CharTypeEnum::DIGIT:       // 状态流转
            case CharTypeEnum::LETTER:
                stateTurn(curChar, StateEnum::IDENTIFIER_1);
                break;
            case CharTypeEnum::UNDERLINE:   // 错误字符
            case CharTypeEnum::SYMBOL:
            case CharTypeEnum::SPACE:
                _errStr = curTokenStr + curChar;
                return TokenErrorType::UNEXPECTED_CHAR_IDEN_2;
            }
            break;

            /*********************  状态NUMBER_1  *****************************/
        case LexicalDFA::StateEnum::NUMBER_1:
            switch (curCharType)
            {
            case CharTypeEnum::DIGIT:   // 状态不变
                stateTurn(curChar, StateEnum::NUMBER_1);
                break;
            case CharTypeEnum::LETTER:      // 错误的字符
            case CharTypeEnum::UNDERLINE:
                _errStr = curTokenStr + curChar;
                return TokenErrorType::UNEXPECTED_CHAR_NUM1;
            case CharTypeEnum::SYMBOL:
                if (curChar == '.') {     // 浮点错误
                    _errStr = curTokenStr + curChar;
                    return TokenErrorType::FLOAT_ERROR_NUM1;
                }
                else {
                    _token = shared_ptr<Token>(new Token(TokenTypeEnum::INTEGER, stoi(curTokenStr)));
                    return TokenErrorType::OK;
                }
            case CharTypeEnum::SPACE:   // 识别完成
                _token = shared_ptr<Token>(new Token(TokenTypeEnum::INTEGER, stoi(curTokenStr)));
                return TokenErrorType::OK;
            }
            break;
        }
    }


    // 到达了字符串的末尾
    switch (curState) { // 根据状态设置_token或_errStr，并返回状态码
    case StateEnum::START:
        return TokenErrorType::SPACE;
    case StateEnum::NUMBER_1:
        _token = shared_ptr<Token>(new Token(TokenTypeEnum::INTEGER, stoi(curTokenStr)));
        return TokenErrorType::OK;
    case StateEnum::IDENTIFIER_1:
        _token = shared_ptr<Token>(new Token(TokenTypeEnum::IDENTIFIER, curTokenStr));
        return TokenErrorType::OK;
    case StateEnum::IDENTIFIER_2:
        _errStr = curTokenStr + "[endl]";
        return TokenErrorType::UNEXPECTED_CHAR_IDEN_2;
    case StateEnum::SYMBOL_1:
        return TokenErrorType::UNEXPECTED_CHAR_SYMB_1;
    case StateEnum::SYMBOL_2:
    case StateEnum::SYMBOL_3:
        _token = shared_ptr<Token>(new Token(TokenTypeEnum::SYMBOL, curTokenStr));
        return TokenErrorType::OK;
    }
}




