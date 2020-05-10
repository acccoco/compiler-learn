#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
using namespace std;
#include "Property.h"
#include "../LexicalAnalyzer/Token.h"
#include "EToken.h"


/*
 用于读取词法分析tokens的类
 这个类只是用来读取token的，不应该插手错误处理
*/
class TokenReader {
private:
    vector<ETokenPtr> _tokens;          // tokens总体
    int _tokenCount;                    // tokens的总个数
    int _curIndex;                      // 当前token的数组下标，即将被识别。GetToken()和SeekToken()方法获得的就是这个token
public:

    /*
     构造函数：
     通过Token*[][]构造Reader
     通过数组下标推断行号
    */
    TokenReader(const vector<vector<TokenPtr>>& tokens):
        _curIndex(0) {
        _tokenCount = 0;
        for (size_t i = 0; i < tokens.size(); ++i) {
            for (size_t j = 0; j < tokens[i].size(); ++j) {
                _tokens.push_back(ETokenPtr(new EToken(tokens[i][j], i + 1, j + 1)));
                ++_tokenCount;
            }
        }
    }

    ROProperty<int> Index = ROProperty<int>(_curIndex);     // reader当前的指针位置
    Property<int> TokenCount = Property<int>(_tokenCount);      // reader里面token的总数

    /*
     判断是否读完了
     如果读完了，那么指针值应该是tokenCount
     异常：无
    */
    bool IsEnd() const noexcept {
        return _curIndex == _tokenCount;
    }

    /*
     读取当前指针指向的token，，然后指针后移
     如果指针越界，就返回null
     异常：无
    */
    ETokenPtr GetToken() noexcept {
        if (_curIndex >= _tokenCount) {
            return NULL;
        }
        return _tokens[_curIndex++];
    }

    /*
     读取当前指针指向的Token，不会改变指针的值
     如果没有可读的，就返回空指针
     异常：无
    */
    ETokenPtr SeekToken() const noexcept {
        return _curIndex < _tokenCount ? _tokens[_curIndex] : NULL;
    }

    /*
     当前指针指向的tokne的行号
     如果没有，就返回0
     异常：无
    */
    size_t GetCurLineNum() const noexcept {
        if (_curIndex < _tokenCount) {
            return _tokens[_curIndex]->LineNum.Get();
        }
        else {
            return 0;
        }
    }
    /*
     上一个指针指向的tokne的行号
     如果没有，就返回0
     异常：无
    */
    size_t GetLastLineNum() const  noexcept {
        auto lastIndex = _curIndex - 1;
        if (0 <= lastIndex && lastIndex < _tokenCount) {
            return _tokens[lastIndex]->LineNum.Get();
        }
        else {
            return 0;
        }
    }

    /*
     恢复Index指针
     一般用于出现错误后，将reader回退到错误发生前
     异常：不会抛出异常
    */
    void RecoveryIndex(int index) {
        _curIndex = index;
    }
};
/*
 用于读取词法分析tokens的类
*/
typedef std::shared_ptr<TokenReader> ReaderPtr;