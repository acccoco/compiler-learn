#include "SyntaxParser.h"
using namespace std;

#if false

/*
 VarDeclaration -> Type Identifier ";"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::VarDeclaration(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeMainTypeEnum::VarDeclaration);
    /**********************************
     Type
    ************************************/
    tempTreeNode = Type(tempErrorList);
    SET_SUBTREE_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Type失败");
    /**********************************
     Identifier
    ************************************/
    tempTreeNode = _MatchIdentifier();
    SET_IDEN_OR_INT_SUBLING_RETURN(tempTreeNode, root->Child.Get(), curTreeType, "识别Identifier失败");
    /**********************************
     ";"
    ************************************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ";", curTreeType, "识别;失败");
    return root;
}

#endif