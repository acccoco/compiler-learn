#pragma once
#include "../LexicalAnalyzer/Token.h"

/*
 扩展的Token
 主要是添加了行号
*/
class EToken {
private:
    TokenTypeEnum _type;
    int _numValue;
    string _strValue;

    int _line;
    int _column;

public:
    EToken(TokenPtr token, int line, int column) {
        _line = line;
        _column = column;

        _type = token->type;
        _numValue = token->numValue;
        _strValue = token->strValue;
    }
    /*
     取得行号
    */
    int GetLineNum() {
        return _line;
    }
    /*
     取得类型
    */
    TokenTypeEnum GetTokenType() {
        return _type;
    }
    /*
     取得值str值
    */
    string GetStrValue() {
        return _strValue;
    }

    /*
     取得num值
    */
    int GetNumValue() {
        return _numValue;
    }
};
/*
 EToken的指针类型
*/
typedef std::shared_ptr<EToken> ETokenPtr;