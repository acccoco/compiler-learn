#include "SyntaxParser.h"
using namespace std;



/*
 递归下降
 MainClass
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::MainClass(list<shared_ptr<SyntaxError>>& errorList) {

    errorList.clear();                                      // 清空错误列表    
    const int curIndex = _reader->GetIndex();               // reader当前的位置    
    SyntaxTreeNodePtr tempTreeNode;                         // 收集结果的临时节点          
    list<shared_ptr<SyntaxError>> tempErrorList;            // 收集错误的临时链表
    auto curTreeType = TreeNodeMainTypeEnum::MainClass;     // 当前树节点类型              


    // 判断第一个Token是否存在
    ETokenPtr firstToken = _reader->SeekToken();
    if (firstToken == NULL) {
        errorList.emplace_back(new SyntaxError(_reader, "识别第一个token失败", curTreeType));
        return NULL;
    }
    // 建立根节点
    SyntaxTreeNodePtr root(new SyntaxTreeNode(curTreeType, firstToken->GetLineNum()));
    


    /******************
     "class"
     ********************/
    if (_MatchTokenSequence({ {TokenTypeEnum::KEYWORD, "class"} }) == false) {
        errorList.emplace_back(new SyntaxError(_reader, "识别关键字class失败", curTreeType));
        _reader->SetIndex(curIndex);
        return NULL;
    }
    /******************
     类名
     Identifier
     ********************/
    if (tempTreeNode = _MatchIdentifier()) {
        root->SetChild(tempTreeNode);
    }
    else {
        errorList.emplace_back(new SyntaxError(_reader, "识别类名失败", curTreeType));
        _reader->SetIndex(curIndex);
        return NULL;
    }
    /******************
     "{ public static void main ( String [ ]"
     ********************/
    vector<pair<TokenTypeEnum, string>> tokenSequence_1 = {
        {TokenTypeEnum::SYMBOL, "{"},
        {TokenTypeEnum::KEYWORD, "public"},
        {TokenTypeEnum::KEYWORD, "static"},
        {TokenTypeEnum::KEYWORD, "void"},
        {TokenTypeEnum::KEYWORD, "main"},
        {TokenTypeEnum::SYMBOL, "("},
        {TokenTypeEnum::KEYWORD, "String"},
        {TokenTypeEnum::SYMBOL, "["},
        {TokenTypeEnum::SYMBOL, "]"},
    };
    if (_MatchTokenSequence(tokenSequence_1) == false) {
        errorList.emplace_back(new SyntaxError(_reader, "识别{public static void main(String[]失败", curTreeType));
        _reader->SetIndex(curIndex);
        return NULL;
    }
    /******************
     参数名
     Identifier
     ********************/
    tempTreeNode = _MatchIdentifier();
    if (tempTreeNode = _MatchIdentifier()) {
        root->GetChild()->SetSubling(tempTreeNode);
    }
    else {
        errorList.emplace_back(new SyntaxError(_reader, "识别主方法参数名失败", curTreeType));
        _reader->SetIndex(curIndex);
        return NULL;
    }
    /******************
     ") {"
     ********************/
    vector<pair<TokenTypeEnum, string>> tokenSequence_2 = {
        {TokenTypeEnum::SYMBOL, ")"},
        {TokenTypeEnum::SYMBOL, "{"},
    };
    if (_MatchTokenSequence(tokenSequence_1) == false) {
        errorList.emplace_back(new SyntaxError(_reader, "识别){失败", curTreeType));
        _reader->SetIndex(curIndex);
        return NULL;
    }
    /******************
     识别Statement
     ********************/
    if (tempTreeNode = Statement(tempErrorList)) {
        root->GetChild()->GetSubling()->SetSubling(tempTreeNode);
    }
    else {
        errorList.emplace_back(new SyntaxError(_reader, "识别Statement失败", curTreeType));
        errorList.insert(errorList.end(), tempErrorList.begin(), tempErrorList.end());
        _reader->SetIndex(curIndex);
        return NULL;
    }
    /******************
     "} }"
     ********************/
    vector<pair<TokenTypeEnum, string>> tokenSequence_3 = {
        {TokenTypeEnum::SYMBOL, "}"},
        {TokenTypeEnum::SYMBOL, "}"},
    };
    if (_MatchTokenSequence(tokenSequence_1) == false) {
        errorList.emplace_back(new SyntaxError(_reader, "识别}}失败", curTreeType));
        _reader->SetIndex(curIndex);
        return NULL;
    }

    return root;
}