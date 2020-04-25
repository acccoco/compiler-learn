#include <iostream>
#include <fstream>
using namespace std;
#include "LexicalAnalyzer.h"


/**
 词法分析的主程序。命令行参数：
   参数1：输入文件
   参数2：输出文件
*/
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

	// 进行词法分析
	LexialAnalyzer().LexicalAnlyze(fileIn, fileOut);

	// 关闭文件
	fileIn.close();
	fileOut.close();
	return 0;
}

