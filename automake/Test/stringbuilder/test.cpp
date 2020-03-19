/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2020年01月16日 星期四 21时40分40秒
 ************************************************************************/

#include<iostream>
#include "stringbuilder_1.h"
using namespace std;

int main(int argc, char **argv){
    TStringBuilder builder("Hello World");

    builder.append("123").append("456").append(123).append(123.231).append('c');

    std::cout << builder.toString() << std::endl;

    TStringBuilder temp(builder.toString());
    std::cout << temp.toString() << std::endl;

    return 0;
}
