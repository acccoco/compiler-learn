#include "SyntaxParser.h"
using namespace std;



/*
 Expression -> ...
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeMainTypeEnum::Expression);
    // 首先尝试以终结符开头的产生式
    if (!(tempTreeNode = Expression_this(tempErrorList))
        && !(tempTreeNode = Expression_true(tempErrorList))
        && !(tempTreeNode = Expression_false(tempErrorList))
        && !(tempTreeNode = Expression_bracket(tempErrorList))
        && !(tempTreeNode = Expression_not(tempErrorList))
        && !(tempTreeNode = Expression_newIdentifier(tempErrorList))
        && !(tempTreeNode = Expression_newIntArray(tempErrorList))
        && !(tempTreeNode = Expression_Identifier(tempErrorList))
        && !(tempTreeNode = Expression_IntegerLiteral(tempErrorList))) {

        errorList.emplace_back(new SyntaxError(_reader, "识别Expression错误", curTreeType));
        _reader->SetIndex(curIndex);
        return NULL;
    }
    // 尝试以非终结符开头的产生式，识别不成功，并不报错
    SyntaxTreeNodePtr newRoot;
    if ((newRoot = Expression_operate(root, tempErrorList))
        || (newRoot = Expression_arrayExpression(root, tempErrorList))
        || (newRoot = Expression_length(root, tempErrorList))
        || (newRoot = Expression_functionCall(root, tempErrorList))) {
        return newRoot;
    }
    else {
        return root;
    }
}
#pragma region 以终结符开头的产生式
/*
 Expression -> Identifier
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_Identifier(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_Identifier);
    /******************
     Identifier
     ********************/
    tempTreeNode = _MatchIdentifier();
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Identifier失败");

    return root;
}
/*
 Expression -> IntegerLiteral
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_IntegerLiteral(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_IntegerLiteral);
    /******************
     IntegerLiteral
     ********************/
    tempTreeNode = _MatchIntegerLiteral();
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别IntegerLiteral失败");

    return root;
}
/*
 Expression -> "this"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_this(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_This);
    /******************
     "this"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "this", curTreeType, "识别this失败");

    return root;
}
/*
 Expression -> "true"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_true(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_True);
    /******************
     "true"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "true", curTreeType, "识别true失败");

    return root;
}
/*
 Expression -> "false"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_false(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_False);
    /******************
     "false"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "false", curTreeType, "识别false失败");

    return root;
}
/*
 Expression -> "!" Expression
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_not(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_Not);
    /******************
     "!"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "!", curTreeType, "识别!失败");
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_SUBTREE_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Expression失败");

    return root;
}
/*
 Expression -> "(" Expression ")"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_bracket(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_Bracket);
    /******************
     "("
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "(", curTreeType, "识别(失败");
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_SUBTREE_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Expression失败");
    /******************
     ")"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ")", curTreeType, "识别)失败");

    return root;
}
/*
 Expression -> "new" "int" "[" Expression "]"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_newIntArray(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_NewIntArray);
    /******************
     "new" "int" "["
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "new", curTreeType, "识别new失败");
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "int", curTreeType, "识别int失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "[", curTreeType, "识别[失败");
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_SUBTREE_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Expression失败");
    /******************
     "]"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "]", curTreeType, "识别]失败");

    return root;
}
/*
 Expression -> "new" Identifier "(" ")"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_newIdentifier(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_NewIdentifier);
    /******************
     "new"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "new", curTreeType, "识别new失败");
    /******************
     Identifier
     ********************/
    tempTreeNode = _MatchIdentifier();
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Identifier失败");
    /******************
     "(" ")"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "(", curTreeType, "识别(失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ")", curTreeType, "识别)失败");

    return root;
}
#pragma endregion

#pragma region 以非终结符开头的产生式
/*
 Expression -> Expression "[" Expression "]"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_arrayExpression(shared_ptr<SyntaxTreeNode> firstNode, list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_ArrayExpression);
    /******************
     Expression
     ********************/
    root->SetChild(firstNode);
    /******************
     "["
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "[", curTreeType, "识别[失败");
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_SUBTREE_SUBLING_RETURN(tempTreeNode, root->GetChild(), curTreeType, "识别Expression失败");
    /******************
     "]"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "]", curTreeType, "识别]失败");

    return root;
}
/*
 Expression ->  Expression ( "&&" | "<" | "+" | "-" | "*" ) Expression
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_operate(shared_ptr<SyntaxTreeNode> firstNode, list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_Operate);
    /******************
     Expression
     ********************/
    root->SetChild(firstNode);
    /******************
     ( "&&" | "<" | "+" | "-" | "*" )
     ********************/
    if (_MatchKeywordOrSymbol(TokenTypeEnum::SYMBOL, "&&") == false
        && _MatchKeywordOrSymbol(TokenTypeEnum::SYMBOL, "<") == false
        && _MatchKeywordOrSymbol(TokenTypeEnum::SYMBOL, "+") == false
        && _MatchKeywordOrSymbol(TokenTypeEnum::SYMBOL, "-") == false
        && _MatchKeywordOrSymbol(TokenTypeEnum::SYMBOL, "*") == false) {
        errorList.emplace_back(new SyntaxError(_reader, "识别&&/</+/-/*均失败", curTreeType));
        _reader->SetIndex(curIndex);
        return NULL;
    }
    /******************
     Expression
     ********************/
    tempTreeNode = Expression(tempErrorList);
    SET_SUBTREE_SUBLING_RETURN(tempTreeNode, root->GetChild(), curTreeType, "识别Expression失败");

    return root;
}
/*
 Expression -> Expression "." "length"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_length(shared_ptr<SyntaxTreeNode> firstNode, list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_Length);
    /******************
     Expression
     ********************/
    root->SetChild(firstNode);
    /******************
     "." "length"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ".", curTreeType, "识别.失败");
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "length", curTreeType, "识别length失败");

    return root;
}
/*
 Expression "." Identifier "(" [ Expression { "," Expression } ] ")"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Expression_functionCall(shared_ptr<SyntaxTreeNode> firstNode, list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeSubTypeEnum_Expression::Exp_FunctionCall);
    /******************
     Expression
     ********************/
    root->SetChild(firstNode);
    /******************
     "."
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ".", curTreeType, "识别.失败");
    /******************
     Identifier
     ********************/
    tempTreeNode = _MatchIdentifier();
    SET_IDEN_OR_INT_SUBLING_RETURN(tempTreeNode, root->GetChild(), curTreeType, "识别Identifier失败");
    /******************
     "("
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "(", curTreeType, "识别(失败");

    /******************
     [ Expression { "," Expression } ]
     ********************/
    if (tempTreeNode = Expression(tempErrorList)) {
        SyntaxTreeNodePtr paramSequence(new SyntaxTreeNode(TreeNodeMainTypeEnum::Default, tempTreeNode->GetLineNum(), tempTreeNode));
        root->GetChild()->GetSubling()->SetSubling(paramSequence);
        SyntaxTreeNodePtr pre = tempTreeNode;
        while (true) {
            const int curIndex2 = _reader->GetIndex();
            if (_MatchKeywordOrSymbol(TokenTypeEnum::SYMBOL, ",") == false) break;
            if ((tempTreeNode = Expression(tempErrorList)) == NULL) {
                _reader->SetIndex(curIndex2);       // 这一小节识别失败，指针需要回退
                break;
            }
            pre->SetSubling(tempTreeNode);
            pre = tempTreeNode;
        }
    }
    /******************
     ")"
     ********************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ")", curTreeType, "识别)失败");

    return root;
}

#pragma endregion

