#pragma once
#include <string>
#include "TreeType.h"
#include "TokenReader.h"

/*
 语法错误的定义
*/
class SyntaxError {
protected:
    shared_ptr<const TokenReader> _reader;      // TODO：不应该有这个，从reader中提取一点东西出来就好了
    std::string _message;             // 错误的提示信息
    TreeType _treeType;         // 发生错误时的树节点类型


public:

    /*
     构造一个语法错误对象
     参数：reader：用于得到错误上下文信息，比如出错的行号等
     参数：message：错误的提示信息
     参数：treeType：错误发生的语法树位置
    */
    SyntaxError(shared_ptr<const TokenReader> reader, std::string message, TreeType treeType)
        : _reader(reader), _message(message), _treeType(treeType) {}

};

/* 语法错误对象 */
typedef shared_ptr<SyntaxError> SyntaxErrorPtr;