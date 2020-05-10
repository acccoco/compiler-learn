#include "../LexicalAnalyzer/Token.h"
#include "../LexicalAnalyzer/LexicalAnalyzer.h"
#include "../LexicalAnalyzer/fileReader.h"
#include "SyntaxWriter.h"
#include "../LexicalAnalyzer/FileWriter.h"
#include <vector>
#include <iostream>
using namespace std;

#if true
int main(int argc, char** argv) {

	bool flagPrintConsole = true;		// 是否将结果打印到控制台
	bool flagWrite2File = true;		// 是否将结果输出到文件

	/* 读取命令行参数 */
	if (argc != 3) {
		cout << "命令参数错误" << endl;
		return 0;
	}

	/* 打开文件，读取文件 */
	string filePathIn, filePathOut;
	filePathIn = argv[1];
	filePathOut = argv[2];
	ifstream fileIn;
	ofstream fileOut;
	fileIn.open(filePathIn, ios::in);
	fileOut.open(filePathOut, ios::out | ios::trunc);
	if (!fileIn.is_open()) {
		cout << "打开输入文件失败" << endl;
		return 0;
	}
	if (!fileOut.is_open()) {
		cout << "打开输出文件失败" << endl;
		return 0;
	}
	vector<string> rawStrs = FileReader().readFile(fileIn);

	/* 词法分析 */
	LexialAnalyzer lexicalAnalyzer;
	lexicalAnalyzer.LexicalAnlyze(rawStrs);
	auto tokens = lexicalAnalyzer.GetResultTokens();
	auto lexicalErrors = lexicalAnalyzer.GetResultErrors();

	/* 判断词法分析的结果 */
	if (lexicalErrors.size() != 0) {
		cout << "词法分析有错误，无法进行语法分析" << endl;
		return 0;
	}

	/* 进行语法分析 */
	auto reader = ReaderPtr(new TokenReader(tokens));
	auto syntaxParser = SyntaxParser(reader);
	syntaxParser.Parse();
	auto syntaxTreeStrs = syntaxParser.getSerialTree();
	auto syntaxTreeRoot = syntaxParser.Root.Get();
	auto syntaxErrors = syntaxParser.ErrorList.Get();
	auto syntaxErrorStrs = syntaxParser.getErrorStrs();
	
	/* 打印语法分析的结果 */
	if (flagPrintConsole) {

		/* 打印错误到控制台 */
		cout << endl << "Syntax Errors: " << endl;
		for (string syntaxErrorStr : syntaxErrorStrs) {
			cout << syntaxErrorStr << endl;
		}
		cout << endl;

		/* 打印树节点到控制台 */
		cout << "Syntax Tree: " << endl;
		for (string str : syntaxTreeStrs) {
			cout << str << endl;
		}
		cout << endl;
	}

	/* 结果输出到文件 */
	if (flagWrite2File) {
		FileWriter fileWriter;

		/* 输出错误到文件 */
		fileWriter.write2File(fileOut, vector<string>{"Syntax Errors:"});
		fileWriter.write2File(fileOut, syntaxErrorStrs);

		/* 输出树节点到文件 */
		fileWriter.write2File(fileOut, vector<string>{"Syntax Tree"});
		fileWriter.write2File(fileOut, syntaxTreeStrs);
	}

	/* 关闭文件 */
	fileIn.close();
	fileOut.close();

	return 0;
}

#endif
