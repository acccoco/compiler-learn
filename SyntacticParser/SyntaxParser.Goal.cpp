#include "SyntaxParser.h"
using namespace std;


#if false
/*
 Goal -> MainClass { ClassDeclaration } EOF
*/
shared_ptr<TreeNode> SyntaxParser::Goal(list<shared_ptr<SyntaxError>>& errorList) {

    RECURSIVE_DESCENT_INIT_RETURN(TreeNodeMainTypeEnum::Goal);

    /**********************************
     MainClass
    ************************************/
    tempTreeNode = MainClass(tempErrorList);
    SET_SUBTREE_CHILD_RETURN(tempTreeNode, root, curTreeType, "识别MainClass失败");
    /**********************************
     { ClassDeclaration }
    ************************************/
    TreeNodePtr preNode = tempTreeNode;
    while (tempTreeNode = ClassDeclaration(tempErrorList)) {
        preNode->Subling.Set(tempTreeNode);
        preNode = tempTreeNode;
    }
    /**********************************
     EOF
    ************************************/
    if (_reader->IsEnd() == false) {
        errorList.emplace_back(new SyntaxError(_reader, "本该到达文件结尾", curTreeType));
        _reader->Index.Set(curIndex);
        return NULL;
    }

    return root;
}

#endif