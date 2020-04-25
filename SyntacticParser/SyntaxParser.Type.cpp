#include "SyntaxParser.h"
using namespace std;

/*
 递归下降
 Type
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Type(list<shared_ptr<SyntaxError>>& errorList) {

    errorList.clear();
    // 判断第一个单词是否存在
    ETokenPtr firstToken = _reader->SeekToken();
    if (firstToken == NULL) {
        return NULL;
    }
    // 建立根节点
    SyntaxTreeNodePtr root(new SyntaxTreeNode(TreeNodeMainTypeEnum::Type, firstToken->GetLineNum()));
    SyntaxTreeNodePtr temp;


    if (_MatchTokenSequence({ {TokenTypeEnum::KEYWORD, "int"} })) {
        /**********************************
         分支1
         "int[]"
        ************************************/
        if (_MatchTokenSequence({ {TokenTypeEnum::SYMBOL, "["}, {TokenTypeEnum::SYMBOL, "]"} })) {
            root->SetSubType(TreeNodeSubTypeEnum_Type::Type_IntArray);
            return root;
        }
        /**********************************
         分支2
         "int"
        ************************************/
        else {
            root->SetSubType(TreeNodeSubTypeEnum_Type::Type_Int);
            return root;
        }
    }
    /**********************************
     分支3
     "boolean"
    ************************************/
    else if (_MatchTokenSequence({ {TokenTypeEnum::KEYWORD, "boolean"} })) {
        root->SetSubType(TreeNodeSubTypeEnum_Type::Type_boolean);
        return root;
    }
    /**********************************
     分支4
     自定义类型
     Identifier
    ************************************/
    else if (temp = _MatchIdentifier()) {
        root->SetSubType(TreeNodeSubTypeEnum_Type::Type_Identifier);
        if (temp = _MatchIdentifier()) {
            root->SetChild(temp);
            return root;
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}