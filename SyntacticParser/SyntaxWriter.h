#pragma once
#include "SyntaxParser.h"

#if false

/*
 将语法分析的结果序列化，然后写入文件
*/
class SyntaxWriter {
public:
    /*
     前序遍历树
    */
    list<pair<int, string>> PreOrderTraval(shared_ptr<TreeNode> root, const int intendLevel) {
        list<pair<int, string>> rawData;            // 包含缩进级别的节点信息
        list<pair<int, string>> tempResultList;     // 用于存放子节点的结果
        shared_ptr<TreeNode> tempTreeNode;
        switch (root->MainType.Get()) {

            /********************
             Goal
            ********************/
        case TreeNodeMainTypeEnum::Goal:
            rawData.push_back({ intendLevel, "Goal" });
            tempResultList = PreOrderTraval(root->Child.Get(), intendLevel + 1);
            rawData.insert(rawData.end(), tempResultList.begin(), tempResultList.end());
            tempTreeNode = root->Child.Get();
            while (tempTreeNode->Subling.Get()) {
                tempResultList = PreOrderTraval(tempTreeNode->Subling.Get(), intendLevel + 1);
                rawData.insert(rawData.end(), tempResultList.begin(), tempResultList.end());
                tempTreeNode = tempTreeNode->Subling.Get();
            }
            break;
            /********************
             MainClass
            ********************/
        case TreeNodeMainTypeEnum::MainClass:
            rawData.push_back({ intendLevel, "MainClass" });
            tempTreeNode = root->Child.Get();
            rawData.push_back({ intendLevel + 1, string("className: ") + tempTreeNode->StrValue.Get() });
            tempTreeNode = tempTreeNode->Subling.Get();
            rawData.push_back({ intendLevel + 1, string("paramName: ") + tempTreeNode->StrValue.Get() });
            tempTreeNode = tempTreeNode->Subling.Get();
            tempResultList = PreOrderTraval(tempTreeNode, intendLevel + 1);
            rawData.insert(rawData.end(), tempResultList.begin(), tempResultList.end());
            break;
        case TreeNodeMainTypeEnum::ClassDeclaration:

            break;
        case TreeNodeMainTypeEnum::VarDeclaration:

            break;
        case TreeNodeMainTypeEnum::MethodDeclaration:

            break;
        case TreeNodeMainTypeEnum::Type:
            switch (root->GetSubType_Type()) {
            case TreeNodeSubTypeEnum_Type::Type_boolean:
                break;
            case TreeNodeSubTypeEnum_Type::Type_Identifier:
                break;
            case TreeNodeSubTypeEnum_Type::Type_Int:
                break;
            case TreeNodeSubTypeEnum_Type::Type_IntArray:
                break;
            }
            break;
        case TreeNodeMainTypeEnum::Statement:
            switch (root->GetSubType_Statement()) {
            case TreeNodeSubTypeEnum_Statement::Statement_arrayAssign:
                break;
            case TreeNodeSubTypeEnum_Statement::Statement_assign:
                break;
            case TreeNodeSubTypeEnum_Statement::Statement_if:
                break;
            case TreeNodeSubTypeEnum_Statement::Statement_println:
                rawData.push_back({ intendLevel, "Statement-println" });
                tempResultList = PreOrderTraval(root->Child.Get(), intendLevel + 1);
                rawData.insert(rawData.end(), tempResultList.begin(), tempResultList.end());
                break;
            case TreeNodeSubTypeEnum_Statement::Statement_Sequence:
                break;
            case TreeNodeSubTypeEnum_Statement::Statement_while:
                break;
            }

            break;
        case TreeNodeMainTypeEnum::Expression:
            switch (root->GetSubType_Expression()) {
            case TreeNodeSubTypeEnum_Expression::Exp_ArrayExpression:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_Bracket:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_False:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_FunctionCall:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_Identifier:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_IntegerLiteral:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_Length:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_NewIdentifier:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_NewIntArray:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_Not:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_Operate:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_This:
                break;
            case TreeNodeSubTypeEnum_Expression::Exp_True:
                rawData.push_back({ intendLevel, "Expression-true" });
                break;
            }

            break;
        case TreeNodeMainTypeEnum::Default:
            break;
        }

        return rawData;
    }

    /*
     前序遍历的结果序列化
    */
    vector<string> Serialize(list<pair<int, string>> rawData) {
        vector<string> result;
        string temp;
        for (auto data : rawData) {
            temp.clear();
            if (data.first == 0) {
                temp = data.second;
                result.push_back(temp);
                continue;
            }
            for (int i = 1; i < data.first; ++i) {
                temp += "  │";
            }
            temp += "  ├── ";
            temp += data.second;
            result.push_back(temp);
        }
        return result;
    }


};

#endif