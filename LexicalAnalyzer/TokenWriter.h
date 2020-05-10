#pragma once
#include "Token.h"
#include "TokenError.h"
#include <vector>
#include <fstream>
#include <memory>
using namespace std;

/*
 将词法分析得到的一系列token写入文件
*/
class TokenWriter {
public:
    /*
     将token和错误写入指定文件中
    */
    void Write2File(const vector<vector<shared_ptr<Token>>>& tokens,
        const vector<shared_ptr<TokenError>> tokenErrors, ofstream& fileOut) {
        /*
         写入错误
         #Errors: 1
         xxxxx
        */
        fileOut << "#Errors: " << tokenErrors.size() << endl;
        for (auto tokenError : tokenErrors) {
            fileOut << tokenError->errMsg << endl;
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
        for (size_t lineNum = 1; lineNum <= tokens.size(); ++lineNum) {
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
};