#include "SyntaxParser.h"
using namespace std;

#if false

/*
 MethodDeclaration -> "public" Type Identifier
 "(" [ Type Identifier { "," Type Identifier } ] ")" "{" { VarDeclaration } { Statement }
 "return" Expression ";" "}"
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::MethodDeclaration(list<shared_ptr<SyntaxError>>& errorList) {
    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeMainTypeEnum::MethodDeclaration);
    SyntaxTreeNodePtr preNode;          // subling很长，需要这个辅助
    SyntaxTreeNodePtr tempTypeNode;     // 由于type是identifier的子节点，所以用这个来缓存
    /**********************************
     "public"
    ************************************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "public", curTreeType, "识别public失败");
    /**********************************
     Type
    ************************************/
    tempTypeNode = Type(tempErrorList);
    if (!tempTypeNode) {
        errorList.emplace_back(new SyntaxError(_reader, "", TreeNodeMainTypeEnum::MethodDeclaration));
        errorList.insert(errorList.end(), tempErrorList.begin(), tempErrorList.end());
        _reader->Index.Set(curIndex);
        return NULL;
    }
    /**********************************
     Identifier
    ************************************/
    tempTreeNode = _MatchIdentifier();
    SET_IDEN_OR_INT_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别Identifier失败");
    tempTreeNode->Child.Set(tempTypeNode);
    /**********************************
     "("
    ************************************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "(", curTreeType, "识别(失败");
    /**********************************
     [ Type Identifier { "," Type Identifier } ]
    ************************************/
    SyntaxTreeNodePtr paramSequence(new SyntaxTreeNode(TreeNodeMainTypeEnum::Default, 0));
    (root->Child).Get()->Subling.Set(paramSequence);
    const int curIndex1 = _reader->Index.Get();
    tempTypeNode = Type(tempErrorList);
    if (tempTypeNode) {
        tempTreeNode = _MatchIdentifier();
        if (tempTreeNode) {
            tempTreeNode->Child.Set(tempTypeNode);
            paramSequence->Child.Set(tempTreeNode);
            preNode = tempTreeNode;
            while (true) {
                const int curIndex2 = _reader->Index.Get();
                tempTypeNode = Type(tempErrorList);
                if (!tempTypeNode) break;
                tempTreeNode = _MatchIdentifier();
                if (!tempTreeNode) {
                    _reader->Index.Set(curIndex2);  // 这个小节的指针回退  
                    break;
                }
                tempTreeNode->Child.Set(tempTypeNode);
                preNode->Subling.Set(tempTreeNode);
                preNode = tempTreeNode;
            }
        }
        else {
            _reader->Index.Set(curIndex1);  // 这个小节的指针回退
        }
    }
    /**********************************
     ")" "{"
    ************************************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ")", curTreeType, "识别)失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "{", curTreeType, "识别{失败");
    /**********************************
     { VarDeclaration }
    ************************************/
    SyntaxTreeNodePtr varDeclarSeq(new SyntaxTreeNode(TreeNodeMainTypeEnum::Default, 0));
    paramSequence->Subling.Set(varDeclarSeq);
    if (tempTreeNode = VarDeclaration(tempErrorList)) {
        varDeclarSeq->Child.Set(tempTreeNode);
        preNode = tempTreeNode;
        while (tempTreeNode = VarDeclaration(tempErrorList)) {
            preNode->Subling.Set(tempTreeNode);
            preNode = tempTreeNode;
        }
    }
    /**********************************
     { Statement }
    ************************************/
    SyntaxTreeNodePtr statementSeq(new SyntaxTreeNode(TreeNodeMainTypeEnum::Default, 0));
    varDeclarSeq->Subling.Set(statementSeq);
    if (tempTreeNode = Statement(tempErrorList)) {
        statementSeq->Child.Set(tempTreeNode);
        preNode = tempTreeNode;
        while (tempTreeNode = Statement(tempErrorList)) {
            preNode->Subling.Set(tempTreeNode);
            preNode = tempTreeNode;
        }
    }
    /**********************************
     "return"
    ************************************/
    PROCESS_STR_RETURN(TokenTypeEnum::KEYWORD, "return", curTreeType, "识别return失败");
    /**********************************
     Expression
    ************************************/
    tempTreeNode = Expression(tempErrorList);
    SET_SUBTREE_SUBLING_RETURN(tempTreeNode, statementSeq, curTreeType, "识别Expression失败");
    /**********************************
     ";" "}"
    ************************************/
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, ";", curTreeType, "识别;失败");
    PROCESS_STR_RETURN(TokenTypeEnum::SYMBOL, "}", curTreeType, "识别}失败");
    return root;

}

#endif