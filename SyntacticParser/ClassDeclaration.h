#pragma once
#include "RDCase.h"
#include "VarDeclaration.h"
#include "MethodDeclaration.h"

class RDCase_ClassDeclaration : public RDCase {
public:
    RDCase_ClassDeclaration(shared_ptr<TokenReader> reader)noexcept :
        RDCase(reader) {}
protected:

    /*
     ClassDeclaration -> "class" Identifier [ "extends" Identifier ]
        "{" { VarDeclaration } { MethodDeclaration } "}"
    */
    void _RunException(TreeNodePtr firstRoot = nullptr)override {

        _Init(TreeMainType::ClassDeclaration);

        /**** "class" ****/
        _CheckKeyword("class");

        /**** Identifier ****/
        TreeNodePtr identifier1 = _CheckIdentifier();
        _root->Child.Set(identifier1);

        /**** ["extends" Identifier] ****/
        const int curIndex2 = _reader->Index.Get();
        if (_MatchKeyword("extends")) {
            TreeNodePtr identifier2 = _MatchIdentifier();
            if (identifier2 == nullptr) {
                _reader->RecoveryIndex(curIndex2);
            }
            _root->Child.Get()->Child.Set(identifier2);
        }

        /**** "{" ****/
        _CheckSymbol("{");

        /**** { VarDeclaration } ****/
        TreeNodePtr varDecSeqNode = _MatchCaseSequence<RDCase_VarDeclaration>();
        identifier1->Subling.Set(varDecSeqNode);

        /**** { MethodDeclaration } ****/
        TreeNodePtr methodDecSeqNode = _MatchCaseSequence<RDCase_MethodDeclaration>();
        varDecSeqNode->Subling.Set(methodDecSeqNode);

        /**** "}" ****/
        _CheckSymbol("}");
    }
};