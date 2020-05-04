#include "../LexicalAnalyzer/Token.h"
#include "../LexicalAnalyzer/LexicalAnalyzer.h"
#include "../LexicalAnalyzer/fileReader.h"
#include "SyntaxWriter.h"
#include "Expression.h"
#include <vector>
#include <iostream>
using namespace std;

#if true

/*
 通过一系列字符串，生成一个 reader
*/
shared_ptr<TokenReader> ConstructReader(vector<string> strs) {
	LexialAnalyzer analyzer;
	analyzer.LexicalAnlyze(strs);
	auto tokens = analyzer.GetResultTokens();
	auto reader = shared_ptr<TokenReader>(new TokenReader(tokens));

	return reader;
}
int main() {
    vector<string> strs;
    strs.push_back("this");
    auto reader = ConstructReader(strs);
    auto expression = RDCase_Expression(reader);
    expression.Run();
    auto root = expression.Root.Get();
    auto child = root->Child.Get();
}

#endif