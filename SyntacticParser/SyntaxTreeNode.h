#pragma once
#include <string>
#include <memory>
#include "SyntaxTreeType.h"

using namespace std;


/*
 语法树的树节点
*/
class SyntaxTreeNode {
public:
    typedef std::shared_ptr<SyntaxTreeNode> Ptr;
private:
    Ptr _child;                     // 子节点
    Ptr _subling;                   // 兄弟节点
    int _lineNum;                   // 行号
    TreeType _treeType;
    int _numValue;                  // 数字值
    std::string _strValue;          // 字符串值

public:

#pragma region 属性
    Property<string> StrValue = Property<string>(_strValue);
    Property<int> NumValue = Property<int>(_numValue);
    Property<int> LineNum = Property<int>(_lineNum);
    Property<Ptr> Child = Property<Ptr>(_child);
    Property<Ptr> Subling = Property<Ptr>(_subling);
    Property<TreeType> Type = Property<TreeType>(_treeType);

#pragma endregion

#pragma region 构造函数

    /* 一般构造函数 */
    SyntaxTreeNode(TreeType treeType, int lineNum, string strValue = "", int numValue = 0)
        : _treeType(treeType), _lineNum(lineNum), _strValue(strValue), _numValue(numValue) {}

    /* 使用token构造，只能使用Identifer或IntegerLiteral的token构造此对象 */
    SyntaxTreeNode(ETokenPtr token)
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

typedef shared_ptr<SyntaxTreeNode> SyntaxTreeNodePtr;