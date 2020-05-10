#pragma once
#include "RDCase.h"
#include "Type.h"

class RDCase_VarDeclaration : public RDCase {
public:
    RDCase_VarDeclaration(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
protected:

    /*
     VarDeclaration -> Type Identifier ";"
    */
    void _RunException(TreeNodePtr firstRoot = nullptr)override {

        _Init(TreeMainType::VarDeclaration);

        /**** Type ****/
        auto type1 = RDCase_Type(_reader);
        type1.Run();
        _CheckSubTree(&type1);
        _root->Child.Set(type1.Root.Get());

        /**** Identifier ****/
        TreeNodePtr identifier1 = _CheckIdentifier();
        _root->Child.Get()->Subling.Set(identifier1);

        /**** ";" ****/
        _CheckSymbol(";");

    }
};