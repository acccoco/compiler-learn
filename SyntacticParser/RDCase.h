#pragma once
#include "TokenReader.h"
#include "TreeType.h"
#include "TreeNode.h"
#include "SyntaxError.h"
#include <list>


/*
 递归下降一个产生式的抽象
 将这个过程使用一个类来表示
 调用Run()函数，就是运行这个递归下降过程
*/
class RDCase {
public:

    // 该次递归下降过程产生的错误
    ROProperty<list<SyntaxErrorPtr>> Errors = ROProperty<list<SyntaxErrorPtr>>(_errors);

    // 该次递归下降得到的语法树的根节点
    ROProperty<TreeNodePtr> Root = ROProperty<TreeNodePtr>(_root);

    /*
     提供token reader来构造一个递归下降过程
     不会抛出异常
    */
    RDCase(shared_ptr<TokenReader> reader) noexcept
        : _reader(reader), _curIndex(reader->Index.Get()) {}



    /*
     递归下降的主函数
     异常：不会抛出异常
     */
    TreeNodePtr Run(TreeNodePtr firstRoot = nullptr) noexcept {
        try {
            _RunException(firstRoot);
        }
        catch (list<SyntaxErrorPtr> errors) {
            _errors.insert(_errors.end(), errors.begin(), errors.end());
            _root = nullptr;
        }
        catch (SyntaxErrorPtr error) {
            _errors.push_back(error);
            _root = nullptr;
        }
        return _root;
    }

protected:

    /*
     主函数
     异常：会抛出异常
     */
    virtual void _RunException(TreeNodePtr firstRoot = nullptr) = 0;

    shared_ptr<TokenReader> _reader;    // 引用的token reader
    const int _curIndex;            // 进入时的位置，用于回退。常量
    TreeType _treeType;     // 产生式非终结符对应的树节点类型
    list<SyntaxErrorPtr> _errors;       // 该产生式过程产生的错误
    TreeNodePtr _root = nullptr;    // 产生式得到的树节点

    /*
     产生式的初始化方法
     初始化函数，设置类型，初始化 root
     异常：检查是否存在第一个token，如果不存在，则抛出异常
    */
    void _Init(TreeType treeType, string msg = "") {
        _treeType = treeType;
        if (msg == "")
            msg = "识别初始token失败";
        ETokenPtr firstToken = _reader->SeekToken();
        if (firstToken == nullptr) {
            auto error = SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
            _reader->RecoveryIndex(_curIndex);
            throw error;
        }
        _root = TreeNodePtr(new TreeNode(treeType, firstToken->LineNum.Get()));
    }
    /*
     左递归产生式的初始化方法
     初始化函数，设置类型，初始化 root
     异常：检查是否提供了firstRoot，不存在，则抛出异常
     异常：检查是否存在第一个token，如果不存在，则抛出异常
    */
    void _InitRecursive(TreeType treeType, TreeNodePtr firstRoot, string msg = "") {
        if (msg == "")
            msg = "识别初始token失败";
        ETokenPtr firstToken = _reader->SeekToken();
        if (firstToken == nullptr) {
            auto error = SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
            _reader->RecoveryIndex(_curIndex);
            throw error;
        }
        if (firstRoot == nullptr) {
            auto error = SyntaxErrorPtr(new SyntaxError(_reader, "第一子节点不能为空", _treeType));
            _reader->RecoveryIndex(_curIndex);
            throw error;
        }

        _root = TreeNodePtr(new TreeNode(treeType, firstRoot->LineNum.Get()));
    }

#pragma region 识别函数，不抛出异常

    /*
     匹配 Keyword。
     若匹配失败，不移动指针。
     若匹配成功，返回true
     异常：无异常
     */
    bool _MatchKeyword(const string str) noexcept {
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::KEYWORD && token->StrValue.Get() == str) {
            _reader->GetToken();
            return true;
        }
        else
            return false;
    }
    /*
    匹配 Symbol。
    若匹配失败，不移动指针。
    匹配成功，返回true
    异常：无异常
    */
    bool _MatchSymbol(const string str) noexcept {
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::SYMBOL && token->StrValue.Get() == str) {
            _reader->GetToken();
            return true;
        }
        else
            return false;
    }

    /*
    匹配 Identifier。
    若匹配失败，不移动指针。
    匹配成功，返会主类型为Idntifier的树节点
    异常：无异常
    */
    TreeNodePtr _MatchIdentifier() noexcept {
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::IDENTIFIER) {
            token = _reader->GetToken();
            return TreeNodePtr(new TreeNode(token));
        }
        else
            return nullptr;
    }

    /*
    匹配 IntegerLiteral。
    若匹配失败，不移动指针。
    匹配成功，返会主类型为IntegerLiteral的树节点
    异常：无异常
    */
    TreeNodePtr _MatchIntegerLiteral() noexcept {
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::INTEGER) {
            token = _reader->GetToken();
            return TreeNodePtr(new TreeNode(token));
        }
        else
            return nullptr;
    }

    /*
     匹配一系列的产生式，形如：
     { Expression }
     异常：不会抛出异常
    */
    template<class RD_Case>
    TreeNodePtr _MatchCaseSequence() noexcept {

        // 序列的根节点
        TreeNodePtr sequenceNode(new TreeNode(TreeMainType::Default, 0));

        auto rd_case1 = RD_Case(_reader);
        rd_case1.Run();
        if (rd_case1.Root.Get()) {
            sequenceNode->Child.Set(rd_case1.Root.Get());
            TreeNodePtr preNode = rd_case1.Root.Get();
            while (true) {
                auto rd_case2 = RD_Case(_reader);
                rd_case2.Run();
                if (rd_case2.Root.Get() == nullptr)
                    break;
                preNode->Subling.Set(rd_case2.Root.Get());
                preNode = rd_case2.Root.Get();
            }
        }

        return sequenceNode;
    }

#pragma endregion

#pragma region 检查函数，会抛出异常

    /*
    检查关键字是否匹配。
    失败则抛出异常，不会移动reader指针
    成功则后移reader指针一位
    */
    void _CheckKeyword(const string str, string msg = "") {
        if (msg == "")
            msg = "匹配关键字失败: " + str;
        ETokenPtr token = _reader->SeekToken();
        if (!token || token->TokenType.Get() != TokenTypeEnum::KEYWORD || token->StrValue.Get() != str) {
            auto error = SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
            _reader->RecoveryIndex(_curIndex);
            throw error;
        }
        else
            _reader->GetToken();
    }

    /*
    检查符号是否匹配。
    失败则抛出异常 。不会移动reader指针
    成功则后移reader指针一位
    */
    void _CheckSymbol(const string str, string msg = "") {
        if (msg == "")
            msg = "匹配符号失败: " + str;
        ETokenPtr token = _reader->SeekToken();
        if (!token || token->TokenType.Get() != TokenTypeEnum::SYMBOL || token->StrValue.Get() != str) {
            auto error = SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
            _reader->RecoveryIndex(_curIndex);
            throw error;
        }
        else
            _reader->GetToken();
    }

    /*
    检查 Identifier 是否匹配，
    失败则抛出异常。不会移动reader指针
    成功则后移reader指针一位，并返回主类型为Identifier的树节点
    */
    TreeNodePtr _CheckIdentifier(string msg = "") {
        if (msg == "")
            msg = "匹配 Identifier 失败";
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::IDENTIFIER) {
            token = _reader->GetToken();
            return TreeNodePtr(new TreeNode(token));
        }
        else {
            auto error = SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
            _reader->RecoveryIndex(_curIndex);
            throw error;
        }

    }
    /*
    检查 IntegerLiteral 是否匹配，
    失败则抛出异常。不会移动reader指针
    成功则后移reader指针一位，并返回主类型为IntegerLiteral的树节点
    */
    TreeNodePtr _CheckIntegerLiteral(string msg = "") {
        if (msg == "")
            msg = "匹配 IntegerLiteral 失败";
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::INTEGER) {
            token = _reader->GetToken();
            return TreeNodePtr(new TreeNode(token));
        }
        else {
            auto error = SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
            _reader->RecoveryIndex(_curIndex);
            throw error;
        }

    }

    /*
    检查递归下降的过程是否成功。
    如果不成功，就抛出异常，异常内容是从树节点读取到的错误信息。不会移动reader指针
    */
    void _CheckSubTree(const RDCase* const subTree) {
        if (subTree->Root.Get() == nullptr) {
            _reader->RecoveryIndex(_curIndex);
            throw subTree->Errors.Get();
        }
    }

    /*
    检查递归下降的过程是否成功。
    如果不成功，就抛出异常，异常内容是从树节点读取到的错误信息。不会移动reader指针
    如果成功，就返回相应的树节点
    */
    template <class RD_Case>
    TreeNodePtr _CheckRDCase(shared_ptr<TokenReader> reader) {
        RD_Case rd_case = RD_Case(_reader);
        rd_case.Run();

        if (rd_case.Root.Get() == nullptr) {
            _reader->RecoveryIndex(_curIndex);
            throw rd_case.Errors.Get();
        }

        return rd_case.Root.Get();
    }
#pragma endregion
};


/* 定义一个局部作用域，这里面的语句可以被break中断。如果不被中断，这些语句会顺序执行完 */
#define RUN_ONCE() for(int __once__ = 0; __once__ < 1; ++__once__)
