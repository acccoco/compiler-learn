#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
using namespace std;
#include "../LexicalAnalyzer/Token.h"
#include "EToken.h"


/*
 用于读取tokens的类
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
    */
    TokenReader(const vector<vector<TokenPtr>>& tokens) {
        _curIndex = 0;
        _tokenCount = 0;
        for (int i = 0; i < tokens.size(); ++i) {
            for (int j = 0; j < tokens[i].size(); ++j) {
                _tokens.push_back(ETokenPtr(new EToken(tokens[i][j], i + 1, j + 1)));
                ++_tokenCount;
            }
        }
    }

    /*
     判断是否读完了
     当前下标是否越界
    */
    bool IsEnd() const {
        return _curIndex == _tokenCount;
    }
    /*
     指针回退多步
     如果无法回退，则返回false
    */
    bool GoBack(int step) {
        if (_curIndex >= step) {
            return false;
        }
        else {
            _curIndex -= step;
            return true;
        }
    }
    /*
     读取一个token，然后指针后移
     如果越界，则返回NULL
    */
    ETokenPtr GetToken() {
        if (_curIndex >= _tokenCount) {
            return NULL;
        }
        return _tokens[_curIndex++];
    }

    /*
     看一眼当前的Token
     如果没有可读的，就返回空指针
    */
    ETokenPtr SeekToken() const {
        return _curIndex < _tokenCount ? _tokens[_curIndex] : NULL;
    }

    /*
     正要读取的token的行号
     如果没有，就返回-1
    */
    int GetCurLineNum() const {
        if (_curIndex < _tokenCount) {
            return _tokens[_curIndex]->GetLineNum();
        }
        else {
            return -1;
        }
    }
    /*
     上一个读取完的token的行号
     如果没有，就返回-1
    */
    int GetLastLineNum() const {
        auto lastIndex = _curIndex - 1;
        if (0 <= lastIndex && lastIndex < _tokenCount) {
            return _tokens[lastIndex]->GetLineNum();
        }
        else {
            return -1;
        }
    }

    /*
     获取当前的Index，用于回退
    */
    int GetIndex() const {
        return _curIndex;
    }

    /*
     设置Index，用于回退
    */
    void SetIndex(int index) {
        _curIndex = index;
    }
};
/*
 Reader类的指针类型
*/
typedef std::shared_ptr<TokenReader> ReaderPtr;