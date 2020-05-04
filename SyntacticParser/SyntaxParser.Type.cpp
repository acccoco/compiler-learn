#include "SyntaxParser.h"
using namespace std;

#if false

/*
 Type -> ...
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Type(list<shared_ptr<SyntaxError>>& errorList) {

    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeMainTypeEnum::Type);

    if (!(tempTreeNode = Type_Identifier(errorList))
        && !(tempTreeNode = Type_boolean(errorList))
        && !(tempTreeNode = Type_Int(errorList))
        && !(tempTreeNode = Type_IntArray(errorList))) {

        errorList.emplace_back(new SyntaxError(_reader, "识别Type失败", curTreeType));
        return root;
    }
    return tempTreeNode;
}

/*
 Type -> Identifier
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Type_Identifier(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Type::Type_Identifier);
    /**********************************
     Identifier
    ************************************/
    tempTreeNode = _MatchIdentifier();
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Identifier失败");
    return root;
}
/*
 Type -> "boolean"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Type_boolean(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Type::Type_boolean);
    /**********************************
     "boolean"
    ************************************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "boolean", curTreeType, "识别boolean失败");
    return root;
}
/*
 Type -> "int"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Type_Int(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Type::Type_Int);
    /**********************************
     "int"
    ************************************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "int", curTreeType, "识别int失败");
    return root;
}
/*
 Type -> "int" "[" "]"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Type_IntArray(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Type::Type_IntArray);
    /**********************************
     "int" "[" "]"
    ************************************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "int", curTreeType, "识别int失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "[", curTreeType, "识别[失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "]", curTreeType, "识别]失败");
    return root;

}

#endif