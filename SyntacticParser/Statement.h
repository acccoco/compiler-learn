#pragma once
#include "RDCase.h"

class RDCase_Statement :public RDCase {
public:
    RDCase_Statement(shared_ptr<TokenReader> reader) noexcept :
        RDCase(reader) {}
protected:
    TreeNodePtr _RunException(TreeNodePtr firstRoot = nullptr)override {
        _Init(TreeMainType::Statement);


        return _root;
    }
};