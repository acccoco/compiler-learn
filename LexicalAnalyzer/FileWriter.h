#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <list>
using namespace std;



/* 将数据写入文件的辅助类 */
class FileWriter {
public:

    /* 向输出文件流中写入字符串数组 */
    void write2File(ofstream& of, vector<string> strs) {
        for (string str : strs) {
            of << str << endl;
        }
    }

    /* 向输出文件流中写入字符串数组 */
    void write2File(ofstream& of, list<string> strs) {
        for (string str : strs) {
            of << str << endl;
        }
    }
};