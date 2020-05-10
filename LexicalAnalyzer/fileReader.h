#pragma once
#include <vector>
#include <fstream>
#include <string>

using namespace std;

/*
 从文件读出数据
*/
class FileReader {
public:
    /*
     读取一个文件
     返回：字符串数组，一个元素表示一行
    */
    vector<string> readFile(ifstream& ifs) {
        vector<string> rawStrs;
        string lineStr;
        do {
            getline(ifs, lineStr, '\n');
            rawStrs.push_back(lineStr);
        } while (ifs.good());
        return rawStrs;
    }
};