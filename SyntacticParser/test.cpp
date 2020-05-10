#include "../LexicalAnalyzer/Token.h"
#include "../LexicalAnalyzer/LexicalAnalyzer.h"
#include "../LexicalAnalyzer/fileReader.h"
#include "SyntaxWriter.h"
#include "Expression.h"
#include "TreeSerialize.h"
#include <vector>
#include <iostream>
using namespace std;


/* 测试TreeSerialize */
void TreeSerializeTest() {

    /* 首先构造语法树 */
    /*
     root
      |
     node1 - node2 ----------- node5
              |                 |
             node3 - node4     node6 - node7
    */
    TreeNodePtr  root(new TreeNode(TreeMainType::Default, 0, "root"));
    TreeNodePtr node1(new TreeNode(TreeMainType::Default, 0, "node1"));
    TreeNodePtr node2(new TreeNode(TreeMainType::Default, 0, "node2"));
    TreeNodePtr node3(new TreeNode(TreeMainType::Default, 0, "node3"));
    TreeNodePtr node4(new TreeNode(TreeMainType::Default, 0, "node4"));
    TreeNodePtr node5(new TreeNode(TreeMainType::Default, 0, "node5"));
    TreeNodePtr node6(new TreeNode(TreeMainType::Default, 0, "node6"));
    TreeNodePtr node7(new TreeNode(TreeMainType::Default, 0, "node7"));
    root->Child.Set(node1);
    node1->Subling.Set(node2);
    node2->Child.Set(node3);
    node3->Subling.Set(node4);
    node2->Subling.Set(node5);
    node5->Child.Set(node6);
    node6->Subling.Set(node7);

    /* 序列化 */
    auto strs = TreeSerialize::Serialize(root);

    /* 打印 */
    for (auto str : strs) {
        cout << str << endl;
    }
}

/* 测试语法分析 */
void SyntaxTest() {

    /* 定义一组源代码 */
    vector<string> code;
    code.emplace_back(
"class Main{\
public static void main(String[] argv){\
System.out.println(true);}\
}\
class Derive extends Test {\
boolean isTrue;\
public int foo(){return true;}\
}");

    /* 词法分析 */
    auto lexicalAnalyzer = LexialAnalyzer();
    lexicalAnalyzer.LexicalAnlyze(code);
    auto tokens = lexicalAnalyzer.GetResultTokens();

    /* 语法分析 */
    shared_ptr<TokenReader> reader(new TokenReader(tokens));
    auto syntaxAnalyzer = SyntaxParser(reader);
    syntaxAnalyzer.Parse();
    auto root = syntaxAnalyzer.Root.Get();
    auto errors = syntaxAnalyzer.ErrorList.Get();

    /* 语法分析结果序列化 */
    auto serialTree = TreeSerialize::Serialize(root);
    for (string str : serialTree)
        cout << str << endl;
}

#if false
int main() {

    SyntaxTest();

}
#endif