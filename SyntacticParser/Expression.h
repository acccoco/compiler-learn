#pragma once
#include "RDCase.h"


// 交叉依赖的前置声明
/*
 Expression -> ...
*/
class RDCase_Expression :public RDCase {
public:
    RDCase_Expression(shared_ptr<TokenReader> reader)noexcept;
protected:
    void _RunException(TreeNodePtr firstNode = nullptr)override;
};


/*
 Expression -> Identifier
*/
class RDCase_Expression_Identifier :public RDCase {
public:
    RDCase_Expression_Identifier(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {
        _Init(TreeType(TreeSubType_Expression::Exp_Identifier));

        /******************
         Identifier
        ********************/
        auto node = _CheckIdentifier();
        _root->Child.Set(node);

        return;
    }
};

/*
 Expression -> IntegerLiteral
*/
class RDCase_Expression_IntegerLiteral : public RDCase {
public:
    RDCase_Expression_IntegerLiteral(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {
        _Init(TreeType(TreeSubType_Expression::Exp_IntegerLiteral));

        /******************
         IntegerLiteral
        ********************/
        auto node = _CheckIntegerLiteral();
        _root->Child.Set(node);

        return;
    }
};

/*
 Expression -> "this"
*/
class RDCase_Expression_this : public RDCase {
public:
    RDCase_Expression_this(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {
        _Init(TreeSubType_Expression::Exp_This);

        /******************
         "this"
        ********************/
        _CheckKeyword("this");

        return;
    }
};
/*
 Expression -> "true"
*/
class RDCase_Expression_true : public RDCase {
public:
    RDCase_Expression_true(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {
        _Init(TreeSubType_Expression::Exp_True);

        /******************
         "true"
        ********************/
        _CheckKeyword("true");

        return;
    }
};
/*
 Expression -> "flase"
*/
class RDCase_Expression_false : public RDCase {
public:
    RDCase_Expression_false(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {
        _Init(TreeSubType_Expression::Exp_False);

        /******************
         "false"
        ********************/
        _CheckKeyword("false");

        return;
    }
};

/*
 Expression -> "!" Expression
*/
class RDCase_Expression_not : public RDCase {
public:
    RDCase_Expression_not(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {
        _Init(TreeSubType_Expression::Exp_Not);

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

        return;
    }
};

/*
 Expression -> "(" Expression ")"
*/
class RDCase_Expression_bracket :public RDCase {
public:
    RDCase_Expression_bracket(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)
    {

        _Init(TreeSubType_Expression::Exp_Bracket);
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

        return;
    }
};
/*
 Expression -> "new" "int" "[" Expression "]"
*/
class RDCase_Expression_newIntArray :public RDCase {
public:
    RDCase_Expression_newIntArray(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)
    {
        _Init(TreeSubType_Expression::Exp_NewIntArray);
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

        return;
    }
};
/*
 Expression -> "new" Identifier "(" ")"
*/
class RDCase_Expression_newIdentifier :public RDCase {
public:
    RDCase_Expression_newIdentifier(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)
    {
        _Init(TreeSubType_Expression::Exp_NewIdentifier);
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

        return;
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
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr) {
        _InitRecursive(TreeSubType_Expression::Exp_ArrayExpression, firstRoot);
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

        return;
    }
};
/*
 Expression ->  Expression ( "&&" | "<" | "+" | "-" | "*" ) Expression
*/
class RDCase_Expression_operate :public RDCase {
public:
    RDCase_Expression_operate(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr) {
        _InitRecursive(TreeSubType_Expression::Exp_Operate, firstRoot);
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
            auto error = SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
            _reader->RecoveryIndex(_curIndex);
            throw error;
        }
        _root->StrValue.Set(token->StrValue.Get());
        /******************
         Expression
        ********************/
        auto expression2 = RDCase_Expression(_reader);
        expression2.Run();
        _CheckSubTree(&expression2);
        _root->Child.Get()->Subling.Set(expression2.Root.Get());

        return;
    }
};
/*
 Expression -> Expression "." "length"
*/
class RDCase_Expression_length :public RDCase {
public:
    RDCase_Expression_length(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr) {
        _InitRecursive(TreeSubType_Expression::Exp_Length, firstRoot);
        /******************
         Expression
        ********************/
        _root->Child.Set(firstRoot);
        /******************
         "." "length"
        ********************/
        _CheckSymbol(".");
        _CheckKeyword("length");

        return;
    }
};
/*
 Expression "." Identifier "(" [ Expression { "," Expression } ] ")"
*/
class RDCase_Expression_functionCall :public RDCase {
public:
    RDCase_Expression_functionCall(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr) {
        _InitRecursive(TreeSubType_Expression::Exp_FunctionCall, firstRoot);
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
        expression2.Run();
        if (expression2.Root.Get()) {

            // 参数列表节点
            TreeNodePtr paramSequence(new TreeNode(TreeType(), expression2.Root.Get()->LineNum.Get()));
            paramSequence->Child.Set(expression2.Root.Get());
            _root->Child.Get()->Subling.Get()->Subling.Set(paramSequence);
            TreeNodePtr pre = expression2.Root.Get();
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
                    _reader->RecoveryIndex(curIndex);
                    break;
                }
            }
        }
        /******************
         ")"
        ********************/
        _CheckSymbol(")");

        return;
    }
};
#pragma endregion

/*
 Expression 的定义
*/
inline RDCase_Expression::RDCase_Expression(shared_ptr<TokenReader> reader) noexcept :
    RDCase(reader) {}
inline void RDCase_Expression::_RunException(TreeNodePtr firstNode) {
    _Init(TreeType(TreeMainType::Expression));

    /* 首先使用以终结符开头的产生式进行识别 */
    auto expression_this = RDCase_Expression_this(_reader);
    auto expression_true = RDCase_Expression_true(_reader);
    auto expression_false = RDCase_Expression_false(_reader);
    auto expression_bracket = RDCase_Expression_bracket(_reader);
    auto expression_not = RDCase_Expression_not(_reader);
    auto expression_newIdentifier = RDCase_Expression_newIdentifier(_reader);
    auto expression_newIntArray = RDCase_Expression_newIntArray(_reader);
    auto expression_Identifier = RDCase_Expression_Identifier(_reader);
    auto expression_IntegerLiteral = RDCase_Expression_IntegerLiteral(_reader);
    RUN_ONCE() {
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

    /* 以非终结符开头的产生式识别失败，抛出异常 */
    if (_root == nullptr) {
        string msg = "识别Expression错误";
        auto error = SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
        _reader->RecoveryIndex(_curIndex);
        throw error;
    }

    /* 非终结符开头产生式成功了，尝试左递归产生式 */
    while (true) {
        TreeNodePtr newRoot;
        auto expression_operate = RDCase_Expression_operate(_reader);
        auto expression_arrayExpression = RDCase_Expression_arrayExpression(_reader);
        auto expression_length = RDCase_Expression_length(_reader);
        auto expression_functionCall = RDCase_Expression_functionCall(_reader);
        RUN_ONCE() {
            if (newRoot = expression_operate.Run(_root)) break;
            if (newRoot = expression_arrayExpression.Run(_root)) break;
            if (newRoot = expression_length.Run(_root)) break;
            if (newRoot = expression_functionCall.Run(_root)) break;
            return;
        }
        _root = newRoot;

    }
}
