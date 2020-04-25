#include <vector>
#include <fstream>
#include <string>

using namespace std;

/*
 读取文件为字符串数组
*/

class FileReader {
public:
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
};