#pragma once
#include "RDCase.h"

class RDCase_Statement :public RDCase {
public:
    RDCase_Statement(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
    SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr)override {
        _InitT(TreeMainType::Statement);


        return _root;
    }
};