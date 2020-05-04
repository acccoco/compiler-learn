#include "../LexicalAnalyzer/Token.h"
#include "../LexicalAnalyzer/LexicalAnalyzer.h"
#include "../LexicalAnalyzer/fileReader.h"
#include "SyntaxWriter.h"
#include <vector>
#include <iostream>
using namespace std;

#if false
int main(int argc, char** argv) {
	// 读取命令行参数
	if (argc != 3) {
		cout << "命令参数错误" << endl;
		return 0;
	}
	string fileInPath, fileOutPath;
	fileInPath = argv[1];
	fileOutPath = argv[2];

	// 读取文件
	ifstream fileIn;
	ofstream fileOut;
	fileIn.open(fileInPath, ios::in);
	fileOut.open(fileOutPath, ios::out | ios::trunc);
	if (!fileIn.is_open()) {
		cout << "打开输入文件失败" << endl;
		return 0;
	}
	if (!fileOut.is_open()) {
		cout << "打开输出文件失败" << endl;
		return 0;
	}
	vector<string> rawStrs = FileReader().readFile(fileIn);

	// 词法分析
	LexialAnalyzer analyzer;
	analyzer.LexicalAnlyze(rawStrs);
	auto tokens = analyzer.GetResultTokens();
	auto tokenErrors = analyzer.GetResultErrors();

	// 关闭文件
	fileIn.close();
	fileOut.close();

	// 语法分析


	// 写入结果


	return 0;
}

#endif
