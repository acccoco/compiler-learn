#pragma once
#include <map>
#include <string>
using namespace std;

#pragma region 树节点类型的枚举

/* 语法分析树节点的主类型 */
enum class TreeMainType {

    /*--------语法节点-------*/
    Goal,
    MainClass,
    ClassDeclaration,
    VarDeclaration,
    MethodDeclaration,
    Type,
    Statement,
    Expression,

    /*----------词法节点----------*/
    Identifier,         // 标识符，比如变量名称
    IntegerLeterial,    // 数字

    /*-----------特殊节点--------------*/
    Default,        // 用于组合多个节点
};

/* Expression主类型配套的子类型 */
enum class TreeSubType_Expression {
    Default,        // 没有子类型，只有主类型

    /*------对应以终结符开头的产生式-----*/
    Exp_Bracket,    // Expression -> "(" Expression ")"
    Exp_Not,    // Expression -> "!" Expression
    Exp_NewIdentifier,  // Expression -> "new" Identifier "(" ")"
    Exp_NewIntArray,    // Expression -> "new" "int" "[" Expression "]"
    Exp_True,   // Expression -> "true"
    Exp_False,  // Expression -> "false"
    Exp_This,   // Expression -> "this"
    Exp_Identifier, // Expression -> Identifier
    Exp_IntegerLiteral, // Expression -> IntegerLiteral

    /*------对应以非终结符开头的产生式-----*/
    Exp_ArrayExpression,    // Expression -> Expression "[" Expression "]"
    Exp_Length, // Expression -> Expression "." "length"
    Exp_FunctionCall,   // Expression "." Identifier "(" [ Expression { "," Expression } ] ")"
    Exp_Operate,    // Expression ->  Expression ( "&&" | "<" | "+" | "-" | "*" ) Expression
};

/* Statement主类型配套的子类型 */
enum class TreeSubType_Statement {
    Default,    // 没有子类型，只有主类型

    /*------对应相应的产生式-----*/
    Statement_Sequence,
    Statement_if,
    Statement_while,
    Statement_println,
    Statement_assign,
    Statement_arrayAssign,
};

/* Type主类型配套的子类型 */
enum class TreeSubType_Type {
    Default,        // 没有子类型，只有主类型
    /*------对应相应的产生式-----*/
    Type_boolean,
    Type_Int,
    Type_IntArray,
    Type_Identifier,
};
#pragma endregion



/*
 树类型的枚举常量说明字符串
*/
class TreeTypeConst {
public:

    /* MainType的常量字符串量，不添加任何修饰 */
    const map<TreeMainType, string> mainTypeStr = {
        {TreeMainType::Goal, "Goal"},
        {TreeMainType::MainClass, "MainClass"},
        {TreeMainType::ClassDeclaration, "ClassDeclaration"},
        {TreeMainType::VarDeclaration, "VarDeclaration"},
        {TreeMainType::MethodDeclaration, "MethodDeclaration"},
        {TreeMainType::Type, "Type"},
        {TreeMainType::Statement, "Statement"},
        {TreeMainType::Expression, "Expression"},

        {TreeMainType::Identifier, "Identifier"},
        {TreeMainType::IntegerLeterial, "integerLiterial"},

        {TreeMainType::Default, "Default"},
    };

    /* subType_Expression的常量字符串量，不添加任何修饰 */
    const map<TreeSubType_Expression, string> subType_ExpressionStr = {
        {TreeSubType_Expression::Default, "Expression-Default"},

        {TreeSubType_Expression::Exp_Bracket, "Expression-Bracket"},
        {TreeSubType_Expression::Exp_Not, "Expression-Not"},
        {TreeSubType_Expression::Exp_NewIdentifier, "Expression-NewIdentifier"},
        {TreeSubType_Expression::Exp_NewIntArray, "Expression-NewIntArray"},
        {TreeSubType_Expression::Exp_True, "Expression-True"},
        {TreeSubType_Expression::Exp_False, "Expression-False"},
        {TreeSubType_Expression::Exp_This, "Expression-This"},
        {TreeSubType_Expression::Exp_Identifier, "Expression-Identifier"},
        {TreeSubType_Expression::Exp_IntegerLiteral, "Expression-IntegerLiteral"},

        {TreeSubType_Expression::Exp_ArrayExpression, "Expression-ArrayExpression"},
        {TreeSubType_Expression::Exp_Length, "Expression-Length"},
        {TreeSubType_Expression::Exp_FunctionCall, "Expression-FunctionCall"},
        {TreeSubType_Expression::Exp_Operate, "Expression-Operate"},
    };

    /* subType_Statement的常量字符串量，不添加任何修饰 */
    const map<TreeSubType_Statement, string> subType_StatementStr{
        {TreeSubType_Statement::Default, "Statement-Default"},

        {TreeSubType_Statement::Statement_Sequence, "Statement-Sequence"},
        {TreeSubType_Statement::Statement_if, "Statement-if"},
        {TreeSubType_Statement::Statement_while, "Statement-while"},
        {TreeSubType_Statement::Statement_println, "Statement-println"},
        {TreeSubType_Statement::Statement_assign, "Statement-assign"},
        {TreeSubType_Statement::Statement_arrayAssign, "Statement-arrayAssign"},
    };

    /* SubType_Type的常量字符串量，不添加任何修饰 */
    const map<TreeSubType_Type, string> subType_TypeStr = {
        {TreeSubType_Type::Default, "Type-Default"},

        {TreeSubType_Type::Type_boolean, "Type-boolean"},
        {TreeSubType_Type::Type_Int, "Type-Int"},
        {TreeSubType_Type::Type_IntArray, "Type-IntArray"},
        {TreeSubType_Type::Type_Identifier, "Type-Identifier"},
    };
};

/*
 树节点的类型
 对于Expresison，Statement，Type这三种类型，需要看相应的子类型才完整
*/
class TreeType {
public:

    /*
     将该对象的信息以字符串的形式表现出来
     异常：无
    */
    string toString() noexcept {

        TreeTypeConst treeTypeConst;

        switch (_mainType) {
        case TreeMainType::Expression:
            return treeTypeConst.subType_ExpressionStr.at(_subType_Expression);
            break;
        case TreeMainType::Statement:
            return treeTypeConst.subType_StatementStr.at(_subType_Statement);
            break;
        case TreeMainType::Type:
            return treeTypeConst.subType_TypeStr.at(_subType_Type);
            break;
        default:
            return treeTypeConst.mainTypeStr.at(_mainType);
            break;
        }
    }

#pragma region 读取节点信息的方法

    /* 获得节点的主要类型  */
    TreeMainType GetMainType() const { return _mainType; }

    /* 对于Expression类型的节点，获得其子类型 */
    TreeSubType_Expression GetSubType_Expression() const { return _subType_Expression; }

    /* 对于Statement类型的节点，获得其子类型 */
    TreeSubType_Statement GetSubType_Statement() const { return _subType_Statement; }

    /* 对于Type类型的节点，获得其子类型 */
    TreeSubType_Type GetSubType_Type() const { return _subType_Type; }

#pragma endregion

#pragma region 构造函数

    /* 主类型为默认类型(Default)的类型对象，子类型全默认 */
    TreeType()
        : _mainType(TreeMainType::Default) {}

    /* 主类型为指定类型的类型对象，子类型全默认 */
    TreeType(TreeMainType mainType)
        :_mainType(mainType) {}

    /* 主类型为 Expression 类型，子类型指定 */
    TreeType(TreeSubType_Expression subType)
        : _mainType(TreeMainType::Expression), _subType_Expression(subType) {}

    /* 主类型为 Statement 类型，子类型指定 */
    TreeType(TreeSubType_Statement subType)
        : _mainType(TreeMainType::Statement), _subType_Statement(subType) {}

    /* 主类型为 Type 类型，子类型指定 */
    TreeType(TreeSubType_Type subType)
        : _mainType(TreeMainType::Type), _subType_Type(subType) {}

#pragma endregion

protected:
    TreeMainType _mainType = TreeMainType::Default;                         // 节点的主类型
    TreeSubType_Expression _subType_Expression = TreeSubType_Expression::Default;   // Expression主类型配套的子类型
    TreeSubType_Type _subType_Type = TreeSubType_Type::Default;         // Type主类型配套的子类型
    TreeSubType_Statement _subType_Statement = TreeSubType_Statement::Default;      // Statement主类型配套的子类型
};
