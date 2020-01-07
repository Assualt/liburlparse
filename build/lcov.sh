#!/bin/bash 

#1. make 添加参数 -fprofile-arcs -ftest-coverage 生成 gcno文件
#2. 执行 生成 gcda文件
#3. lcov -d . -t 'unitmain' -o 'unitmain.info' -b . -c 生成 unitmain.info文件
#4. genhtml -o result hello_test.info   
#5. python3 -m http.server 8080 

lcov -d . -t 'unitmain' -o 'unitmain.info' -b . -c
genhtml -o result unitmain.info
cd result && python3 -m http.server 8080
