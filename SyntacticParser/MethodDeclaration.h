#pragma once
#include "RDCase.h"
#include "Type.h"

/*
 MethodDeclaration -> "public" Type Identifier
 "(" [ Type Identifier { "," Type Identifier } ] ")" "{" { VarDeclaration } { Statement }
 "return" Expression ";" "}"
*/

class RDCase_MethodDeclaration : public RDCase {
public:
    RDCase_MethodDeclaration(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
protected:
    TreeNodePtr _RunException(TreeNodePtr firstNode = nullptr) override {
        

        return _root;
    }
};