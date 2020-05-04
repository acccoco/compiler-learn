#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <list>
#include "TokenReader.h"
#include "SyntaxTreeNode.h"
#include "SyntaxError.h"
#include "Goal.h"

/*
 语法分析器的实现
 根据输入的token，生活抽象语法树，输出语法错误
*/
class SyntaxParser {
protected:
    typedef SyntaxTreeNode::Ptr SyntaxTreeNodePtr;
    shared_ptr<TokenReader> _reader;                    // token的读取器
    SyntaxTreeNodePtr _root;                            // 分析结果，树根
    list<shared_ptr<SyntaxError>> _errorList;          // 分析结果，错误链表

public:

    ROProperty<list<shared_ptr<SyntaxError>>> ErrorList = ROProperty<list<shared_ptr<SyntaxError>>>(_errorList);
    ROProperty<SyntaxTreeNodePtr> Root = ROProperty<SyntaxTreeNodePtr>(_root);
#pragma region 构造函数，析构函数
    /*
     通过reader来初始化
    */
    SyntaxParser(shared_ptr<TokenReader> reader): _reader(reader) {
        if (reader = nullptr)
            throw string("reader为空");
    }
#pragma endregion

    /*
     进行递归下降语法分析
     结果存放在对象的属性中
    */
    void Parse() {
        auto goal = RDCase_Goal(_reader);
        goal.Run();
        _root = goal.Root.Get();
        _errorList = goal.Errors.Get();
    }
};

