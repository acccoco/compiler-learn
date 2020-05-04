#pragma once


class TreeType {
public:
#pragma region 节点类型枚举

    /*
     树节点类型的定义
    */
    enum class MainType {
        // 语法节点
        Goal,
        MainClass,
        ClassDeclaration,
        VarDeclaration,
        MethodDeclaration,
        Type,
        Statement,
        Expression,
        // 词法节点
        Identifier,
        IntegerLeterial,
        // 特殊的节点
        Default,
    };



    /*
     语法树-Expression-的子类型
    */
    enum class SubType_Expression {
        Exp_Bracket,    // Expression -> "(" Expression ")"
        Exp_Not,    // Expression -> "!" Expression
        Exp_NewIdentifier,  // Expression -> "new" Identifier "(" ")"
        Exp_NewIntArray,    // Expression -> "new" "int" "[" Expression "]"
        Exp_True,   // Expression -> "true"
        Exp_False,  // Expression -> "false"
        Exp_This,   // Expression -> "this"
        Exp_Identifier, // Expression -> Identifier
        Exp_IntegerLiteral, // Expression -> IntegerLiteral

        Exp_ArrayExpression,    // Expression -> Expression "[" Expression "]"
        Exp_Length, // Expression -> Expression "." "length"
        Exp_FunctionCall,   // Expression "." Identifier "(" [ Expression { "," Expression } ] ")"
        Exp_Operate,    // Expression ->  Expression ( "&&" | "<" | "+" | "-" | "*" ) Expression
    };

    /*
     语法树-Statement-的子类型
    */
    enum class SubType_Statement {
        Statement_Sequence,
        Statement_if,
        Statement_while,
        Statement_println,
        Statement_assign,
        Statement_arrayAssign,
    };
    /*
     语法树-Type-的子类型
    */
    enum class SubType_Type {
        Type_boolean,
        Type_Int,
        Type_IntArray,
        Type_Identifier,
    };
#pragma endregion

    MainType GetMainType() { return _mainType; }
    SubType_Expression GetSubType_Expression() { return _subType_Expression; }
    SubType_Statement GetSubType_Statement() { return _subType_Statement; }
    SubType_Type GetSubType_Type() { return _subType_Type; }
    
    TreeType()
        : _mainType(MainType::Default) {}
    TreeType(MainType mainType)
        :_mainType(mainType) {}
    TreeType(SubType_Expression subType)
        : _mainType(MainType::Expression), _subType_Expression(subType) {}
    TreeType(SubType_Statement subType)
        : _mainType(MainType::Statement), _subType_Statement(subType) {}
    TreeType(SubType_Type subType)
        : _mainType(MainType::Type), _subType_Type(subType) {}

protected:
    MainType _mainType = MainType::Default;                         // 节点的主要类型
    SubType_Expression _subType_Expression = SubType_Expression::Exp_ArrayExpression;       /*************/
    SubType_Type _subType_Type = SubType_Type::Type_boolean;                                /* 如果主要类型不是Default，那么这三个类型三选一 */
    SubType_Statement _subType_Statement = SubType_Statement::Statement_arrayAssign;        /************* */
};
typedef TreeType::MainType TreeMainType;
typedef TreeType::SubType_Expression TreeSubType_Expression;
typedef TreeType::SubType_Statement TreeSubType_Statement;
typedef TreeType::SubType_Type TreeSybType_Type;