#pragma once
#include "RDCase.h"

class RDCase_ClassDeclaration : public RDCase {
public:
    RDCase_ClassDeclaration(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)override {


    }
};