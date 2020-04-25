#include "SyntaxParser.h"
#include <iostream>
#include <vector>
#include <string>

class ParserTest {
private:
    /*
     打印当前reader的状态
    */
    void _PrintReaderState() {
        auto reader = parser->_reader;
        cout << "Reader状态: " << endl;
        cout << "   index: " << reader->GetIndex() << endl;
        cout << "   lineNum: " << reader->GetCurLineNum() << endl;
        cout << "   token: " << reader->SeekToken()->GetStrValue() << ", " << reader->SeekToken()->GetNumValue() << endl;
    }
public:
    shared_ptr<SyntaxParser> parser;
    /*
     初始化测试对象
    */
    void Init(vector<vector<shared_ptr<Token>>> tokens) {
        parser = shared_ptr<SyntaxParser>(new SyntaxParser(tokens));
    }

    /*
     测试MatchKeywordOrSymbol函数
    */
    void MatchKeywordOrSymbol(TokenTypeEnum tokenType, string str) {
        cout << __FUNCTION__ << endl;
        _PrintReaderState();
        bool result = parser->_MatchKeywordOrSymbol(tokenType, str);
        cout << "match result: " << result << endl;
        _PrintReaderState();
    }
};