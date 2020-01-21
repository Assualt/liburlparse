/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2020年01月17日 星期五 12时42分54秒
 ************************************************************************/

#include<iostream>
#include"tstringbuilder.h"
using namespace std;

int main(int argc,char **argv){
    TStringBuilder temp;
    temp.append("hello world").append(1).append('c').append(-123).append(-34.33);

    std::cout << temp.toString() << std::endl;
    
    TStringBuilder an;
    TStringBuilder::copyOf(temp, an);

    std::cout << an.toString() << std::endl;
    return 0;
}
