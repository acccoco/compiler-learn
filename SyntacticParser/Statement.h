#pragma once
#include "RDCase.h"
#include "Expression.h"


/*
 Statement -> ...
 前置声明
*/
class RDCase_Statement :public RDCase {
public:
    RDCase_Statement(shared_ptr<TokenReader> reader) noexcept;
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override;
};

/*
 Statement -> "{" { Statement } "}"
*/
class RDCase_Statement_Sequence : public RDCase {
public:
    RDCase_Statement_Sequence(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {

        _Init(TreeSubType_Statement::Statement_Sequence);

        /**** "{" ****/
        _CheckSymbol("{");

        /**** { Statement } ****/
        TreeNodePtr statementSeqNode = _MatchCaseSequence<RDCase_Statement>();
        _root->Child.Set(statementSeqNode);

        /**** "}" ****/
        _CheckSymbol("}");
    }
};

/*
 Statement -> "while" "(" Expression ")" Statement
*/
class RDCase_Statement_While : public RDCase {
public:
    RDCase_Statement_While(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {

        _Init(TreeSubType_Statement::Statement_while);

        /**** "while" "(" ****/
        _CheckKeyword("while");
        _CheckSymbol("(");

        /**** Expression ****/
        auto expression1 = RDCase_Expression(_reader);
        expression1.Run();
        _CheckSubTree(&expression1);
        _root->Child.Set(expression1.Root.Get());

        /**** ")" ****/
        _CheckSymbol(")");

        /**** Statement ****/
        auto statement1 = RDCase_Statement(_reader);
        statement1.Run();
        _CheckSubTree(&statement1);
        _root->Child.Get()->Subling.Set(statement1.Root.Get());
    }
};

/*
 Statement -> "if" "(" Expression ")" Statement "else" Statement
*/
class RDCase_Statement_If : public RDCase {
public:
    RDCase_Statement_If(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {

        _Init(TreeSubType_Statement::Statement_if);

        /**** "if" "(" ****/
        _CheckKeyword("if");
        _CheckSymbol("(");

        /**** Expression ****/
        TreeNodePtr expression1 = _CheckRDCase<RDCase_Expression>(_reader);
        _root->Child.Set(expression1);

        /**** ")" ****/
        _CheckSymbol(")");

        /**** Statement ****/
        TreeNodePtr statement1 = _CheckRDCase<RDCase_Statement>(_reader);
        _root->Child.Get()->Subling.Set(statement1);

        /**** "else" ****/
        _CheckKeyword("else");

        /**** Statement ****/
        TreeNodePtr statement2 = _CheckRDCase<RDCase_Statement>(_reader);
        statement1->Subling.Set(statement2);
    }
};

/*
 Statement -> "System.out.println" "(" Expression ")" ";"
*/
class RDCase_Statement_Println : public RDCase {
public:
    RDCase_Statement_Println(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {

        _Init(TreeSubType_Statement::Statement_println);

        /**** "System.out.println" "(" ****/
        _CheckKeyword("System.out.println");
        _CheckSymbol("(");

        /**** Expression ****/
        TreeNodePtr expression1 = _CheckRDCase<RDCase_Expression>(_reader);
        _root->Child.Set(expression1);

        /**** ")" ";" ****/
        _CheckSymbol(")");
        _CheckSymbol(";");
    }
};

/*
 Statement -> Identifier "=" Expression ";"
*/
class RDCase_Statement_Assign : public RDCase {
public:
    RDCase_Statement_Assign(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {

        _Init(TreeSubType_Statement::Statement_assign);

        /**** Identifier ****/
        TreeNodePtr identifier1 = _CheckIdentifier();
        _root->Child.Set(identifier1);

        /**** "=" ****/
        _CheckSymbol("=");

        /**** Expression ****/
        TreeNodePtr expression1 = _CheckRDCase<RDCase_Expression>(_reader);
        identifier1->Subling.Set(expression1);

        /**** ";" ****/
        _CheckSymbol(";");
    }
};


/*
 Statement -> Identifier "[" Expression "]" "=" Expression ";"
*/
class RDCase_Statement_ArrayAssign : public RDCase {
public:
    RDCase_Statement_ArrayAssign(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstRoot = nullptr)override {

        _Init(TreeSubType_Statement::Statement_arrayAssign);

        /**** Identifier ****/
        TreeNodePtr identifier1 = _CheckIdentifier();
        _root->Child.Set(identifier1);

        /**** "[" ****/
        _CheckSymbol("[");

        /**** Expression ****/
        TreeNodePtr expression1 = _CheckRDCase<RDCase_Expression>(_reader);
        identifier1->Subling.Set(expression1);

        /**** "]" "=" ****/
        _CheckSymbol("]");
        _CheckSymbol("=");

        /**** Expression ****/
        TreeNodePtr expression2 = _CheckRDCase<RDCase_Expression>(_reader);
        expression1->Subling.Set(expression2);

        /**** ";" ****/
        _CheckSymbol(";");
    }
};


inline RDCase_Statement::RDCase_Statement(shared_ptr<TokenReader> reader) noexcept :
    RDCase(reader) {}

/*
 Statement -> ...
*/
inline void RDCase_Statement::_RunException(TreeNodePtr firstRoot) {

    _Init(TreeMainType::Statement);

    /* 识别每一种产生式 */
    auto statement_Sequence = RDCase_Statement_Sequence(_reader);
    auto statement_If = RDCase_Statement_If(_reader);
    auto statement_While = RDCase_Statement_While(_reader);
    auto statement_Assign = RDCase_Statement_Assign(_reader);
    auto statement_ArrayAssign = RDCase_Statement_ArrayAssign(_reader);
    auto statement_Println = RDCase_Statement_Println(_reader);
    RUN_ONCE() {
        if (_root = statement_Sequence.Run()) break;
        if (_root = statement_If.Run()) break;
        if (_root = statement_While.Run()) break;
        if (_root = statement_Assign.Run()) break;
        if (_root = statement_ArrayAssign.Run()) break;
        if (_root = statement_Println.Run()) break;
    }

    /* 识别失败了 */
    if (_root == nullptr) {
        string msg = "识别Statement失败";
        auto error = SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
        _reader->RecoveryIndex(_curIndex);
        throw error;
    }

    /* 识别成功了 */
    return;
}
