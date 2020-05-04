#pragma once
#include "RDCase.h"
using namespace std;

/*
 Type -> Identifier
*/
class RDCase_Type_Identifier : public RDCase {
public:
    RDCase_Type_Identifier(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstNode = nullptr) override {
        _InitT(TreeSybType_Type::Type_Identifier);
        /**********************************
         Identifier
        ************************************/
        auto identifier = _CheckIdentifier();
        _root->Child.Set(identifier);

        return _root;
    }
};
/*
 Type -> "boolean"
*/
class RDCase_Type_Boolean : public RDCase {
public:
    RDCase_Type_Boolean(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstNode = nullptr) override {
        _InitT(TreeSybType_Type::Type_boolean);
        /**********************************
         "boolean"
        ************************************/
        _CheckKeyword("boolean");

        return _root;
    }
};
/*
 Type -> "int"
*/
class RDCase_Type_Int : public RDCase {
public:
    RDCase_Type_Int(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstNode = nullptr) override {
        _InitT(TreeSybType_Type::Type_Int);
        /**********************************
         "int"
        ************************************/
        _CheckKeyword("int");

        return _root;
    }
};
/*
 Type -> "int" "[" "]"
*/
class RDCase_Type_IntArray : public RDCase {
public:
    RDCase_Type_IntArray(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstNode = nullptr) override {
        _InitT(TreeSybType_Type::Type_IntArray);
        /**********************************
         "int" "[" "]"
        ************************************/
        _CheckKeyword("int");
        _CheckSymbol("[");
        _CheckSymbol("]");

        return _root;
    }
};



/*
 Type -> ...
*/
class RDCase_Type : public RDCase {
public:
    RDCase_Type(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstNode = nullptr) override {
        _InitT(TreeMainType::Type);

        auto type_identifier = RDCase_Type_Identifier(_reader);
        auto type_boolean = RDCase_Type_Boolean(_reader);
        auto type_int = RDCase_Type_Int(_reader);
        auto type_intArray = RDCase_Type_IntArray(_reader);
        if (!(_root = type_identifier.Run())
            && !(_root = type_boolean.Run())
            && !(_root = type_int.Run())
            && !(_root = type_intArray.Run())) {

            string msg = "识别Type失败";
            // _errors
            // 使用异常处理这个错误
        }

        return _root;
    }
};