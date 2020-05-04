#include <vector>
#include <fstream>
#include <string>

using namespace std;

/*
 处理文件输入的类
*/
class FileReader {
public:
    /*
     读取指定文件，返回一个字符串数组，每一个元素代表一行字符串
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