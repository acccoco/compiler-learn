#pragma once
#include <string>
#include "SyntaxTreeType.h"
#include "TokenReader.h"

/*
 语法错误的定义
*/
class SyntaxError {
private:
    shared_ptr<const TokenReader> _reader;
    std::string _message;                       // 错误的信息
    TreeType _treeType;


public:
#pragma region 构造函数
    SyntaxError(shared_ptr<const TokenReader> reader, std::string message, TreeType treeType)
        : _reader(reader), _message(message), _treeType(treeType) {}
#pragma endregion

    
};
typedef shared_ptr<SyntaxError> SyntaxErrorPtr;