#include "SyntaxParser.h"
using namespace std;

#if false

/*
 MainClass -> "class" Identifier "{" "public" "static" "void" "main" "(" "String" "[" "]" Identifier ")" "{" Statement "}" "}"
*/
shared_ptr<TreeNode> SyntaxParser::MainClass(list<shared_ptr<SyntaxError>>& errorList) {

    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeMainTypeEnum::MainClass);
    /******************
     "class"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "class", curTreeType, "识别class失败");
    /******************
     Identifier
     ********************/
    tempTreeNode = _MatchIdentifier();
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别类名Identifier失败");
    /******************
     "{" "public" "static" "void" "main" "(" "String" "[" "]"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "{", curTreeType, "识别{失败");
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "public", curTreeType, "识别public失败");
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "static", curTreeType, "识别static失败");
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "void", curTreeType, "识别void失败");
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "main", curTreeType, "识别main失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "(", curTreeType, "识别(失败");
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "String", curTreeType, "识别String失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "[", curTreeType, "识别[失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "]", curTreeType, "识别]失败");
    /******************
     Identifier
     ********************/
    tempTreeNode = _MatchIdentifier();
    SET_SUBTREE_SUBLING_RETURN(tempTreeNode, root->Child.Get(), curTreeType, "识别参数名Identifier失败");
    /******************
     ")" "{"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ")", curTreeType, "识别)失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "{", curTreeType, "识别{失败");
    /******************
     Statement
     ********************/
    tempTreeNode = Statement(tempErrorList);
    SET_SUBTREE_SUBLING_RETURN(tempTreeNode, root->Child.Get()->Subling.Get(), curTreeType, "识别Statement失败");
    /******************
     "}" "}"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "}", curTreeType, "识别}失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "}", curTreeType, "识别}失败");

    return root;
}

#endif