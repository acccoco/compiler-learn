/**********************************
 词法分析器
   - 功能1：对一行字符串进行词法分析，输出错误 Token 和 错误信息
   - 功能2：对一个文件进行词法分析，将 错误信息 和 Token信息输出到指定文件
***********************************/
#pragma once
#include <fstream>
#include "TokenError.h"
#include "utils.h"
#include "LexicalDFA.h"

class LexialAnalyzer {

public:
    /**
     词法分析
     对输入文件流的内容进行词法分析，将Token和错误信息写入输出文件流
    */
    void LexicalAnlyze(ifstream& fileIn, ofstream& fileOut) {

        vector<vector<shared_ptr<Token>>> tokens;
        vector<TokenError> tokenErrors;
        shared_ptr<LexicalDFA> DFA = LexicalDFA::GetInstance();

        // 读入文件内容
        vector<string> rawStrs = readFile(fileIn);

        // 一行一行地进行词法分析
        for (int lineNum = 1; lineNum <= rawStrs.size(); ++lineNum) {
            vector<shared_ptr<Token>> lineTokens;
            lexicalAnlyzeOneLine(DFA, lineTokens, tokenErrors, rawStrs[lineNum - 1], lineNum);
            tokens.push_back(lineTokens);
        }



        // 写入文件
        write2File(tokens, tokenErrors, fileOut);
        // 释放资源
        LexicalDFA::DestroyInstance();
    }




    /**
     对一行字符串进行词法分析
     返回token和错误信息
    */
    void lexicalAnlyzeOneLine(shared_ptr<LexicalDFA> DFA, vector<shared_ptr<Token>>& tokens,
        vector<TokenError>& tokenErrors, string rawStr, int lineNum) {

        int index = 0;
        int lineLength = rawStr.length();
        while (index < lineLength) {
            TokenErrorType tokenError = DFA->Parse(rawStr, index);
            shared_ptr<Token> token = DFA->GetToken();
            string errStr = DFA->GetError();

            // 正常识别以及错误处理
            errorHandling(lineNum, rawStr, index, DFA, tokenError, tokens, tokenErrors);
        }
    }
private:
    /* 一次性读取文件 */
    vector<string> readFile(ifstream& ifs) {
        vector<string> rawStrs;
        string lineStr;
        do {
            getline(ifs, lineStr, '\n');
            rawStrs.push_back(lineStr);
        } while (ifs.good());
        return rawStrs;
    }

    /**
     将token和错误信息写入到输出文件流中
    */
    void write2File(const vector<vector<shared_ptr<Token>>>& tokens, vector<TokenError> tokenErrors, ofstream& fileOut) {
        /*
         写入错误
         #Errors: 1
         xxxxx
        */
        fileOut << "#Errors: " << tokenErrors.size() << endl;
        for (auto tokenError : tokenErrors) {
            fileOut << tokenError.errMsg << endl;
        }
        /*
         写入Token
         #Token:
         #line 1
         KEYWORD int
         IDENTIFIER acc
         SYMBOL ;
         #line 2
         */
        fileOut << "#Token: " << endl;
        for (int lineNum = 1; lineNum <= tokens.size(); ++lineNum) {
            fileOut << "#line " << lineNum << endl;
            for (auto token : tokens[lineNum - 1]) {
                switch (token->type)
                {
                case TokenTypeEnum::IDENTIFIER:
                    fileOut << "IDENTIFIER " << token->strValue << endl;
                    break;
                case TokenTypeEnum::SYMBOL:
                    fileOut << "SYMBOL " << token->strValue << endl;
                    break;
                case TokenTypeEnum::KEYWORD:
                    fileOut << "KEYWORD " << token->strValue << endl;
                    break;
                case TokenTypeEnum::INTEGER:
                    fileOut << "INTEGER " << token->numValue << endl;
                    break;
                }
            }
        }
    }

    

    /*
     从identifier中找出keyword
     需要是IDENTIFIER类型
     */
    void findKeyword(string rawStr, int& index, shared_ptr<Token> token) {
        if (token->type != TokenTypeEnum::IDENTIFIER)
            return;
        if (IsKeyword(token->strValue)) {   // 识别一般关键字
            token->type = TokenTypeEnum::KEYWORD;
            return;
        }
        /*
         System.out.println
               |index      |index+12
         */
        if (token->strValue == "System" 			// 处理System.out.println，需要考虑println之后的字符
            && index + 11 < rawStr.length()
            && rawStr.substr(index, 12) == ".out.println") {
            if (index + 12 < rawStr.length()
                && JudgeCharType(rawStr[index + 12]) != CharTypeEnum::SPACE
                && JudgeCharType(rawStr[index + 12]) != CharTypeEnum::SYMBOL) {
                return;
            }
            else {
                token->type = TokenTypeEnum::KEYWORD;
                token->strValue = "System.out.println";
                index += 12;
                return;
            }
        }
    }

    /*
     错误处理
     移动index指针
     并且将这之间的字符加入到错误信息中
     */
    void errorHandling(int lineNum, string rawStr, int& index, shared_ptr<LexicalDFA> dfa, TokenErrorType tokenError,
        vector<shared_ptr<Token>>& tokens, vector<TokenError>& tokenErrors) {

        // lambda：走到下一个space或symbol的地方
        auto toNextSplit = [&index, &rawStr]() {
            string addedErrStr = "";    // 之后附加的错误信息
            // 首先需要判断第一个错误字符是否有着分割的作用
            CharTypeEnum curCharType = JudgeCharType(rawStr[index]);
            if (curCharType == CharTypeEnum::SPACE || curCharType == CharTypeEnum::SYMBOL) {
                ++index;
                return addedErrStr;
            }
            // 第一个错误字符不具有分割作用，可以再往下走了
            ++index;
            while (index < rawStr.size()) {
                curCharType = JudgeCharType(rawStr[index]);
                if (curCharType == CharTypeEnum::SPACE || curCharType == CharTypeEnum::SYMBOL) {
                    break;
                }
                else {
                    addedErrStr += rawStr[index];
                    ++index;
                }
            }
            return addedErrStr;
        };

        shared_ptr<Token> token = dfa->GetToken();
        string errStr = dfa->GetError();

        // 错误处理
        if (tokenError == TokenErrorType::OK) {     // 没有错误
            if (token->type == TokenTypeEnum::IDENTIFIER)
                findKeyword(rawStr, index, token);
            tokens.push_back(token);
            return;
        }
        else if (tokenError == TokenErrorType::SPACE) { // 全是空格
            return;
        }
        else {      // 有错误
            // 移动index，直到下一个space或symbol，并且将内容加到错误信息中
            errStr += toNextSplit();
            tokenErrors.push_back(TokenError(lineNum, index + 1, tokenError, errStr));
            return;
        }
    }

};


