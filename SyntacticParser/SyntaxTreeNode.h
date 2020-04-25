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
    Ptr _child;         // 子节点
    Ptr _subling;       // 兄弟节点
    int _lineNum;       // 行号

    TreeNodeMainTypeEnum _mainType;
    union {
        TreeNodeSubTypeEnum_Expression _subType_Expression;
        TreeNodeSubTypeEnum_Statement _subType_Statement;
        TreeNodeSubTypeEnum_Type _subType_Type;
    };

    int _numValue;              // 数字值
    std::string _strValue;      // 字符串值

    /*
     用于辅助构造函数的初始化方法
    */
    void _InitAssist(TreeNodeMainTypeEnum mainType, int lineNum, Ptr child, Ptr subling, int numValue, string strValue) {
        _mainType = mainType;
        _numValue = numValue;
        _strValue = strValue;
        _lineNum = lineNum;
        _child = child;
        _subling = subling;
    }
public:
    
    /*
     构造函数：初始化一个Expression-树节点
    */
    SyntaxTreeNode(TreeNodeSubTypeEnum_Expression subKind, int lineNum, Ptr child = NULL, Ptr subling = NULL) {
        _InitAssist(TreeNodeMainTypeEnum::Expression, lineNum, child, subling, 0, "");
        _subType_Expression = subKind;
    }
    /*
     构造函数：初始化一个Statement-树节点
    */
    SyntaxTreeNode(TreeNodeSubTypeEnum_Statement subKind, int lineNum, Ptr child = NULL, Ptr subling = NULL) {
        _InitAssist(TreeNodeMainTypeEnum::Statement, lineNum, child, subling, 0, "");
        _subType_Statement = subKind;
    }
    /*
     构造函数：初始化一个Type-树节点
    */
    SyntaxTreeNode(TreeNodeSubTypeEnum_Type subKind, int lineNum, Ptr child = NULL, Ptr subling = NULL) {
        _InitAssist(TreeNodeMainTypeEnum::Type, lineNum, child, subling, 0, "");
        _subType_Type = subKind;
    }
    /*
     构造函数：初始化一个Identifier-树节点
    */
    SyntaxTreeNode(string strValue, int lineNum, Ptr child = NULL, Ptr subling = NULL) {
        _InitAssist(TreeNodeMainTypeEnum::Identifier, lineNum, child, subling, 0, strValue);
    }
    /*
     构造函数：初始化一个IntegerLiteral-树节点
    */
    SyntaxTreeNode(int numValue, int lineNum, Ptr child = NULL, Ptr subling = NULL) {
        _InitAssist(TreeNodeMainTypeEnum::IntegerLeterial, lineNum, child, subling, numValue, "");
    }
    /*
     构造函数：只设置mainType
     Default，Goal, MainClass, ClassDeclaration, VarDeclaration, MethodDeclaration,
    */
    SyntaxTreeNode(TreeNodeMainTypeEnum mainKind, int lineNum, Ptr child = NULL, Ptr subling = NULL) {
        _InitAssist(mainKind, lineNum, child, subling, 0, "");
    }
    /*
     获得节点的值
    */
    string GetStrValue() {
        return _strValue;
    }
    /*
     获得节点的值
    */
    int GetNumValue() {
        return _numValue;
    }
    /*
     获得节点的行号
    */
    int GetLineNum() {
        return _lineNum;
    }

    /*
     设置孩子节点
    */
    void SetChild(Ptr child) {
        _child = child;
    }
    /*
     设置兄弟节点
    */
    void SetSubling(Ptr subling) {
        _subling = subling;
    }
    /*
     获得孩子节点
    */
    Ptr GetChild() {
        return _child;
    }
    /*
     获得兄弟节点
    */
    Ptr GetSubling() {
        return _subling;
    }
    /*
     获得类型
    */
    TreeNodeMainTypeEnum GetMainType() {
        return _mainType;
    }
    /*
     获得子类型：Expression
    */
    TreeNodeSubTypeEnum_Expression GetSubType_Expression() {
        return _subType_Expression;
    }
    /*
     获得子类型：Statement
    */
    TreeNodeSubTypeEnum_Statement GetSubType_Statement() {
        return _subType_Statement;
    }
    /*
     获得子类型：Type
    */
    TreeNodeSubTypeEnum_Type GetSubType_Type() {
        return _subType_Type;
    }
    /*
     设置子类型：Expression
    */
    void SetSubType(TreeNodeSubTypeEnum_Expression subType) {
        this->_subType_Expression = subType;
    }
    /*
     设置子类型：Statement
    */
    void SetSubType(TreeNodeSubTypeEnum_Statement subType) {
        this->_subType_Statement = subType;
    }
    /*
     设置子类型：Type
    */
    void SetSubType(TreeNodeSubTypeEnum_Type subType) {
        this->_subType_Type = subType;
    }
};

