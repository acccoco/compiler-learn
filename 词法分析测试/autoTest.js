// 语法分析批处理文件
const cp = require("child_process");


// tokenOut部分
cp.execFileSync("./词法分析.exe", ["./词法分析测试文件/标识符/test1.txt", "./tokenOut/tokenOut_1.txt"]);
cp.execFileSync("./词法分析.exe", ["./词法分析测试文件/标识符/test2.txt", "./tokenOut/tokenOut_2.txt"]);
cp.execFileSync("./词法分析.exe", ["./词法分析测试文件/关键字/test3.txt", "./tokenOut/tokenOut_3.txt"]);
cp.execFileSync("./词法分析.exe", ["./词法分析测试文件/关键字/test4.txt", "./tokenOut/tokenOut_4.txt"]);
cp.execFileSync("./词法分析.exe", ["./词法分析测试文件/整数/test5.txt", "./tokenOut/tokenOut_5.txt"]);
cp.execFileSync("./词法分析.exe", ["./词法分析测试文件/专用符号/test6.txt", "./tokenOut/tokenOut_6.txt"]);
cp.execFileSync("./词法分析.exe", ["./词法分析测试文件/专用符号/test7.txt", "./tokenOut/tokenOut_7.txt"]);

// 测试部分
