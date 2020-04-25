#pragma once
#include <string>
#include "SyntaxTreeType.h"
#include "TokenReader.h"

/*
 语法错误的定义
*/
class SyntaxError {
private:
    int _index;                             // 出错的token在reader中的位置。出现错误时，使用GetIndex()传递该参数
    std::string _message;                   // 错误的信息
    TreeNodeMainTypeEnum _treeMainType;     // 正位于哪个语法树下
    union {
        TreeNodeSubTypeEnum_Expression _treeSubType_Expression;
        TreeNodeSubTypeEnum_Statement _treeSubType_Statement;
        TreeNodeSubTypeEnum_Type _treeSubType_Type;
    };
    void _InitAssist(shared_ptr<const TokenReader> reader, std::string message, TreeNodeMainTypeEnum treeMainType) {
        _index = reader->GetIndex();
        _message = message;
        _treeMainType = treeMainType;
    }

public:
    /*
     构造函数
     仅设置出现错误的语法树的主类型
    */
    SyntaxError(shared_ptr<const TokenReader> reader, std::string message, TreeNodeMainTypeEnum treeMainType) {
        _InitAssist(reader, message, treeMainType);
    }
    /*
     构造函数
     设置出现错误的语法树的主类型和子类型
    */
    SyntaxError(shared_ptr<const TokenReader> reader, std::string message, TreeNodeSubTypeEnum_Expression treeSubType_Expression) {
        _InitAssist(reader, message, TreeNodeMainTypeEnum::Expression);
        _treeSubType_Expression = treeSubType_Expression;
    }
    /*
     构造函数
     设置出现错误的语法树的主类型和子类型
    */
    SyntaxError(shared_ptr<const TokenReader> reader, std::string message, TreeNodeSubTypeEnum_Statement treeSubType_Statement) {
        _InitAssist(reader, message, TreeNodeMainTypeEnum::Statement);
        _treeSubType_Statement = treeSubType_Statement;
    }
    /*
     构造函数
     设置出现错误的语法树的主类型和子类型
    */
    SyntaxError(shared_ptr<const TokenReader> reader, std::string message, TreeNodeSubTypeEnum_Type treeSubType_Type) {
        _InitAssist(reader, message, TreeNodeMainTypeEnum::Type);
        _treeSubType_Type = treeSubType_Type;
    }
};