#include "SyntaxParser.h"
using namespace std;

/*
 递归下降：Goal
 如果识别失败，reader的指针会回退
*/
shared_ptr<SyntaxTreeNode> SyntaxParser::Goal(list<shared_ptr<SyntaxError>>& errorList) {

    errorList.clear();                              // 清空错误列表
    const int curIndex = _reader->GetIndex();       // reader当前的位置
    SyntaxTreeNodePtr tempTreeNode;                 // 收集结果的临时节点
    list<shared_ptr<SyntaxError>> tempErrorList;    // 收集错误的临时链表
    auto curTreeType = TreeNodeMainTypeEnum::Goal;  // 当前树节点类型

    /**********************************
     建立根节点
     判断第一个token是否存在
    ************************************/
    ETokenPtr firstToken = _reader->SeekToken();
    if (firstToken == NULL) {
        errorList.emplace_back(new SyntaxError(_reader, "识别开始token失败", curTreeType));
        return NULL;
    }
    SyntaxTreeNodePtr root(new SyntaxTreeNode(curTreeType, firstToken->GetLineNum()));


    /**********************************
     读取MainClass：必须有一个
    ************************************/
    SyntaxTreeNodePtr mainClass = MainClass(tempErrorList);
    if (mainClass == NULL) {
        errorList.emplace_back(new SyntaxError(_reader, "识别MainClass失败", curTreeType));
        errorList.insert(errorList.end(), tempErrorList.begin(), tempErrorList.end());
        _reader->SetIndex(curIndex);
        return NULL;
    }
    else {
        root->SetChild(mainClass);
    }
    /**********************************
     读取类定义ClassDeclaration：0个或多个
    ************************************/
    SyntaxTreeNodePtr preNode = mainClass;
    SyntaxTreeNodePtr classDeclaration;
    while (classDeclaration = ClassDeclaration(tempErrorList)) {
        preNode->SetSubling(classDeclaration);
        preNode = classDeclaration;
    }
    /**********************************
     以EOF结束
    ************************************/
    if (_reader->IsEnd() == false) {
        errorList.emplace_back(new SyntaxError(_reader, "本该到达文件结尾", curTreeType));
        _reader->SetIndex(curIndex);
        return NULL;
    }

    return root;
}