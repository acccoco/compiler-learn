#pragma once
#include <string>
#include "TreeType.h"
#include "TokenReader.h"
#include <sstream>

/*
 语法错误的定义
*/
class SyntaxError {
protected:
    size_t _curLineNum;     // 错误发生时的上一行
    size_t _preLineNum;     // 错误发生时的当前行
    size_t _curIndex;       // 错误发生时reader的指针位置
    size_t _columnNum;      // 错误发生与行内的第几个token
    string _tokenValue;    // 错误发生时 token的内容
    string _message;             // 错误的提示信息
    TreeType _treeType;         // 发生错误时的树节点类型


public:

    ROProperty<size_t> curLineNum = ROProperty<size_t>(_curLineNum);         // 错误发生时的上一行
    ROProperty<size_t> preLineNum = ROProperty<size_t>(_preLineNum);         // 错误发生时的当前行
    ROProperty<size_t> columnNum = ROProperty<size_t>(_columnNum);          // 错误发生时token在行内的位置
    ROProperty<size_t> curIndex = ROProperty<size_t>(_curIndex);        // 错误发生时reader的指针位置
    ROProperty<string> tokenValue = ROProperty<string>(_tokenValue);        // 错误发生时 token的内容
    ROProperty<string> message = ROProperty<string>(_message);                    // 错误的提示信息
    ROProperty<TreeType> treeType = ROProperty<TreeType>(_treeType);             // 发生错误时的树节点类型

    /*
     构造一个语法错误对象
     参数：reader：用于得到错误上下文信息，比如出错的行号等
     参数：message：错误的提示信息
     参数：treeType：错误发生的语法树位置
    */
    SyntaxError(shared_ptr<const TokenReader> reader, std::string message, TreeType treeType)
        : _curLineNum(reader->GetCurLineNum()),
        _preLineNum(reader->GetLastLineNum()),
        _curIndex(reader->Index.Get()),
        _message(message), 
        _treeType(treeType) {
    
        /* 提取token内容 */
        auto token = reader->SeekToken();
        if (token) {
            if (token->TokenType.Get() == TokenTypeEnum::INTEGER) {
                char temp[256];
                _itoa_s(token->NumValue.Get(), temp, 10);
                _tokenValue = temp;
            }
            else {
                _tokenValue = token->StrValue.Get();
            }
        }
        else {
            _tokenValue = "";
        }
        

        /* 提取列号内容 */
        if (token) {
            _columnNum = token->ColumnNum.Get();
        }
        else {
            _columnNum = 0;
        }
    }

    /* 
     将语法分析的错误对象转化为字符串
     异常：无
    */
    string toString() noexcept {
        
        char buffer[256];
        _itoa_s((int)_preLineNum, buffer, 10);
        string preLineNumStr = buffer;
        _itoa_s((int)_curLineNum, buffer, 10);
        string curLineNumStr = buffer;
        string lineNumStr = (_curLineNum == 0) ? preLineNumStr : curLineNumStr;
        _itoa_s((int)_columnNum, buffer, 10);
        string columnNumStr = buffer;

        stringstream strBuilder;

        /* AST(Expression), at line(1, 2), token(static): 识别xx错误 */
        strBuilder << "AST(" << _treeType.toString() << "), ";
        strBuilder << "at line(" << lineNumStr << ", " << columnNumStr << "), ";
        strBuilder << "token(" << _tokenValue << "): ";
        strBuilder << _message;

        return strBuilder.str();
    }

};

/* 语法错误对象 */
typedef shared_ptr<SyntaxError> SyntaxErrorPtr;