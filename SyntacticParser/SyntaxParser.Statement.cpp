#include "SyntaxParser.h"
using namespace std;

#if false

/*
 Statement -> ...
*/
shared_ptr<TreeNode> SyntaxParser::Statement(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeMainTypeEnum::Statement);
    if (!(tempTreeNode = Statement_Sequence(tempErrorList))
        && !(tempTreeNode = Statement_If(tempErrorList))
        && !(tempTreeNode = Statement_while(tempErrorList))
        && !(tempTreeNode = Statement_Assign(tempErrorList))
        && !(tempTreeNode = Statement_ArrayAssign(tempErrorList))
        && !(tempTreeNode = Statement_Println(tempErrorList))) {

        return NULL;
    }
    return tempTreeNode;
}

/*
 Statement -> "{" { Statement } "}"
*/
shared_ptr<TreeNode> SyntaxParser::Statement_Sequence(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Statement::Statement_Sequence);
    /******************
     "{"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "{", curTreeType, "识别{失败");
    /******************
     { Statement }
     ********************/
    if (tempTreeNode = Statement(tempErrorList)) {
        root->Child.Set(tempTreeNode);
        TreeNodePtr preNode = tempTreeNode;
        while (tempTreeNode = Statement(tempErrorList)) {
            preNode->Subling.Set(tempTreeNode);
            preNode = tempTreeNode;
        }
    }
    /******************
     "}"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "}", curTreeType, "识别}失败");
    return root;
}
/*
 Statement -> "while" "(" Expression ")" Statement
*/
shared_ptr<TreeNode> SyntaxParser::Statement_while(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Statement::Statement_while);
    /******************
     "while" "("
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "while", curTreeType, "识别while失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "(", curTreeType, "识别(失败");
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Expression失败");
    /******************
     ")"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ")", curTreeType, "识别)失败");
    /******************
      Statement
      ********************/
    tempTreeNode = Statement(tempErrorList);
    SET_IDEN_OR_INT_SUBLING_RETURN(tempTreeNode, root->Child.Get(), curTreeType, "识别Statement失败");
    return root;
}
/*
 Statement -> "if" "(" Expression ")" Statement "else" Statement
*/
shared_ptr<TreeNode> SyntaxParser::Statement_If(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Statement::Statement_if);
    /******************
     "if" "("
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "if", curTreeType, "识别if失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "(", curTreeType, "识别(失败");
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Expression失败");
    /******************
     ")"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ")", curTreeType, "识别)失败");
    /******************
      Statement
      ********************/
    tempTreeNode = Statement(tempErrorList);
    SET_IDEN_OR_INT_SUBLING_RETURN(tempTreeNode, root->Child.Get(), curTreeType, "识别Statement失败");
    /******************
     "else"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "else", curTreeType, "识别else失败");
    /******************
      Statement
      ********************/
    tempTreeNode = Statement(tempErrorList);
    SET_IDEN_OR_INT_SUBLING_RETURN(tempTreeNode, root->Child.Get()->Subling.Get(), curTreeType, "识别Statement失败");
    return root;
}
/*
 Statement -> "System.out.println" "(" Expression ")" ";"
*/
shared_ptr<TreeNode> SyntaxParser::Statement_Println(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Statement::Statement_println);
    /******************
     "System.out.println" "("
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "System.out.println", curTreeType, "识别System.out.println失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "(", curTreeType, "识别(失败");
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Expression失败");
    /******************
     ")" ";"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ")", curTreeType, "识别)失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ";", curTreeType, "识别;失败");
    return root;
}
/*
 Statement -> Identifier "=" Expression ";"
*/
shared_ptr<TreeNode> SyntaxParser::Statement_Assign(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Statement::Statement_assign);
    /******************
     Identifier
     ********************/
    tempTreeNode = _MatchIdentifier();
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别identifier失败");
    /******************
     "="
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "=", curTreeType, "识别=失败");
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_IDEN_OR_INT_SUBLING_RETURN(tempTreeNode, root->Child.Get(), curTreeType, "识别Expression失败");
    /******************
     ";"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ";", curTreeType, "识别;失败");
    return root;
}
/*
 Statement -> Identifier "[" Expression "]" "=" Expression ";"
*/
shared_ptr<TreeNode> SyntaxParser::Statement_ArrayAssign(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Statement::Statement_arrayAssign);
    /******************
     Identifier
     ********************/
    tempTreeNode = _MatchIdentifier();
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别identifier失败");
    /******************
     "["
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "[", curTreeType, "识别[失败");
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_IDEN_OR_INT_SUBLING_RETURN(tempTreeNode, root->Child.Get(), curTreeType, "识别Expression失败");
    /******************
     "]" "="
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "]", curTreeType, "识别]失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "=", curTreeType, "识别=失败");
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_IDEN_OR_INT_SUBLING_RETURN(tempTreeNode, root->Child.Get()->Subling.Get(), curTreeType, "识别Expression失败");
    /******************
     ";"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ";", curTreeType, "识别;失败");

    return root;
}

#endif