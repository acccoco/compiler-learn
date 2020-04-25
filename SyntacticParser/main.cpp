/*************************************
 主程序入口
 TODO：错误处理
 TODO：合并两个工程
*******************************************/
#include "../LexicalAnalyzer/Token.h"
#include "../LexicalAnalyzer/LexicalAnalyzer.h"
#include "../LexicalAnalyzer/fileReader.h"
#include "ParserTest.h"
#include "SyntaxWriter.h"
#include <vector>
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
	// 判断参数是否合法
	if (argc != 3) {
		cout << "命令参数错误" << endl;
		return 0;
	}
	// 读取参数
	string fileInPath, fileOutPath;
	fileInPath = argv[1];
	fileOutPath = argv[2];
	// 打开文件
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

	// 读取文件
	auto rawStrs = FileReader().readFile(fileIn);
	// 进行词法分析
	LexialAnalyzer analyzer;
	analyzer.LexicalAnlyze(rawStrs);
	// 获得结果
	auto tokens = analyzer.GetResultTokens();
	auto tokenErrors = analyzer.GetResultErrors();
	// 关闭文件
	fileIn.close();
	fileOut.close();

	// 词法分析
	SyntaxParser parser(tokens);
	parser.Parse();
	auto root = parser.GetRoot();
	auto syntaxErrors = parser.GetErrorList();

	SyntaxWriter writer;
	auto result = writer.Serialize(writer.PreOrderTraval(root, 0));
	for (auto data : result) {
		cout << data << endl;
	}

	return 0;
}