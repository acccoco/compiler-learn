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
protected:
    void _RunException(TreeNodePtr firstNode = nullptr) override {
        _Init(TreeSubType_Type::Type_Identifier);
        /**********************************
         Identifier
        ************************************/
        auto identifier = _CheckIdentifier();
        _root->Child.Set(identifier);

        return;
    }
};
/*
 Type -> "boolean"
*/
class RDCase_Type_Boolean : public RDCase {
public:
    RDCase_Type_Boolean(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstNode = nullptr) override {
        _Init(TreeSubType_Type::Type_boolean);
        /**********************************
         "boolean"
        ************************************/
        _CheckKeyword("boolean");

        return;
    }
};
/*
 Type -> "int"
*/
class RDCase_Type_Int : public RDCase {
public:
    RDCase_Type_Int(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstNode = nullptr) override {
        _Init(TreeSubType_Type::Type_Int);
        /**********************************
         "int"
        ************************************/
        _CheckKeyword("int");

        return;
    }
};
/*
 Type -> "int" "[" "]"
*/
class RDCase_Type_IntArray : public RDCase {
public:
    RDCase_Type_IntArray(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstNode = nullptr) override {
        _Init(TreeSubType_Type::Type_IntArray);
        /**********************************
         "int" "[" "]"
        ************************************/
        _CheckKeyword("int");
        _CheckSymbol("[");
        _CheckSymbol("]");

        return;
    }
};



/*
 Type -> ...
*/
class RDCase_Type : public RDCase {
public:
    RDCase_Type(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
protected:
    void _RunException(TreeNodePtr firstNode = nullptr) override {
        _Init(TreeMainType::Type);

        /* 每一种产生式都尝试一下 */
        auto type_identifier = RDCase_Type_Identifier(_reader);
        auto type_boolean = RDCase_Type_Boolean(_reader);
        auto type_int = RDCase_Type_Int(_reader);
        auto type_intArray = RDCase_Type_IntArray(_reader);
        RUN_ONCE() {
            if (_root = type_identifier.Run()) break;
            if (_root = type_boolean.Run()) break;

            /* int[] 应该先于 int匹配 */
            if (_root = type_intArray.Run()) break;
            if (_root = type_int.Run()) break;
        }

        /* 识别都失败了，抛出异常 */
        if (_root == nullptr) {
            string msg = "识别Type失败";
            auto error = SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
            _reader->RecoveryIndex(_curIndex);
            throw error;
        }

        /* 识别成功了 */
        return;
    }
};