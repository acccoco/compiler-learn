#pragma once
#include "TreeNode.h"
#include "TreeType.h"
#include <list>
#include <string>
#include <map>
using namespace std;

/*
 将树序列化，输出为字符串数组
*/
class TreeSerialize {
public:

    /*
     将AST序列化
     异常：不会抛出异常
    */
    static list<string> Serialize(TreeNodePtr root)noexcept {
        
        const string rootPrefix = "    ";       // 根节点的前缀是空格

        return _PreTravelSerialize(rootPrefix, root);

    }

protected:

    /*
     对AST前序遍历，得到字符串序列
     格式规定：子节点缩进4位
    */
    static list<string> _PreTravelSerialize(string prefix, TreeNodePtr root) {

        /* 用于控制显示格式的常量值 */
        const size_t replaceLength = 4;          // 替换字符的长度
        const string replaceLast = "    ";      // 替换发生时，该节点没有subling
        const string replaceNotLast = " |  ";   // 替换发生时，该节点有subling
        const string childPrefixAdd = " +- ";   // 为子节点增加的前缀

        list<string> result;

        /* 添加根节点 */
        if (root == nullptr) return result;
        result.push_back(prefix + _NodeToString(root));

        /* 生成子节点的前缀 */
        string childPrefix = prefix;
        childPrefix.erase(prefix.size() - replaceLength, replaceLength);    // 替换后n位
        childPrefix += (root->Subling.Get() == nullptr) ? replaceLast : replaceNotLast;
        childPrefix += childPrefixAdd;

        /* 添加子节点 */
        TreeNodePtr child = root->Child.Get();
        while (child != nullptr) {
            auto childResult = _PreTravelSerialize(childPrefix, child);
            result.insert(result.end(), childResult.begin(), childResult.end());
            child = child->Subling.Get();
        }

        return result;
    }

    /*
     不同节点内容的常量值，节点类型不同，值不同
    */
    class NodeStrConst {
    public:

        /* 主类型节点的说明值 */
        const map<TreeMainType, string> nodeStr_MainType = {

            /* Default作为序列 */
            {TreeMainType::Default, "Sequence"},

            /* 语法节点 */
            {TreeMainType::Goal, "Goal"},
            {TreeMainType::ClassDeclaration, "ClassDeclaration"},
            {TreeMainType::MainClass, "MainClass"},
            {TreeMainType::MethodDeclaration, "MethodDeclaration"},
            {TreeMainType::VarDeclaration, "VarDeclaration"},
        };

        /* Type类型节点的说明值 */
        const map<TreeSubType_Type, string> nodeStr_Type = {
            
            /* Type子类型 */
            {TreeSubType_Type::Type_boolean, "type: boolean"},
            {TreeSubType_Type::Type_Identifier, "type: "},
            {TreeSubType_Type::Type_Int, "type: int"},
            {TreeSubType_Type::Type_IntArray, "type: int[]"},

            /* Type主类型 */
            {TreeSubType_Type::Default, "type"},
        };

        /* Statement类型节点的说明值 */
        const map<TreeSubType_Statement, string> nodeStr_Statement = {

            /* Statement子类型 */
            {TreeSubType_Statement::Statement_arrayAssign, "statement: array assign"},
            {TreeSubType_Statement::Statement_assign, "statement: assign"},
            {TreeSubType_Statement::Statement_if, "statement: if"},
            {TreeSubType_Statement::Statement_println, "statement: println"},
            {TreeSubType_Statement::Statement_Sequence, "statement sequence: "},
            {TreeSubType_Statement::Statement_while, "statement: while"},

            /* Statement主类型 */
            {TreeSubType_Statement::Default, "statement: "},
        };

        /* Expression类型节点的说明值 */
        const map<TreeSubType_Expression, string> nodeStr_Expression = {

            /* 非左递归产生式 */
            {TreeSubType_Expression::Exp_Bracket, "expression: (expression)"},
            {TreeSubType_Expression::Exp_False, "expression: false"},
            {TreeSubType_Expression::Exp_Identifier, "expression: Identifier"},
            {TreeSubType_Expression::Exp_IntegerLiteral, "expression: IntegerLiteral"},
            {TreeSubType_Expression::Exp_NewIdentifier, "expression: new class"},
            {TreeSubType_Expression::Exp_NewIntArray, "expression: new int[]"},
            {TreeSubType_Expression::Exp_Not, "expression: !expression"},
            {TreeSubType_Expression::Exp_This, "expression: this"},
            {TreeSubType_Expression::Exp_True, "expression: true"},

            /* 左递归产生式 */
            {TreeSubType_Expression::Exp_ArrayExpression, "expression: expression[expression]"},
            {TreeSubType_Expression::Exp_Length, "expression: expression.length"},
            {TreeSubType_Expression::Exp_FunctionCall, "expression: functioncall"},
            {TreeSubType_Expression::Exp_Operate, "expression: operate "},

            /* 默认情形 */
            {TreeSubType_Expression::Default, "expression: "},
        };

        /* 节点为空的情形 */
        const string nodeStr_Null = "";

        /* 词法节点IntegerLiteral的说明值 */
        string nodeStr_Integer(int numValue) {
            char temp[256];
            string str = "IntegerLiteral<";
            _itoa_s(numValue, temp, 10);
            str += temp;
            str += ">";
            return str;
        }

        /* 词法节点Identifier的说明值 */
        string nodeStr_Identifier(string strValue) {
            return "Ientifier<" + strValue + ">";
        }
    };

    /*
     根据节点类型，将节点转化为字符串
    */
    static string _NodeToString(TreeNodePtr node) {

        NodeStrConst nodeStrConst;      // 节点说明值常量类的引用
        const auto treeType = node->Type.Get();     // 节点的类型
        const auto mainType = treeType.GetMainType();   // 主类型
        const auto subType_Expression = treeType.GetSubType_Expression();
        const auto subType_Statement = treeType.GetSubType_Statement();
        const auto subType_Type = treeType.GetSubType_Type();

        /* 检查节点是否存在 */
        if (node == nullptr) return nodeStrConst.nodeStr_Null;

        string str = "";

        /* 不同的节点类型，具有不同的值 */
        switch (node->Type.Get().GetMainType()) {

            /* 没有子类型的语法节点 */
        case TreeMainType::Goal:
        case TreeMainType::ClassDeclaration:
        case TreeMainType::MainClass:
        case TreeMainType::MethodDeclaration:
        case TreeMainType::VarDeclaration:
            str = nodeStrConst.nodeStr_MainType.at(mainType);
            break;

            /* Default节点（用作集合） */
        case TreeMainType::Default:
            str = nodeStrConst.nodeStr_MainType.at(mainType);
            break;

            /* 词法节点 */
        case TreeMainType::IntegerLeterial:
            str = nodeStrConst.nodeStr_Integer(node->NumValue.Get());
            break;
        case TreeMainType::Identifier:
            str = nodeStrConst.nodeStr_Identifier(node->StrValue.Get());
            break;

            /* 具有子类型的节点 Type */
        case TreeMainType::Type:
            str = nodeStrConst.nodeStr_Type.at(subType_Type);
            break;

            /* 具有子类型的节点 Statement */
        case TreeMainType::Statement:
            str = nodeStrConst.nodeStr_Statement.at(subType_Statement);
            break;

            /* 具有子类型的节点 Expression */
        case TreeMainType::Expression:
            if (subType_Expression == TreeSubType_Expression::Exp_Operate) {
                str = nodeStrConst.nodeStr_Expression.at(TreeSubType_Expression::Exp_Operate) + node->StrValue.Get();
            }
            else {
                str = nodeStrConst.nodeStr_Expression.at(subType_Expression);
            }
            break;

        default:;/* never */
        }
        return str;
    }

};
