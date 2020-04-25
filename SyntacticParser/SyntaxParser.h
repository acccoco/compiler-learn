#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <stack>
#include <list>
#include "TokenReader.h"
#include "SyntaxTreeNode.h"
#include "SyntaxError.h"

/*
 语法分析器的实现
 根据输入的token，生活抽象语法树，输出语法错误
*/
class SyntaxParser
{
protected:
    typedef SyntaxTreeNode::Ptr SyntaxTreeNodePtr;
    friend class ParserTest;                            // 用于测试的友元类
    shared_ptr<TokenReader> _reader;                    // token的读取器
    SyntaxTreeNodePtr _root;                            // 分析结果，树根
    list<shared_ptr<SyntaxError>> _errorList;          // 分析结果，错误链表

    /*
     识别关键字或者符号
     如果传入的不是 keyword 或 symbol ，会识别失败
     如果不匹配，reader 的 index 会回退
    */
    bool _MatchKeywordOrSymbol(TokenTypeEnum tokenType, string str) {
        // 判断类型是否合格
        if (tokenType != TokenTypeEnum::KEYWORD && tokenType != TokenTypeEnum::SYMBOL) {
            return false;
        }
        // 进行匹配
        ETokenPtr token = _reader->SeekToken();
        if (token->GetTokenType() == tokenType && token->GetStrValue() == str) {
            _reader->GetToken();
        }
        else {
            return false;
        }
    }


    /*
     识别Identifier
     如果失败，会回退，并且返回NULL
     成功返回树节点
    */
    SyntaxTreeNodePtr _MatchIdentifier() {
        ETokenPtr token;
        if ((token = _reader->SeekToken()) && token->GetTokenType() == TokenTypeEnum::IDENTIFIER) {
            token = _reader->GetToken();
            return SyntaxTreeNodePtr(new SyntaxTreeNode(token->GetStrValue(), token->GetLineNum()));
        }
        else {
            return NULL;
        }
    }
    /*
     识别IntgetLiteral
     如果失败，会回退，并且返回NULL
     成功返回树节点
    */
    SyntaxTreeNodePtr _MatchIntegerLiteral() {
        ETokenPtr token;
        if ((token = _reader->SeekToken()) && token->GetTokenType() == TokenTypeEnum::INTEGER) {
            token = _reader->GetToken();
            return SyntaxTreeNodePtr(new SyntaxTreeNode(token->GetNumValue(), token->GetLineNum()));
        }
        else {
            return NULL;
        }
    }

public:
#pragma region 构造函数，析构函数
    /*
     直接通过tokens的二维数组来初始化
    */
    SyntaxParser(vector<vector<shared_ptr<Token>>> tokens) {
        _reader = shared_ptr<TokenReader>(new TokenReader(tokens));
        if (_reader == NULL) {
            throw("Error: SyntaxParser Construction fail.");
        }
    }
    /*
     通过reader来初始化
    */
    SyntaxParser(shared_ptr<TokenReader> reader) {
        if (!reader) {
            throw("Error: SyntaxParser Construction fail.");
        }
        else {
            _reader = reader;
        }
    }
#pragma endregion

    /*
     语法分析主函数
    */
    void Parse() {
        // 清空结果
        _root = NULL;
        _errorList.clear();

        // 调用递归下降算法
        _root = Goal(_errorList);
    }
    /*
     获得语法分析树的树根
    */
    SyntaxTreeNodePtr GetRoot() {
        return _root;
    }
    /*
     获得错误链表
    */
    list<shared_ptr<SyntaxError>> GetErrorList() {
        return _errorList;
    }


protected:
    SyntaxTreeNodePtr Goal(list<shared_ptr<SyntaxError>>& errorList);
    SyntaxTreeNodePtr MainClass(list<shared_ptr<SyntaxError>>& errorList);
    SyntaxTreeNodePtr ClassDeclaration(list<shared_ptr<SyntaxError>>& errorList);
    SyntaxTreeNodePtr VarDeclaration(list<shared_ptr<SyntaxError>>& errorList);
    SyntaxTreeNodePtr MethodDeclaration(list<shared_ptr<SyntaxError>>& errorList);
    SyntaxTreeNodePtr Type(list<shared_ptr<SyntaxError>>& errorList);
    SyntaxTreeNodePtr Statement(list<shared_ptr<SyntaxError>>& errorList);
    SyntaxTreeNodePtr Expression(list<shared_ptr<SyntaxError>>& errorList);

#pragma region 识别Type的子过程
    shared_ptr<SyntaxTreeNode> Type_Identifier(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Type_boolean(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Type_Int(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Type_IntArray(list<shared_ptr<SyntaxError>>& errorList);
#pragma endregion

#pragma region 识别Statement的子过程
    shared_ptr<SyntaxTreeNode> Statement_Sequence(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Statement_while(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Statement_If(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Statement_Println(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Statement_Assign(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Statement_ArrayAssign(list<shared_ptr<SyntaxError>>& errorList);
#pragma endregion



#pragma region 识别Expression的子过程
    shared_ptr<SyntaxTreeNode> Expression_Identifier(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_IntegerLiteral(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_this(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_true(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_false(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_not(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_bracket(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_newIntArray(list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_newIdentifier(list<shared_ptr<SyntaxError>>& errorList);

    shared_ptr<SyntaxTreeNode> Expression_operate(shared_ptr<SyntaxTreeNode> firstNode, list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_length(shared_ptr<SyntaxTreeNode> firstNode, list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_arrayExpression(shared_ptr<SyntaxTreeNode> firstNode, list<shared_ptr<SyntaxError>>& errorList);
    shared_ptr<SyntaxTreeNode> Expression_functionCall(shared_ptr<SyntaxTreeNode> firstNode, list<shared_ptr<SyntaxError>>& errorList);
#pragma endregion


};

#pragma region 重复代码的宏定义
/*
 递归下降函数初始化的过程：
 - 清空 errorList
 - 创建 curIndex，用来缓存_reader的位置
 - 创建 tempTreeNode
 - 创建 tempErrorList
 - 创建 curTreeType
 - 判断是否存在第一个 token，如果不存在，就返回
 - 创建根节点 root
*/
#define RECURSIVE_DESCENT_INIT_RETURN(treeType)\
    errorList.clear();\
    const int curIndex = _reader->GetIndex();\
    SyntaxTreeNodePtr tempTreeNode;\
    list<shared_ptr<SyntaxError>> tempErrorList;\
    auto curTreeType = treeType;\
    ETokenPtr firstToken = _reader->SeekToken(); \
    if (firstToken == NULL) {\
        errorList.emplace_back(new SyntaxError(_reader, "识别开始token失败", treeType)); \
        return NULL; \
    }\
    shared_ptr<SyntaxTreeNode> root(new SyntaxTreeNode(treeType, firstToken->GetLineNum()));

/*
 递归下降，识别 Identifier 或 IntegerLiteral
 如果识别成功：
 - 将树节点设为指定节点的child
 如果识别失败：
 - 在 errorList 后追加错误
 - 指针回退
 - 返回
*/
#define SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, message)\
    if (tempTreeNode) {\
        root->SetChild(tempTreeNode);\
    }\
    else {\
        errorList.emplace_back(new SyntaxError(_reader, message, curTreeType));\
        _reader->SetIndex(curIndex);\
        return NULL;\
    }

/*
 递归下降，识别 Identifier 或 IntegerLiteral
 如果识别成功：
 - 将树节点设为指定节点的subling
 如果识别失败：
 - 在 errorList 后追加错误
 - 指针回退
 - 返回
*/
#define SET_IDEN_OR_INT_SUBLING_RETURN(tempTreeNode, root, curTreeType, message)\
    if (tempTreeNode) {\
        root->SetSubling(tempTreeNode);\
    }\
    else {\
        errorList.emplace_back(new SyntaxError(_reader, message, curTreeType));\
        errorList.insert(errorList.end(), tempErrorList.begin(), tempErrorList.end());\
        _reader->SetIndex(curIndex);\
        return NULL;\
    }

/*
 递归下降，识别 子树
 如果识别成功：
 - 将子树节点设为指定节点的subling
 如果识别失败：
 - 在 errorList 后追加错误
 - 在errorList后追加子树的错误
 - 指针回退
 - 返回
*/
#define SET_SUBTREE_SUBLING_RETURN(tempTreeNode, root, curTreeType, message)\
    if (tempTreeNode) {\
        root->SetSubling(tempTreeNode);\
    }\
    else {\
        errorList.emplace_back(new SyntaxError(_reader, message, curTreeType));\
        errorList.insert(errorList.end(), tempErrorList.begin(), tempErrorList.end());\
        _reader->SetIndex(curIndex);\
        return NULL;\
    }

/*
 递归下降，识别 子树
 如果识别成功：
 - 将子树节点设为指定节点的child
 如果识别失败：
 - 在 errorList 后追加错误
 - 在errorList后追加子树的错误
 - 指针回退
 - 返回
*/
#define SET_SUBTREE_CHILD_RETURN(tempTreeNode, root, curTreeType, message)\
    if (tempTreeNode) {\
        root->SetChild(tempTreeNode);\
    }\
    else {\
        errorList.emplace_back(new SyntaxError(_reader, message, curTreeType));\
        errorList.insert(errorList.end(), tempErrorList.begin(), tempErrorList.end());\
        _reader->SetIndex(curIndex);\
        return NULL;\
    }

/*
 递归下降过程识别 keyword 或 symbol 的过程，如果无法识别，就：
 -  在errorList 后追加错误
 - 指针回退
 - 返回
*/
#define PROCESS_STR_RETURN(tokenType, tokenStr, curTreeType, message)\
    if (_MatchKeywordOrSymbol(tokenType, tokenStr) == false) {\
        errorList.emplace_back(new SyntaxError(_reader, message, curTreeType));\
        _reader->SetIndex(curIndex);\
        return NULL;\
    }
#pragma endregion