#pragma once

/*
 树节点的类型
 对于Expresison，Statement，Type这三种类型，需要看相应的子类型才完整
*/
class TreeType {
public:
#pragma region 节点类型枚举

    /* 语法分析树节点的主类型 */
    enum class MainType {

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
    enum class SubType_Expression {
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
    enum class SubType_Statement {
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
    enum class SubType_Type {
        Default,        // 没有子类型，只有主类型
        /*------对应相应的产生式-----*/
        Type_boolean,
        Type_Int,
        Type_IntArray,
        Type_Identifier,
    };
#pragma endregion

#pragma region 读取节点信息的方法

    /* 获得节点的主要类型  */
    MainType GetMainType() { return _mainType; }

    /* 对于Expression类型的节点，获得其子类型 */
    SubType_Expression GetSubType_Expression() { return _subType_Expression; }

    /* 对于Statement类型的节点，获得其子类型 */
    SubType_Statement GetSubType_Statement() { return _subType_Statement; }

    /* 对于Type类型的节点，获得其子类型 */
    SubType_Type GetSubType_Type() { return _subType_Type; }

#pragma endregion

#pragma region 构造函数

    /* 主类型为默认类型(Default)的类型对象，子类型全默认 */
    TreeType()
        : _mainType(MainType::Default) {}

    /* 主类型为指定类型的类型对象，子类型全默认 */
    TreeType(MainType mainType)
        :_mainType(mainType) {}

    /* 主类型为 Expression 类型，子类型指定 */
    TreeType(SubType_Expression subType)
        : _mainType(MainType::Expression), _subType_Expression(subType) {}

    /* 主类型为 Statement 类型，子类型指定 */
    TreeType(SubType_Statement subType)
        : _mainType(MainType::Statement), _subType_Statement(subType) {}

    /* 主类型为 Type 类型，子类型指定 */
    TreeType(SubType_Type subType)
        : _mainType(MainType::Type), _subType_Type(subType) {}

#pragma endregion

protected:
    MainType _mainType = MainType::Default;                         // 节点的主类型
    SubType_Expression _subType_Expression = SubType_Expression::Default;   // Expression主类型配套的子类型
    SubType_Type _subType_Type = SubType_Type::Default;         // Type主类型配套的子类型
    SubType_Statement _subType_Statement = SubType_Statement::Default;      // Statement主类型配套的子类型
};

/* 主要类型 */
typedef TreeType::MainType TreeMainType;

/* Expression 主要类型的，配套的子类型 */
typedef TreeType::SubType_Expression TreeSubType_Expression;

/* Statement 主要类型的，配套的子类型 */
typedef TreeType::SubType_Statement TreeSubType_Statement;

/* Type 主要类型的，配套的子类型 */
typedef TreeType::SubType_Type TreeSybType_Type;