#pragma once
#include "RDCase.h"
#include "Statement.h"


/*
 MainClass -> "class" Identifier
 "{" "public" "static" "void" "main" "(" "String" "[" "]"
 Identifier ")" "{" Statement "}" "}"
*/
class RDCase_MainClass : public RDCase {
public:
    RDCase_MainClass(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)override {
        _InitT(TreeMainType::MainClass);
        /******************
         "class"
         ********************/
        _CheckKeyword("class");
        /******************
         Identifier
        ********************/
        auto identifier1 = _CheckIdentifier();
        _root->Child.Set(identifier1);
        /******************
         "{" "public" "static" "void" "main" "(" "String" "[" "]"
        ********************/
        _CheckSymbol("{");
        _CheckKeyword("public");
        _CheckKeyword("static");
        _CheckKeyword("void");
        _CheckKeyword("main");
        _CheckSymbol("(");
        _CheckKeyword("String");
        _CheckSymbol("[");
        _CheckSymbol("]");
        /******************
         Identifier
        ********************/
        auto identifier2 = _CheckIdentifier();
        _root->Child.Get()->Subling.Set(identifier2);
        /******************
         ")" "{"
        ********************/
        _CheckSymbol(")");
        _CheckSymbol("{");
        /******************
         Statement
        ********************/
        auto statement = RDCase_Statement(_reader);
        statement.Run();
        _CheckSubTree(&statement);
        _root->Child.Get()->Subling.Get()->Subling.Set(statement.Root.Get());
        /******************
         "}" "}"
        ********************/
        _CheckSymbol("}");
        _CheckSymbol("}");

        return _root;
    }
};