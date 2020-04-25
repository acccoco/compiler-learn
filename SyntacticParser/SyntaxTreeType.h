#pragma once
/*
 树节点类型的定义
*/
enum class TreeNodeMainTypeEnum {
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
enum class TreeNodeSubTypeEnum_Expression {
    Exp_Bracket,
    Exp_Not,
    Exp_NewIdentifier,
    Exp_NewIntArray,
    Exp_True,
    Exp_False,
    Exp_This,
    Exp_Identifier,
    Exp_IntegerLiteral,

    Exp_ArrayExpression,
    Exp_Length,
    Exp_FunctionCall,
    Exp_Operate,
};

/*
 语法树-Statement-的子类型
*/
enum class TreeNodeSubTypeEnum_Statement {
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
enum class TreeNodeSubTypeEnum_Type {
    Type_boolean,
    Type_Int,
    Type_IntArray,
    Type_Identifier,
};


///*
// 语法树节点的类型
//*/
//class TreeNodeType {
//protected:
//    /*
//     让外界无法使用
//    */
//    TreeNodeType() {
//        mainKind = TreeNodeMainTypeEnum::Default;
//    }
//public:
//    TreeNodeMainTypeEnum mainKind;
//};
///*
// Expression-树节点的类型
//*/
//class TreeNodeType_Expression :public TreeNodeType {
//public:
//    TreeNodeSubTypeEnum_Expression subKind;
//    TreeNodeType_Expression(TreeNodeSubTypeEnum_Expression subKind) {
//        mainKind = TreeNodeMainTypeEnum::Expression;
//        this->subKind = subKind;
//    }
//};
///*
// Statement-树节点的类型
//*/
//class TreeNodeType_Statement :public TreeNodeType {
//public:
//    TreeNodeSubTypeEnum_Statement subKind;
//    TreeNodeType_Statement(TreeNodeSubTypeEnum_Statement subKind) {
//        mainKind = TreeNodeMainTypeEnum::Statement;
//        this->subKind = subKind;
//    }
//};
///*
// Type-树节点的类型
//*/
//class TreeNodeType_Type :public TreeNodeType {
//public:
//    TreeNodeSubTypeEnum_Type subKind;
//    TreeNodeType_Type(TreeNodeSubTypeEnum_Type subKind) {
//        mainKind = TreeNodeMainTypeEnum::Type;
//        this->subKind = subKind;
//    }
//};
///*
// 语法树节点的类型
// 适合没有子类型的节点
//*/
//class TreeNodeType_Alone :public TreeNodeType {
//public:
//    TreeNodeType_Alone(TreeNodeMainTypeEnum mainKind) {
//        this->mainKind = mainKind;
//    }
//};