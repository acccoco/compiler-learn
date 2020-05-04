#pragma once
#include <string>
#include <memory>
#include "TreeType.h"

using namespace std;


/*
 语法树的树节点
 有兄弟节点的二叉树：
 node → subling
   ↓
 child
 
*/
class TreeNode {
public:

    /* 语法树的树节点 */
    typedef std::shared_ptr<TreeNode> Ptr;
private:

    int _lineNum;               // 树节点对应的行号
    TreeType _treeType;         // 树节点的类型
    int _numValue;              // 数字值，与字符串值二选一
    std::string _strValue;      // 字符串值，与数字值二选一

    Ptr _child;                     // 子节点
    Ptr _subling;                   // 兄弟节点

public:

#pragma region 属性

    Property<string> StrValue = Property<string>(_strValue);    // 树节点的字符串值
    Property<int> NumValue = Property<int>(_numValue);          // 树节点的数字值
    Property<int> LineNum = Property<int>(_lineNum);            // 树节点对应的行号
    Property<TreeType> Type = Property<TreeType>(_treeType);    // 树节点的类型

    Property<Ptr> Child = Property<Ptr>(_child);                // 树节点的孩子
    Property<Ptr> Subling = Property<Ptr>(_subling);            // 树节点的兄弟节点

#pragma endregion

#pragma region 构造函数

    /*
     构造一个树节点
     兄弟节点和子节点为空
     字符出值默认为空串，数字值默认为0
    */
    TreeNode(TreeType treeType, int lineNum, string strValue = "", int numValue = 0) noexcept
        : _treeType(treeType), _lineNum(lineNum), _strValue(strValue), _numValue(numValue) {}

    /* 
     构造一个树节点
     只能使用Identifer或IntegerLiteral的token构造此对象
     不是上述类型，会抛出string类型的异常
     */
    TreeNode(ETokenPtr token)
        : _lineNum(token->LineNum.Get()), _strValue(token->StrValue.Get()), _numValue(token->NumValue.Get()) {
        if (token->TokenType.Get() == TokenTypeEnum::IDENTIFIER)
            _treeType = TreeType(TreeMainType::Identifier);
        else if (token->TokenType.Get() == TokenTypeEnum::INTEGER)
            _treeType = TreeType(TreeMainType::IntegerLeterial);
        else
            throw string("不能使用Identifer或IntegerLiteral的token构造此对象");
    }
#pragma endregion

};

/* 语法分析树的树节点 */
typedef shared_ptr<TreeNode> TreeNodePtr;