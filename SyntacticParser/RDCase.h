#pragma once
#include "TokenReader.h"
#include "SyntaxTreeType.h"
#include "SyntaxTreeNode.h"
#include "SyntaxError.h"
#include <list>

/*
 递归下降的一个实例
*/
class RDCase {
public:

    // 该次递归下降过程产生的错误
    ROProperty<list<SyntaxErrorPtr>> Errors = ROProperty<list<SyntaxErrorPtr>>(_errors);
    ROProperty<SyntaxTreeNodePtr> Root = ROProperty<SyntaxTreeNodePtr>(_root);

    /* 构造函数 */
    RDCase(shared_ptr<TokenReader> reader) noexcept
        : _reader(reader), _curIndex(reader->Index.Get()) {}

    /* 主函数，会抛出异常 */
    virtual SyntaxTreeNodePtr RunException(SyntaxTreeNodePtr firstRoot = nullptr) = 0;

    /* 递归下降的主函数，不会抛出异常 */
    SyntaxTreeNodePtr Run(SyntaxTreeNodePtr firstRoot = nullptr) noexcept {
        try {
            RunException(firstRoot);
        }
        catch (list<SyntaxErrorPtr> errors) {
            // _errors.emplace_back(new SyntaxError(_reader, "Error", _treeType));
            _errors.insert(_errors.end(), errors.begin(), errors.end());
            _root = nullptr;
        }
        catch (SyntaxErrorPtr error) {
            // _errors.emplace_back(new SyntaxError(_reader, "Error", _treeType));
            _errors.push_back(error);
            _root = nullptr;
        }
        return _root;
    }

protected:
    shared_ptr<TokenReader> _reader;
    const int _curIndex;                      // 进入时的位置，用于回退
    TreeType _treeType;
    list<SyntaxErrorPtr> _errors;
    SyntaxTreeNodePtr _root = nullptr;

    /*
     以终结符开始的产生式的初始化方法
     初始化函数，设置类型，初始化 root
     检查是否存在第一个token，如果不存在，则抛出异常
    */
    void _InitT(TreeType treeType, string msg = "") {
        _treeType = treeType;
        if (msg == "")
            msg = "识别初始token失败";
        ETokenPtr firstToken = _reader->SeekToken();
        if (firstToken == nullptr) {
            _reader->Index.Set(_curIndex);
            throw SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
        }
        _root = SyntaxTreeNodePtr(new SyntaxTreeNode(treeType, firstToken->LineNum.Get()));
    }
    /*
     以非终结符开始的产生式的初始化方法
     初始化函数，设置类型，初始化 root
     检查是否提供了firstRoot，不存在，则抛出异常
     检查是否存在第一个token，如果不存在，则抛出异常
    */
    void _InitNT(TreeType treeType, SyntaxTreeNodePtr firstRoot, string msg = "") {
        if (msg == "")
            msg = "识别初始token失败";
        ETokenPtr firstToken = _reader->SeekToken();
        if (firstToken == nullptr) {
            _reader->Index.Set(_curIndex);
            throw SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
        }
        if (firstRoot == nullptr) {
            _reader->Index.Set(_curIndex);
            throw "第一子节点不能为空";
        }

        _root = SyntaxTreeNodePtr(new SyntaxTreeNode(treeType, firstRoot->LineNum.Get()));
    }

#pragma region 识别函数，不抛出异常

    /* 匹配 Keyword。若匹配失败，不移动指针。 */
    bool _MatchKeyword(const string str) noexcept {
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::KEYWORD && token->StrValue.Get() == str) {
            _reader->GetToken();
            return true;
        }
        else
            return false;
    }
    /* 匹配 Symbol。若匹配失败，不移动指针。 */
    bool _MatchSymbol(const string str) noexcept {
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::SYMBOL && token->StrValue.Get() == str) {
            _reader->GetToken();
            return true;
        }
        else
            return false;
    }

    /* 匹配 Identifier。若匹配失败，不移动指针。 */
    SyntaxTreeNodePtr _MatchIdentifier() noexcept {
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::IDENTIFIER) {
            token = _reader->GetToken();
            return SyntaxTreeNodePtr(new SyntaxTreeNode(token));
        }
        else
            return nullptr;
    }

    /* 匹配 IntegerLiteral。若匹配失败，不移动指针。 */
    SyntaxTreeNodePtr _MatchIntegerLiteral() noexcept {
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::INTEGER) {
            token = _reader->GetToken();
            return SyntaxTreeNodePtr(new SyntaxTreeNode(token));
        }
        else
            return nullptr;
    }
#pragma endregion

#pragma region 检查函数，会抛出异常

    /*  检查关键字是否匹配。失败则抛出异常 */
    void _CheckKeyword(const string str, string msg = "") {
        if (msg == "")
            msg = "匹配关键字失败: " + str;
        ETokenPtr token = _reader->SeekToken();
        if (!token || token->TokenType.Get() != TokenTypeEnum::KEYWORD || token->StrValue.Get() != str) {
            _reader->Index.Set(_curIndex);
            throw SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
        }
        else
            _reader->GetToken();
    }

    /*  检查符号是否匹配。失败则抛出异常 */
    void _CheckSymbol(const string str, string msg = "") {
        if (msg == "")
            msg = "匹配符号失败: " + str;
        ETokenPtr token = _reader->SeekToken();
        if (!token || token->TokenType.Get() != TokenTypeEnum::SYMBOL || token->StrValue.Get() != str) {
            _reader->Index.Set(_curIndex);
            throw SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
        }
        else
            _reader->GetToken();
    }
    /* 检查 Identifier 是否匹配，失败则抛出异常 */
    SyntaxTreeNodePtr _CheckIdentifier(string msg = "") {
        if (msg == "")
            msg = "匹配 Identifier 失败";
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::IDENTIFIER) {
            token = _reader->GetToken();
            return SyntaxTreeNodePtr(new SyntaxTreeNode(token));
        }
        else {
            _reader->Index.Set(_curIndex);
            throw SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
        }

    }
    /* 检查 IntegerLiteral 是否匹配，失败则抛出异常 */
    SyntaxTreeNodePtr _CheckIntegerLiteral(string msg = "") {
        if (msg == "")
            msg = "匹配 IntegerLiteral 失败";
        ETokenPtr token = _reader->SeekToken();
        if (token && token->TokenType.Get() == TokenTypeEnum::INTEGER) {
            token = _reader->GetToken();
            return SyntaxTreeNodePtr(new SyntaxTreeNode(token));
        }
        else {
            _reader->Index.Set(_curIndex);
            throw SyntaxErrorPtr(new SyntaxError(_reader, msg, _treeType));
        }

    }

    /* 检查递归下降的过程是否成功。如果不成功，就抛出异常 */
    void _CheckSubTree(const RDCase* const subTree) {
        if (subTree->Root.Get() == nullptr) {
            _reader->Index.Set(_curIndex);
            throw subTree->Errors.Get();
        }

    }

#pragma endregion

};
