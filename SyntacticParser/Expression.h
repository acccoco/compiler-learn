#pragma once
#include "RDCase.h"

//class RDCase_Expression_this;
//class RDCase_Expression_true;
//class RDCase_Expression_false;
//class RDCase_Expression_bracket;
//class RDCase_Expression_not;
//class RDCase_Expression_newIdentifier;
//class RDCase_Expression_newIntArray;
//class RDCase_Expression_Identifier;
//class RDCase_Expression_IntegerLiteral;
//
//class RDCase_Expression_operate;
//class RDCase_Expression_arrayExpression;
//class RDCase_RDCase_Expression_length;
//class RDCase_Expression_functionCall;

// 交叉依赖的前置声明
/*
 Expression -> ...
*/
class RDCase_Expression :public RDCase {
public:
    RDCase_Expression(shared_ptr<TokenReader> reader)noexcept;
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstNode = nullptr)override;
};


/*
 Expression -> Identifier
*/
class RDCase_Expression_Identifier :public RDCase {
public:
    RDCase_Expression_Identifier(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)override {
        _InitT(TreeType(TreeSubType_Expression::Exp_Identifier));

        /******************
         Identifier
        ********************/
        auto node = _CheckIdentifier();
        _root->Child.Set(node);

        return _root;
    }
};

/*
 Expression -> IntegerLiteral
*/
class RDCase_Expression_IntegerLiteral : public RDCase {
public:
    RDCase_Expression_IntegerLiteral(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)override {
        _InitT(TreeType(TreeSubType_Expression::Exp_IntegerLiteral));

        /******************
         IntegerLiteral
        ********************/
        auto node = _CheckIntegerLiteral();
        _root->Child.Set(node);

        return _root;
    }
};

/*
 Expression -> "this"
*/
class RDCase_Expression_this : public RDCase {
public:
    RDCase_Expression_this(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)override {
        _InitT(TreeSubType_Expression::Exp_This);

        /******************
         "this"
        ********************/
        _CheckKeyword("this");

        return _root;
    }
};
/*
 Expression -> "true"
*/
class RDCase_Expression_true : public RDCase {
public:
    RDCase_Expression_true(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)override {
        _InitT(TreeSubType_Expression::Exp_True);

        /******************
         "true"
        ********************/
        _CheckKeyword("true");

        return _root;
    }
};
/*
 Expression -> "flase"
*/
class RDCase_Expression_false : public RDCase {
public:
    RDCase_Expression_false(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)override {
        _InitT(TreeSubType_Expression::Exp_False);

        /******************
         "false"
        ********************/
        _CheckKeyword("false");

        return _root;
    }
};

/*
 Expression -> "!" Expression
*/
class RDCase_Expression_not : public RDCase {
public:
    RDCase_Expression_not(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)override {
        _InitT(TreeType::SubType_Expression::Exp_Not);

        /******************
         "!"
        ********************/
        _CheckSymbol("!");
        /******************
         Expression
        ********************/
        auto expression = RDCase_Expression(_reader);
        expression.Run();
        _CheckSubTree(&expression);
        _root->Child.Set(expression.Root.Get());

        return _root;
    }
};

/*
 Expression -> "(" Expression ")"
*/
class RDCase_Expression_bracket :public RDCase {
public:
    RDCase_Expression_bracket(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)
    {

        _InitT(TreeType::SubType_Expression::Exp_Bracket);
        /******************
         "("
        ********************/
        _CheckSymbol("(");
        /******************
         Expression
        ********************/
        auto expression = RDCase_Expression(_reader);
        expression.Run();
        _CheckSubTree(&expression);
        _root->Child.Set(expression.Root.Get());
        /******************
         ")"
        ********************/
        _CheckSymbol(")");

        return _root;
    }
};
/*
 Expression -> "new" "int" "[" Expression "]"
*/
class RDCase_Expression_newIntArray :public RDCase {
public:
    RDCase_Expression_newIntArray(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)
    {
        _InitT(TreeType::SubType_Expression::Exp_NewIntArray);
        /******************
         "new" "int" "["
        ********************/
        _CheckKeyword("new");
        _CheckKeyword("int");
        _CheckSymbol("[");
        /******************
         Expression
        ********************/
        auto expression = RDCase_Expression(_reader);
        expression.Run();
        _CheckSubTree(&expression);
        _root->Child.Set(expression.Root.Get());
        /******************
         "]"
        ********************/
        _CheckSymbol("]");

        return _root;
    }
};
/*
 Expression -> "new" Identifier "(" ")"
*/
class RDCase_Expression_newIdentifier :public RDCase {
public:
    RDCase_Expression_newIdentifier(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)
    {
        _InitT(TreeType::SubType_Expression::Exp_NewIdentifier);
        /******************
         "new"
        ********************/
        _CheckKeyword("new");
        /******************
         Identifier
        ********************/
        auto node = _CheckIdentifier();
        _root->Child.Set(node);
        /******************
         "(" ")"
        ********************/
        _CheckSymbol("(");
        _CheckSymbol(")");

        return _root;
    }
};


#pragma region 以非终结符开头的产生式
/*
 Expression -> Expression "[" Expression "]"
*/
class RDCase_Expression_arrayExpression :public RDCase {
public:
    RDCase_Expression_arrayExpression(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr) {
        _InitNT(TreeType::SubType_Expression::Exp_ArrayExpression, firstRoot);
        /******************
         Expression
        ********************/
        _root->Child.Set(firstRoot);
        /******************
         "["
        ********************/
        _CheckSymbol("[");
        /******************
         Expression
        ********************/
        auto expression2 = RDCase_Expression(_reader);
        expression2.Run();
        _CheckSubTree(&expression2);
        _root->Child.Get()->Subling.Set(expression2.Root.Get());
        /******************
         "]"
        ********************/
        _CheckSymbol("]");

        return _root;
    }
};
/*
 Expression ->  Expression ( "&&" | "<" | "+" | "-" | "*" ) Expression
*/
class RDCase_Expression_operate :public RDCase {
public:
    RDCase_Expression_operate(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr) {
        _InitNT(TreeType::SubType_Expression::Exp_Operate, firstRoot);
        /******************
         Expression
        ********************/
        _root->Child.Set(firstRoot);
        /******************
         ( "&&" | "<" | "+" | "-" | "*" )
         将符号值放入根节点的 strValue 域中
        ********************/
        auto token = _reader->SeekToken();
        if (!_MatchSymbol("&&")
            && !_MatchSymbol("<")
            && !_MatchSymbol("+")
            && !_MatchSymbol("-")
            && !_MatchSymbol("*")) {
            string msg = "识别 &&, <, +, -, * 均失败";
            _reader->Index.Set(_curIndex);
            throw SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
        }
        _root->StrValue.Set(token->StrValue.Get());
        /******************
         Expression
        ********************/
        auto expression2 = RDCase_Expression(_reader);
        expression2.Run();
        _CheckSubTree(&expression2);
        _root->Child.Get()->Subling.Set(expression2.Root.Get());

        return _root;
    }
};
/*
 Expression -> Expression "." "length"
*/
class RDCase_Expression_length :public RDCase {
public:
    RDCase_Expression_length(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr) {
        _InitNT(TreeType::SubType_Expression::Exp_Length, firstRoot);
        /******************
         Expression
        ********************/
        _root->Child.Set(firstRoot);
        /******************
         "." "length"
        ********************/
        _CheckSymbol(".");
        _CheckKeyword("length");

        return _root;
    }
};
/*
 Expression "." Identifier "(" [ Expression { "," Expression } ] ")"
*/
class RDCase_Expression_functionCall :public RDCase {
public:
    RDCase_Expression_functionCall(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr) {
        _InitNT(TreeType::SubType_Expression::Exp_FunctionCall, firstRoot);
        /******************
         Expression
        ********************/
        _root->Child.Set(firstRoot);
        /******************
         "."
        ********************/
        _CheckSymbol(".");
        /******************
         Identifier
        ********************/
        auto node = _CheckIdentifier();
        _root->Child.Get()->Subling.Set(node);
        /******************
         "("
        ********************/
        _CheckSymbol("(");
        /******************
         [ Expression { "," Expression } ]
        ********************/
        auto expression2 = RDCase_Expression(_reader);
        if (expression2.Run()) {
            // 参数列表节点
            SyntaxTreeNodePtr paramSequence(new SyntaxTreeNode(TreeType(), expression2.Root.Get()->LineNum.Get()));
            paramSequence->Child.Set(expression2.Root.Get());
            _root->Child.Get()->Subling.Get()->Subling.Set(paramSequence);
            SyntaxTreeNodePtr pre = expression2.Root.Get();
            while (true) {
                // 记录位置，方便回溯
                const int curIndex = _reader->Index.Get();
                if (!_MatchSymbol(",")) break;
                auto expression3 = RDCase_Expression(_reader);
                expression3.Run();
                if (expression3.Root.Get()) {
                    pre->Subling.Set(expression3.Root.Get());
                    pre = expression3.Root.Get();
                }
                else {  // 这一部分识别失败，回溯
                    _reader->Index.Set(curIndex);
                    break;
                }
            }
        }
        /******************
         ")"
        ********************/
        _CheckSymbol(")");

        return _root;
    }
};
#pragma endregion

/*
 Expression 的定义
*/
inline RDCase_Expression::RDCase_Expression(shared_ptr<TokenReader> reader) noexcept :
    RDCase(reader) {}
inline SyntaxTreeNodePtr RDCase_Expression::RunException(SyntaxTreeNodePtr firstNode) {
    _InitT(TreeType(TreeMainType::Expression));

    // 首先使用以终结符开头的产生式进行识别
    auto expression_this = RDCase_Expression_this(_reader);
    auto expression_true = RDCase_Expression_true(_reader);
    auto expression_false = RDCase_Expression_false(_reader);
    auto expression_bracket = RDCase_Expression_bracket(_reader);
    auto expression_not = RDCase_Expression_not(_reader);
    auto expression_newIdentifier = RDCase_Expression_newIdentifier(_reader);
    auto expression_newIntArray = RDCase_Expression_newIntArray(_reader);
    auto expression_Identifier = RDCase_Expression_Identifier(_reader);
    auto expression_IntegerLiteral = RDCase_Expression_IntegerLiteral(_reader);

    for (int once = 0; once < 1; ++once) {  // 只会被执行一次，可以使用break跳出
        if (_root = expression_this.Run()) break;
        if (_root = expression_true.Run()) break;
        if (_root = expression_false.Run()) break;
        if (_root = expression_bracket.Run()) break;
        if (_root = expression_not.Run()) break;
        if (_root = expression_newIdentifier.Run()) break;
        if (_root = expression_newIntArray.Run()) break;
        if (_root = expression_Identifier.Run()) break;
        if (_root = expression_IntegerLiteral.Run()) break;
    }
    if (_root == nullptr) {
        _reader->Index.Set(_curIndex);
        string msg = "识别Expression错误";
        throw SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
    }
    /*if (!(_root = expression_this.Run())
        && !(_root = expression_true.Run())
        && !(_root = expression_false.Run())
        && !(_root = expression_bracket.Run())
        && !(_root = expression_not.Run())
        && !(_root = expression_newIdentifier.Run())
        && !(_root = expression_newIntArray.Run())
        && !(_root = expression_Identifier.Run())
        && !(_root = expression_IntegerLiteral.Run())) {

        string msg = "识别Expression错误";
        _errors.emplace_back(new SyntaxError(_reader, msg, _treeType));
        _reader->Index.Set(_curIndex);
        return nullptr;
    }*/

    // 然后尝试以非终结符开头的产生式进行识别，可能会多次匹配
    while (true) {
        SyntaxTreeNodePtr newRoot;
        auto expression_operate = RDCase_Expression_operate(_reader);
        auto expression_arrayExpression = RDCase_Expression_arrayExpression(_reader);
        auto expression_length = RDCase_Expression_length(_reader);
        auto expression_functionCall = RDCase_Expression_functionCall(_reader);
        for (int once = 0; once < 1; ++once) {      // 仅执行一次，可以使用break跳出
            if (newRoot = expression_operate.Run(_root)) break;
            if (newRoot = expression_arrayExpression.Run(_root)) break;
            if (newRoot = expression_length.Run(_root)) break;
            if (newRoot = expression_functionCall.Run(_root)) break;
            return _root;
        }
        _root = newRoot;
        /*if ((newRoot = expression_operate.Run(_root))
            || (newRoot = expression_arrayExpression.Run(_root))
            || (newRoot = expression_length.Run(_root))
            || (newRoot = expression_functionCall.Run(_root))) {
            _root = newRoot;
        }
        else {
            return _root;
        }*/
    }
}
