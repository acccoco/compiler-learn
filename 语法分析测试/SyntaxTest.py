# coding utf-8
# python3
#
# 语法分析的测试脚本
# 配置要求
# Tree
#  |- SyntaxTest.py
#  |- ../SyntacticParser.exe
#  |- 测试源文件/
#  |- SyntaxOut/

import os
import subprocess

syntaxParserPath = "../SyntacticParser.exe"
sourceDirPath = "./测试源文件"      # 测试用源文件的文件夹
targetDirPath = "./SyntaxOut"      # 测试输出文件的文件夹

# 遍历源文件夹
for root, dir_names, file_names in os.walk(sourceDirPath):
    for file_name in file_names:

        # 构建输入文件的路径
        inputFilePath = os.path.join(root, file_name)

        # 构造输出文件的路径
        outputFilePath = os.path.join(targetDirPath, file_name)

        # 语法分析
        print(file_name)
        cmd = "%s %s %s" % (syntaxParserPath, inputFilePath, outputFilePath)
        subprocess.run(cmd)

print("Syntax Parse Done")