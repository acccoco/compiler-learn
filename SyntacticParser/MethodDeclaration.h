#pragma once
#include "RDCase.h"
#include "Type.h"
#include "VarDeclaration.h"
#include "Statement.h"
#include "Expression.h"

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
    void _RunException(TreeNodePtr firstNode = nullptr) override {

        /* 例行初始化 */
        _Init(TreeMainType::MethodDeclaration);

        /**** "public" ****/
        _CheckKeyword("public");

        /**** Type ****/
        auto type1 = RDCase_Type(_reader);
        type1.Run();
        _CheckSubTree(&type1);

        /**** Identifier ****/
        auto identifier1 = _CheckIdentifier();

        /* 组装AST的第一个子节点 */
        identifier1->Child.Set(type1.Root.Get());
        _root->Child.Set(identifier1);

        /**** "(" ****/
        _CheckSymbol("(");

        /**** [ Type Identifier { "," Type Identifier } ] ****/
        auto paramSeqNode = _MatchOptional1();
        _root->Child.Get()->Subling.Set(paramSeqNode);

        /**** ")" "{" ****/
        _CheckSymbol(")");
        _CheckSymbol("{");

        /**** { VarDeclaration } ****/
        TreeNodePtr varDeclarSeqNode = _MatchCaseSequence<RDCase_VarDeclaration>();
        paramSeqNode->Subling.Set(varDeclarSeqNode);

        /**** { Statement } ****/
        TreeNodePtr stateSeqNode = _MatchCaseSequence<RDCase_Statement>();
        varDeclarSeqNode->Subling.Set(stateSeqNode);

        /**** "return" ****/
        _CheckKeyword("return");

        /**** Expression ****/
        auto expression1 = RDCase_Expression(_reader);
        expression1.Run();
        _CheckSubTree(&expression1);
        stateSeqNode->Subling.Set(expression1.Root.Get());

        /**** ";" "}" ****/
        _CheckSymbol(";");
        _CheckSymbol("}");

        return;
    }

    /* 
     [ Type Identifier { "," Type Identifier } ]
     可选小节的匹配
     异常：不会抛出异常
     失败了会恢复reader的指针
     返回一个sequenceNode，其子节点是一系列小节匹配结果的集合
     */
    TreeNodePtr _MatchOptional1() noexcept{

        /**** 尝试匹配第一个小节 [ Type Identifier ... ] ****/
        TreeNodePtr paramSeqNode(new TreeNode(TreeMainType::Default, 0));

        bool isSectionMatch = false;    // 标记第一个可选小节是否匹配成功
        RUN_ONCE() {
            const int curIndex2 = _reader->Index.Get();     // 失败回退的准备

            auto type2 = RDCase_Type(_reader);
            type2.Run();
            if (type2.Root.Get() == nullptr) break;

            auto identifier2 = _MatchIdentifier();
            if (identifier2 == nullptr) {
                _reader->RecoveryIndex(curIndex2);
                break;
            }

            /* 小节匹配成功，组装节点，连接到AST上 */
            isSectionMatch = true;
            paramSeqNode->Child.Set(identifier2);
            identifier2->Child.Set(type2.Root.Get());
        }

        /* 如果第一小节匹配失败，就此返回 */
        if (!isSectionMatch) return paramSeqNode;

        /**** 尝试匹配其他小节：  { "," Type Identifier } ****/
        TreeNodePtr preNode = paramSeqNode->Child.Get();

        while (true) {
            const int curIndex3 = _reader->Index.Get();     // 失败回退的准备

            if (!_MatchSymbol(",")) break;

            auto type3 = RDCase_Type(_reader);
            type3.Run();
            if (type3.Root.Get() == nullptr) {
                _reader->RecoveryIndex(curIndex3);
                break;
            }

            auto identifier3 = _MatchIdentifier();
            if (identifier3 == nullptr) {
                _reader->RecoveryIndex(curIndex3);
                break;
            }

            /* 小节匹配成功，组装节点 */
            preNode->Subling.Set(identifier3);
            identifier3->Child.Set(type3.Root.Get());
            preNode = identifier3;
        }

        return paramSeqNode;
    }
};